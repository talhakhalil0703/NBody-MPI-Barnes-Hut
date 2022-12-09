#include <iostream>
#include <cstring>
#include "io.h"
#include "argparse.h"
#include "body.h"
#include "mpi.h"
#include "quadtree.h"
// Visual Stuff could be moved?

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glu.h>
#include <GL/glut.h>

double THETA;
double dt;
int steps;
bool visuals;
double EPSILON = 1E-14;

using namespace std;

bool approx_equal(double x, double y)
{
   if (x == 0) return fabs(y) <= EPSILON;
   if (y == 0) return fabs(x) <= EPSILON;
   return fabs(x - y) / max(fabs(x), fabs(y)) <= EPSILON;
}

void create_mpi_body_datatype(MPI_Datatype &mystruct)
{
    Body body;
    int blocklens[2];
    MPI_Aint indices[8];
    MPI_Datatype old_types[2];

    /* One value of each type */
    blocklens[0] = 1;
    blocklens[1] = 7;
    /* The base types */
    old_types[0] = MPI_INT;
    old_types[1] = MPI_DOUBLE;

    /* The locations of each element */
    MPI_Address(&body.index, &indices[0]);
    MPI_Address(&body.x_pos, &indices[1]);
    MPI_Address(&body.y_pos, &indices[2]);
    MPI_Address(&body.mass, &indices[3]);
    MPI_Address(&body.x_vel, &indices[4]);
    MPI_Address(&body.y_vel, &indices[5]);
    MPI_Address(&body.x_force, &indices[6]);
    MPI_Address(&body.y_force, &indices[7]);

    /* Make relative */
    indices[1] = indices[1] - indices[0];
    indices[2] = indices[2] - indices[0];
    indices[3] = indices[3] - indices[0];
    indices[4] = indices[4] - indices[0];
    indices[5] = indices[5] - indices[0];
    indices[6] = indices[6] - indices[0];
    indices[7] = indices[7] - indices[0];
    indices[0] = 0;
    MPI_Type_struct(2, blocklens, indices, old_types, &mystruct);
    MPI_Type_commit(&mystruct);
}

void draw_2d_particle(double x_window, double y_window, double radius, double *colors)
{
    int k = 0;
    double angle = 0.0f;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(colors[0], colors[1], colors[2]);
    glVertex2f(x_window, y_window);
    for (k = 0; k < 20; k++)
    {
        angle = (double)(k) / 19 * 2 * 3.141592;
        glVertex2f(x_window + radius * cos(angle),
                   y_window + radius * sin(angle));
    }
    glEnd();
}

void drawOctreeBounds2D(Node *node)
{
    double x_win = 2 * node->x_pos / XLIM - 1;
    double y_win = 2 * node->y_pos / YLIM - 1;
    double x_lim = node->x_lim * 2 / XLIM; // TODO:Not entirely sure if this is correct if lim change
    double y_lim = node->y_lim * 2 / YLIM;

    glBegin(GL_LINES);
    // set the color of lines to be white
    glColor3f(1.0f, 1.0f, 1.0f);
    // specify the start point's coordinates
    glVertex2f(x_win + x_lim, y_win);
    // specify the end point's coordinates
    glVertex2f(x_win - x_lim, y_win);
    // do the same for verticle line
    glVertex2f(x_win, y_win + y_lim);
    glVertex2f(x_win, y_win - y_lim);
    glEnd();
}

void draw_lines(Node *node)
{
    if (node->type == INTERNAL)
    {
        drawOctreeBounds2D(node);
    }
    if (node->nw != nullptr)
        draw_lines(node->nw);
    if (node->ne != nullptr)
        draw_lines(node->ne);
    if (node->se != nullptr)
        draw_lines(node->se);
    if (node->sw != nullptr)
        draw_lines(node->sw);
}

void draw_bodies(Body *body, int count)
{
    for (int i = 0; i < count; i++)
    {
        double x_win, y_win = 0;
        x_win = 2 * body[i].x_pos / XLIM - 1;
        y_win = 2 * body[i].y_pos / YLIM - 1;
        double colors[3] = {0.9, 0, 0};
        draw_2d_particle(x_win, y_win, 0.01, colors);
    }
}

int setup_window(GLFWwindow *window)
{

    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    return 0;
}

void draw(GLFWwindow *window, Node *root, Body *bodies, int number_of_bodies)
{
    if (visuals)
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        if (root->type != INVALID)
        {
            draw_lines(root);
            draw_bodies(bodies, number_of_bodies);
            glfwSwapBuffers(window);
        }
        /* Swap front and back buffers */
        /* Poll for and process events */
        glfwPollEvents();
    }
}

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int tagno = 42;
    MPI_Datatype MPI_Body_datatype;
    MPI_Status status;
    create_mpi_body_datatype(MPI_Body_datatype);

    visuals = false;
    GLFWwindow *window;
    Body *bodies;
    int number_of_bodies;

    struct options_t opts;
    get_opts(argc, argv, &opts);
    THETA = opts.threshold;
    dt = opts.timestep;
    steps = opts.steps;
    visuals = opts.visuals;

    if (rank == 0)
    {
        // print_opts(&opts);
        if (visuals)
        {
            /* OpenGL window dims */
            int width = 600, height = 600;
            if (!glfwInit())
            {
                fprintf(stderr, "Failed to initialize GLFW\n");
                return -1;
            }
            // Open a window and create its OpenGL context
            window = glfwCreateWindow(width, height, "Simulation", NULL, NULL);
            int ret = setup_window(window);
            if (ret == -1)
                return -1;
        }
        bodies = read_bodies_file(opts.in_file, number_of_bodies);
    }

    double start = MPI_Wtime();
    MPI_Bcast(&number_of_bodies, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0)
    {
        bodies = (Body *)malloc(number_of_bodies * sizeof(Body));
    }

    if (size == 1)
    {
        // Running Sequentially
        for (int s = 0; s < steps; s++)
        {
            auto root = create_quadtree(bodies, number_of_bodies);
            double mass= 0;
            for (int i = 0; i < number_of_bodies; i++){
                if (bodies[i].mass != -1){
                    mass += bodies[i].mass;
                }
            }
            if (!approx_equal(mass, root->total_mass)){
                // printf("Step %d Expected  Mass: %f Mass: %f\n",s, mass, root->total_mass);
                assert(false);
            }
            if (root->type != INVALID)
            {
                draw(window, root, bodies, number_of_bodies);
                for (int i = 0; i < number_of_bodies; i++)
                {
                    calculate_pos_vel_for_body(root, &bodies[i]);
                }
            }
            free_node(root);
        }
    }
    else
    {
        // Parallel

        for (int s = 0; s < steps; s++)
        {

            MPI_Bcast(bodies, number_of_bodies, MPI_Body_datatype, 0, MPI_COMM_WORLD);
            auto root = create_quadtree(bodies, number_of_bodies);
            // Bodies have now been recieved by each process
            if (root->type != INVALID)
            {
                int completed= 0;
                for (int i = rank; i < number_of_bodies; i += (size))
                {
                    calculate_pos_vel_for_body(root, &bodies[i]);
                    completed +=1;
                }


                if (rank != 0)
                {
                    for (int i = rank; i < number_of_bodies; i += (size))
                    {

                        MPI_Send(&bodies[i], 1, MPI_Body_datatype, 0, tagno, MPI_COMM_WORLD);
                    }

                }
                else
                { // rank == 0, Is main thread
                    Body temp;
                    // Handle Receiving bodies with new data, match with index to know where they belong
                    for (int i = 0; i < (number_of_bodies - completed); i++)
                    {
                        MPI_Recv(&temp, 1, MPI_Body_datatype, MPI_ANY_SOURCE, tagno, MPI_COMM_WORLD, &status);
                        bodies[temp.index].index = temp.index;
                        bodies[temp.index].x_pos = temp.x_pos;
                        bodies[temp.index].y_pos = temp.y_pos;
                        bodies[temp.index].mass = temp.mass;
                        bodies[temp.index].x_vel = temp.x_vel;
                        bodies[temp.index].y_vel = temp.y_vel;
                        bodies[temp.index].x_force = temp.x_force;
                        bodies[temp.index].y_force = temp.y_force;
                    }

                    draw(window, root, bodies, number_of_bodies);
                }
            }
            free_node(root);
        }
    }

    if (rank == 0)
    {
        printf("%f\n", MPI_Wtime() - start);
        // Output data here.
        write_file(opts.out_file, bodies, number_of_bodies);
        if (visuals)
        // Struct to send can contain this data.
        {
            while (!glfwWindowShouldClose(window))
            {
                /* Swap front and back buffers */
                /* Poll for and process events */
                glfwPollEvents();
            }
        }
    }

    free(bodies);
    MPI_Finalize();
}

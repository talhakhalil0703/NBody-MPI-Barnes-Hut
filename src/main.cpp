#include <iostream>
#include <cstring>
#include "io.h"
#include "argparse.h"
#include "body.h"
#include "quadtree.h"

// Visual Stuff could be moved?

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glu.h>
#include <GL/glut.h>

float THETA;
float dt;

using namespace std;
void draw_2d_particle(double x_window, double y_window, double radius, float *colors)
{
    int k = 0;
    float angle = 0.0f;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(colors[0], colors[1], colors[2]);
    glVertex2f(x_window, y_window);
    for (k = 0; k < 20; k++)
    {
        angle = (float)(k) / 19 * 2 * 3.141592;
        glVertex2f(x_window + radius * cos(angle),
                   y_window + radius * sin(angle));
    }
    glEnd();
}

void drawOctreeBounds2D(Node &node)
{
    float x_win = 2 * node.x_pos / XLIM - 1;
    float y_win = 2 * node.y_pos / YLIM - 1;
    float x_lim = node.x_lim * 2 / XLIM; // TODO:Not entirely sure if this is correct if lim change
    float y_lim = node.y_lim * 2 / YLIM;

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

void draw_lines(Node &node)
{
    if (node.internal)
    {
        drawOctreeBounds2D(node);
    }
    if (node.nw != nullptr)
        draw_lines(*node.nw);
    if (node.ne != nullptr)
        draw_lines(*node.ne);
    if (node.se != nullptr)
        draw_lines(*node.se);
    if (node.sw != nullptr)
        draw_lines(*node.sw);
}

void draw_bodies(vector<Body> &bodies)
{
    for (const auto &body : bodies)
    {
        double x_win, y_win = 0;
        x_win = 2 * body.x_pos / XLIM - 1;
        y_win = 2 * body.y_pos / YLIM - 1;
        // printf("id: %d, x_win: %f, y_win: %f\n", body.index, x_win, y_win);
        float colors[3] = {0.9, 0.9, 0.9};
        draw_2d_particle(x_win, y_win, 0.004, colors);
    }
}

int main(int argc, char **argv)
{
    /* OpenGL window dims */
    int width = 600, height = 600;
    GLFWwindow *window;
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    // Open a window and create its OpenGL context
    window = glfwCreateWindow(width, height, "Simulation", NULL, NULL);
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

    // Parse args
    struct options_t opts;
    get_opts(argc, argv, &opts);
    print_opts(&opts);
    THETA = opts.threshold;
    dt = opts.timestep;
    vector<Body> bodies = read_bodies_file(opts.in_file);

    // for (uint i = 0; i < bodies.size(); i++)
    // {
    //     print_body(bodies[i]);
    // }

    if (opts.visuals)
    {
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            auto root = create_quadtree(bodies);
            printf("Tree created\n");
            if (!root.empty)
            {
                calculate_forces_within_quadtree(root, bodies);
                printf("Forced Calculated\n");
                draw_bodies(bodies);
                draw_lines(root);
                glfwSwapBuffers(window);
            }
            /* Swap front and back buffers */
            /* Poll for and process events */
            glfwPollEvents();
        }
    }
}

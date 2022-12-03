#include <iostream>
#include <cstring>
#include "io.h"
#include "argparse.h"
#include "body.h"

// Visual Stuff could be moved?

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

void draw_2d_particle(double x_window, double y_window, double radius, float *colors) {
    int k = 0;
    float angle = 0.0f;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(colors[0], colors[1], colors[2]);
    glVertex2f(x_window, y_window);
    for(k=0;k<20;k++){
        angle=(float) (k)/19*2*3.141592;
        glVertex2f(x_window+radius*cos(angle),
           y_window+radius*sin(angle));
    }
    glEnd();
}

void draw_bodies(vector<body> &bodies){
    for (const auto& body: bodies){
        double x_win, y_win = 0;
        x_win = 2*body.x_pos/4-1;
        y_win = 2*body.y_pos/4-1;
        //printf("id: %d, x_win: %f, y_win: %f\n", body.index, x_win, y_win);
        float colors[3] = {0.5, 0.5, 0.5};
        draw_2d_particle(x_win, y_win, 0.02*body.mass, colors);
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
    vector<body> bodies = read_bodies_file(opts.in_file);
    for (uint i = 0; i < bodies.size(); i++)
    {
        print_body(bodies[i]);
    }

    if (opts.visuals)
    {
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClearColor(0.9, 0.1, 0.55f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            draw_bodies(bodies);
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }
    }
}

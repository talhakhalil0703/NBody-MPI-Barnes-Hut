#include "body.h"

void print_body(Body *b)
{
    printf("%d\t%f\t%f\t %f\t%f\t%f\t%f\t%f\n",
           b->index, b->x_pos, b->y_pos, b->mass, b->x_vel, b->y_vel, b->x_force, b->y_force);
}

// void print_body(Body *b)
// {
//     printf("index: %d, x_pos: %f, y_pos: %f, mass: %f, x_vel: %f, y_vel: %f, x_force: %f, y_force: %f\n",
//            b->index, b->x_pos, b->y_pos, b->mass, b->x_vel, b->y_vel, b->x_force, b->y_force);
// }

void update_pos_and_vel(Body *b)
{
    // ax = Fx/M0
    // ay = Fy/M0
    // Px' = Px + Vx*dt + 0.5*ax*dt^2
    // Py' = Py + Vy*dt + 0.5*ay*dt^2
    // Vx' = Vx + ax*dt
    // Vy' = Vy + ay*dt
    assert(b->mass != -1);
    double ax = -1*b->x_force / b->mass;
    double ay = -1*b->y_force / b->mass;
    b->x_pos = (b->x_pos) + (b->x_vel * dt) + (0.5 * ax * dt * dt);
    b->y_pos = (b->y_pos) + (b->y_vel * dt) + (0.5 * ay * dt * dt);
    b->x_vel = (b->x_vel) + (ax * dt);
    b->y_vel = (b->y_vel) + (ay * dt);

    b->x_force = 0;
    b->y_force = 0;
    if (b->x_pos < 0 || b->x_pos > XLIM || b->y_pos < 0 || b->y_pos > YLIM)
    {
        b->mass = -1;
        // printf("body index: %d out of bounds\n", b->index);
    }
}
#ifndef _BODY_H
#define _BODY_H
#include <iostream>
#include <cassert>

#define XLIM 4
#define YLIM 4
#define G 0.0001
#define rlimit 0.03

extern double THETA;
extern double dt;

struct Body
{
    int index = -1;
    double x_pos = 0;
    double y_pos = 0;
    double mass = -1;
    double x_vel = 0;
    double y_vel = 0;
    double x_force = 0;
    double y_force = 0;
};

void print_body(Body *b);
void update_pos_and_vel(Body *b);

#endif
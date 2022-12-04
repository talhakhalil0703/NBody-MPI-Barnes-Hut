#ifndef _BODY_H
#define _BODY_H
#include <iostream>

#define XLIM 4
#define YLIM 4
#define G 0.0001

extern float THETA;
extern float dt;

struct Body
{
	int index = 0;
	float x_pos = 0;
	float y_pos = 0;
	float mass = 0;
	float x_vel = 0;
	float y_vel = 0;
    float x_force = 0;
    float y_force = 0;
};

void print_body(Body &b);
void update_pos_and_vel(Body &b);

#endif
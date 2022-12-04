#ifndef _BODY_H
#define _BODY_H
#include <iostream>

#define XLIM 4
#define YLIM 4
#define G 0.0001
#define rlimit 0.03
#define dt 0.005

struct Body
{
	int index;
	float x_pos;
	float y_pos;
	float mass;
	float x_vel;
	float y_vel;
};

void print_body(Body &b);

#endif
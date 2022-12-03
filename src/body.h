#ifndef _BODY_H
#define _BODY_H
#include <iostream>

struct body
{
	int index;
	float x_pos;
	float y_pos;
	float mass;
	float x_vel;
	float y_vel;
};

void print_body(body &b);

#endif
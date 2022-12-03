#include "body.h"

void print_body(Body &b)
{
	printf("index: %d, x_pos: %f, y_pos: %f, mass: %f, x_vel: %f, y_vel: %f\n",
		   b.index, b.x_pos, b.y_pos, b.mass, b.x_vel, b.y_vel);
}
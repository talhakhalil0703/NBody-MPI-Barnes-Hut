#ifndef _IO_H
#define _IO_H

#include "argparse.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "body.h"

std::vector<Body> read_bodies_file(char *in_file);

void write_file(struct options_t*         args,
                struct prefix_sum_args_t* opts);

#endif

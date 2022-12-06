#ifndef _IO_H
#define _IO_H

#include "argparse.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "body.h"

Body * read_bodies_file(char *in_file, int &count);

void write_file(char *out_file, Body * bodies, int count);


#endif

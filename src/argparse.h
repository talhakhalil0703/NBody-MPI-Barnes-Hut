#ifndef _ARGPARSE_H
#define _ARGPARSE_H

#include <getopt.h>
#include <stdlib.h>
#include <iostream>

struct options_t
{
    char *in_file;
    char *out_file;
    int steps;
    float threshold;
    float timestep;
    bool visuals;
};

void get_opts(int argc, char **argv, struct options_t *opts);
void print_opts(struct options_t *opts);
#endif

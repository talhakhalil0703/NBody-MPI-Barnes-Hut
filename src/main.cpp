#include <iostream>
#include <cstring>
#include "io.h"
#include "argparse.h"
#include "body.h"

using namespace std;


int main(int argc, char **argv)
{
    // Parse args
    struct options_t opts;
    get_opts(argc, argv, &opts);
    print_opts(&opts);
    std::vector<body> bodies = read_bodies_file(opts.in_file);
    for (uint i =0; i < bodies.size(); i++){
        print_body(bodies[i]);
    }
}

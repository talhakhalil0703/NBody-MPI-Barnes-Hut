#include <iostream>
#include <cstring>
#include "io.h"
#include "argparse.h"

using namespace std;


int main(int argc, char **argv)
{
    // Parse args
    struct options_t opts;
    get_opts(argc, argv, &opts);
    print_opts(&opts);
}

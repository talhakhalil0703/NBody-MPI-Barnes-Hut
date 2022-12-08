#include "argparse.h"

void get_opts(int argc,
              char **argv,
              struct options_t *opts)
{
    if (argc == 1)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "\t--in or -i <file_path>" << std::endl;
        std::cout << "\t--out or -o <file_path>" << std::endl;
        std::cout << "\t--steps or -s <number of iterations as an int>" << std::endl;
        std::cout << "\t--theta or -t <threshold for MAC as a double>" << std::endl;
        std::cout << "\t--timestep or -d <timestep as a double>" << std::endl;
        std::cout << "\t[Optional] --visuals or -V" << std::endl;
        exit(0);
    }

    opts->visuals = false;

    struct option l_opts[] = {
        {"in", required_argument, NULL, 'i'},
        {"out", required_argument, NULL, 'o'},
        {"steps", required_argument, NULL, 's'},
        {"threshold", required_argument, NULL, 't'},
        {"timestep", required_argument, NULL, 'd'},
        {"visuals", optional_argument, NULL, 'V'}};

    int ind, c;
    while ((c = getopt_long(argc, argv, "i:o:s:t:d:V", l_opts, &ind)) != -1)
    {
        switch (c)
        {
        case 0:
            break;
        case 'i':
            opts->in_file = (char *)optarg;
            break;
        case 'o':
            opts->out_file = (char *)optarg;
            break;
        case 's':
            opts->steps = atoi((char *)optarg);
            break;
        case 't':
            opts->threshold = atof((char *)optarg);
            break;
        case 'd':
            opts->timestep = atof((char *)optarg);
            break;
        case 'V':
            opts->visuals = true;
            break;
        case ':':
            std::cerr << argv[0] << ": option -" << (char)optopt << "requires an argument." << std::endl;
            exit(1);
        }
    }
}

void print_opts(struct options_t *opts)
{
    std::cout << "Options: " << std::endl;
    std::cout << "\t in: " << opts->in_file << std::endl;
    std::cout << "\t out: " << opts->out_file << std::endl;
    std::cout << "\t steps: " << opts->steps << std::endl;
    std::cout << "\t threshold: " << opts->threshold << std::endl;
    std::cout << "\t timestep: " << opts->timestep << std::endl;
    if (opts->visuals == true)
        std::cout << "\t Visuals: Enabled" << std::endl;
    else
        std::cout << "\t Visuals: Disabled" << std::endl;
}
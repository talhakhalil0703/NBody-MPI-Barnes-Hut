#include "io.h"

Body * read_bodies_file(char *in_file, int & count)
{
    count = 0;
    // Open file
    std::ifstream in;
    in.open(in_file);
    // Get num vals
    in >> count;

    Body * bodies = (Body*) malloc(count*sizeof(Body));
    // Read bodies vals
    for (int i = 0; i < count; ++i)
    {
        in >> bodies[i].index;
        in >> bodies[i].x_pos;
        in >> bodies[i].y_pos;
        in >> bodies[i].mass;
        in >> bodies[i].x_vel;
        in >> bodies[i].y_vel;
    }

    return bodies;
}

void write_file(struct options_t *args,
                struct prefix_sum_args_t *opts)
{
    // Open file
    std::ofstream out;
    out.open(args->out_file, std::ofstream::trunc);

    out.flush();
    out.close();

    // Free memory
    // free(opts->output_vals);
}

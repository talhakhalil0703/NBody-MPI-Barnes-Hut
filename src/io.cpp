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
        bodies[i].x_force = 0;
        bodies[i].y_force = 0;
    }

    return bodies;
}

void write_file(char *out_file, Body * bodies, int count)
{
    // Open file
    std::ofstream out;
    out.open(out_file, std::ofstream::trunc);


    out << count << std::endl;
    out << std::scientific;

    for (int i =0; i < count; i++){
        out << bodies[i].index << "\t";
        out << bodies[i].x_pos << "\t";
        out << bodies[i].y_pos << "\t";
        out << bodies[i].mass << "\t";
        out << bodies[i].x_vel << "\t";
        out << bodies[i].y_vel << std::endl;
    }

    out.flush();
    out.close();

}

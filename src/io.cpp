#include "io.h"

std::vector<body> read_bodies_file(char *in_file)
{

	int number_of_bodies = 0;
	// Open file
	std::ifstream in;
	in.open(in_file);
	// Get num vals
	in >> number_of_bodies;

	std::vector<body> bodies;

	// Read bodies vals
	for (int i = 0; i < number_of_bodies; ++i)
	{
		body temp;
		in >> temp.index;
		in >> temp.x_pos;
		in >> temp.y_pos;
		in >> temp.mass;
		in >> temp.x_vel;
		in >> temp.y_vel;
		bodies.push_back(temp);
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

#include "body.h"
#include <vector>

struct Node {
    Body *body = nullptr;
    Node  *nw = nullptr;
    Node  *ne = nullptr;
    Node  *sw = nullptr;
    Node  *se = nullptr;
    bool internal = false;
    float x_lim=0;
    float y_lim=0;
    float x_pos=0;
    float y_pos=0;
    float center_of_mass_x=0;
    float center_of_mass_y=0;
    float total_mass=0;
};


void check_limit_and_insert(Node& node, Body& body);

void insert_into_external_node(Node& node, Body& body);

void insert_into_internal_node(Node& node, Body& body);

void insert_into_node(Node& node, Body& body);

Node create_quadtree(std::vector<Body> &bodies);
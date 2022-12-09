#include "body.h"
#include <vector>


enum NodeType {EXTERNAL, INTERNAL, INVALID};

struct Node
{
    Body *body = nullptr;
    Node *nw = nullptr;
    Node *ne = nullptr;
    Node *sw = nullptr;
    Node *se = nullptr;
    NodeType type = INVALID;
    double x_lim = 0;
    double y_lim = 0;
    double x_pos = 0;
    double y_pos = 0;
    double center_of_mass_x = 0;
    double center_of_mass_y = 0;
    double total_mass = 0;
};

struct node_mass {
    double mass;
    double x;
    double y;
};

void free_node(Node * node);

void _subdivide_and_insert(Node *node, Body *body);

void _insert_into_external_node(Node *node, Body *body);

void _insert_into_internal_node(Node *node, Body *body);

void _insert_into_node(Node *node, Body *body);

void _calculate_force_on_body(Node *node, Body *body);

Node* create_quadtree(Body * bodies, uint count);

void calculate_pos_vel_for_body(Node *root, Body *body);

void update_masses_for_nodes(Node *node);
node_mass _get_mass_of_node(Node * node);
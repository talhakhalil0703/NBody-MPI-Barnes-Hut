#include "quadtree.h"
#include <cmath>
#include <cassert>

Node *new_node()
{
    Node *node = new Node;
    node->body = nullptr;
    node->nw = nullptr;
    node->ne = nullptr;
    node->sw = nullptr;
    node->se = nullptr;
    node->type = INVALID;
    node->x_lim = 0;
    node->y_lim = 0;
    node->x_pos = 0;
    node->y_pos = 0;
    node->center_of_mass_x = 0;
    node->center_of_mass_y = 0;
    node->total_mass = 0;
    return node;
}

void free_node(Node *node)
{
    if (node->body != nullptr)
    {
        // Don't want to free body
    }
    if (node->nw != nullptr)
    {
        free_node(node->nw);
    }
    if (node->ne != nullptr)
    {
        free_node(node->ne);
    }
    if (node->sw != nullptr)
    {
        free_node(node->sw);
    }
    if (node->se != nullptr)
    {
        free_node(node->se);
    }
    free(node);
}

/// @brief Update the center of mass, and total mass of a node
/// @param node node to update
void _update_mass(Node *node)
{
    assert(node->type == INTERNAL);

    // Assumed that the children nodes masses are already updated.
    double total_mass = 0;
    double center_of_mass_x = 0;
    double center_of_mass_y = 0;

    if (node->ne != nullptr)
    {
        total_mass += node->ne->total_mass;
        center_of_mass_x += node->ne->center_of_mass_x * node->ne->total_mass;
        center_of_mass_y += node->ne->center_of_mass_y * node->ne->total_mass;
    }

    if (node->se != nullptr)
    {
        total_mass += node->se->total_mass;
        center_of_mass_x += node->se->center_of_mass_x * node->se->total_mass;
        center_of_mass_y += node->se->center_of_mass_y * node->se->total_mass;
    }

    if (node->sw != nullptr)
    {
        total_mass += node->sw->total_mass;
        center_of_mass_x += node->sw->center_of_mass_x * node->sw->total_mass;
        center_of_mass_y += node->sw->center_of_mass_y * node->sw->total_mass;
    }

    if (node->nw != nullptr)
    {
        total_mass += node->nw->total_mass;
        center_of_mass_x += node->nw->center_of_mass_x * node->nw->total_mass;
        center_of_mass_y += node->nw->center_of_mass_y * node->nw->total_mass;
    }

    node->total_mass = total_mass;
    node->center_of_mass_x = center_of_mass_x / total_mass;
    node->center_of_mass_y = center_of_mass_y / total_mass;
}

/// @brief Check the limits of where the body should be placed and if the quadrant does not exist
/// create it and insert it there, if it does send the body into that quadrant to be placed
/// @param node node
/// @param body body to insert
void _subdivide_and_insert(Node *node, Body *body)
{
    // This node will get subdivided here if need be.
    if (body->x_pos < node->x_pos)
    {
        // West Side
        if (body->y_pos < node->y_pos)
        {
            // South West Side
            if (node->sw == nullptr)
            {
                node->sw = new_node();
                node->sw->x_lim = node->x_lim / 2;
                node->sw->y_lim = node->y_lim / 2;
                node->sw->x_pos = node->x_pos - node->sw->x_lim;
                node->sw->y_pos = node->y_pos - node->sw->y_lim;
            }

            _insert_into_node(node->sw, body);
        }
        else
        {
            // North West Side
            //  subdivide
            if (node->nw == nullptr)
            {
                node->nw = new_node();
                node->nw->x_lim = node->x_lim / 2;
                node->nw->y_lim = node->y_lim / 2;
                node->nw->x_pos = node->x_pos - node->nw->x_lim;
                node->nw->y_pos = node->y_pos + node->nw->y_lim;
            }
            _insert_into_node(node->nw, body);
        }
    }
    else
    {
        // East Side
        if (body->y_pos < node->y_pos)
        {
            // South East Side
            if (node->se == nullptr)
            {
                node->se = new_node();
                node->se->x_lim = node->x_lim / 2;
                node->se->y_lim = node->y_lim / 2;
                node->se->x_pos = node->x_pos + node->se->x_lim;
                node->se->y_pos = node->y_pos - node->se->y_lim;
            }
            _insert_into_node(node->se, body);
        }
        else
        {
            // North East Side
            if (node->ne == nullptr)
            {
                node->ne = new_node();
                node->ne->x_lim = node->x_lim / 2;
                node->ne->y_lim = node->y_lim / 2;
                node->ne->x_pos = node->x_pos + node->ne->x_lim;
                node->ne->y_pos = node->y_pos + node->ne->y_lim;
            }
            _insert_into_node(node->ne, body);
        }
    }
}

/// @brief Insert a body into an external node, an external node is one that already contains a body
/// @param node node
/// @param body body to insert
void _insert_into_external_node(Node *node, Body *body)
{
    assert(node->type == EXTERNAL);
    node->type = INTERNAL;

    _subdivide_and_insert(node, node->body);
    _subdivide_and_insert(node, body);
    _update_mass(node);
}

/// @brief inserts a body into an internal node, an internal node is one that represents a collection of bodies
/// @param node node
/// @param body body to insert
void _insert_into_internal_node(Node *node, Body *body)
{
    assert(node->type == INTERNAL);

    // Update center of mass
    // Update total mass
    // Check what limit the body should be at nw, ne. se, sw
    _subdivide_and_insert(node, body);
    _update_mass(node);
}

/// @brief Insert body into a node
/// @param node node
/// @param body body to insert
void _insert_into_node(Node *node, Body *body)
{
    assert(body != nullptr);
    assert(body->mass != -1);
    assert(node != nullptr);

    if (node->type == INVALID)
    {
        node->body = body;
        node->total_mass = body->mass;
        node->center_of_mass_x = body->x_pos;
        node->center_of_mass_y = body->y_pos;
        node->type = EXTERNAL;
    }
    else if (node->type == INTERNAL)
    {
        _insert_into_internal_node(node, body);
        node->body = nullptr;
    }
    else if (node->type == EXTERNAL)
    {
        _insert_into_external_node(node, body);
        node->body = nullptr;
    }
}

/// @brief Given a vector of bodies create a quadtree
/// @param bodies vector of bodies
/// @return root Node of the quadtree
Node *create_quadtree(Body *bodies, uint count)
{
    Node *root = new_node();
    root->x_pos = XLIM / 2;
    root->y_pos = YLIM / 2;
    root->x_lim = XLIM;
    root->y_lim = YLIM;

    uint body_count = 0;
    for (uint i = 0; i < count; i++)
    {
        if (bodies[i].mass != -1)
        {
            _insert_into_node(root, &bodies[i]);
            // printf("i: %d, mass: %f\n", i,root->total_mass);
            body_count += 1;
        }
    }

    if (body_count == 1)
    {
        root->type = EXTERNAL;
    }
    else if (body_count > 1)
    {
        root->type = INTERNAL;
    }

    return root;
}

struct node_mass {
    double mass;
    double x;
    double y;
};

node_mass _get_mass_of_node(Node * node){
    node_mass ret;
    ret.mass = 0;
    ret.x = 0;
    ret.y = 0;

    // Go to each child and update the mass
    if (node->body != nullptr) {
        assert(node->type == EXTERNAL);
        ret.mass = node->body->mass;
        ret.x = node->body->x_pos;
        ret.y = node->body->y_pos;
        return ret;
    }

    assert(node->type == INTERNAL);

    if (node->nw != nullptr) {
        node_mass temp = _get_mass_of_node(node->nw);
        ret.mass += temp.mass;
        ret.x += temp.mass*temp.x;
        ret.y += temp.mass*temp.y;
    }

    if (node->ne != nullptr) {
        node_mass temp = _get_mass_of_node(node->ne);
        ret.mass += temp.mass;
        ret.x += temp.mass*temp.x;
        ret.y += temp.mass*temp.y;
    }

    if (node->sw != nullptr) {
        node_mass temp = _get_mass_of_node(node->sw);
        ret.mass += temp.mass;
        ret.x += temp.mass*temp.x;
        ret.y += temp.mass*temp.y;
    }

    if (node->se != nullptr) {
        node_mass temp = _get_mass_of_node(node->se);
        ret.mass += temp.mass;
        ret.x += temp.mass*temp.x;
        ret.y += temp.mass*temp.y;
    }

    assert(ret.mass !=0);
    ret.x = ret.x/ret.mass;
    ret.y = ret.y/ret.mass;
    return ret;

}

void _calculate_force_on_body(Node *node, Body *body)
{
    assert(body->mass != -1);
    if (node->body != nullptr && node->body->index == body->index)
    {
        // printf("body index: %d, node index: %d\n", body->index, node->body->index);
        return;
    }

    node_mass data = _get_mass_of_node(node);
    double dx = body->x_pos - data.x;
    double dy = body->y_pos - data.y;
    double d = sqrt((dx * dx) + (dy * dy));
    if (d < rlimit)
        d = rlimit;

    if (node->type == EXTERNAL)
    {
        // Calculate force of this node onto the body
        // This node is a singular body
        // Add this net force onto b's net force
        double d3 = d * d * d;
        body->x_force += (G * data.mass * body->mass * dx) / d3;
        body->y_force += (G * data.mass * body->mass * dy) / d3;
        // printf("body_index:%d EXTERNAL\n", body->index);
    }
    else if (node->type == INTERNAL)
    {
        assert(data.mass != 0);
        // s in this case can be thought of the xlim or ylim
        if ((node->x_lim/ d) < THETA) // TODO: this could be the case where this is not *2, I think its * 2 because x_lim describes how to draw the kids and we're getting it for this ndoe instead and not the kids
        {                                  // s/d < theta
            double d3 = d * d * d;         // Power of 3
            body->x_force += (G * data.mass * body->mass * dx) / d3;
            body->y_force += (G * data.mass * body->mass * dy) / d3;
            // printf("body_index:%d INTERNAL APROX\n", body->index);

        }
        else
        {
            // printf("body_index:%d INTERNAL NO APROX\n", body->index);

            // Recurse in each node for the body
            // recurse
            if (node->nw != nullptr)
                _calculate_force_on_body(node->nw, body);
            if (node->ne != nullptr)
                _calculate_force_on_body(node->ne, body);
            if (node->se != nullptr)
                _calculate_force_on_body(node->se, body);
            if (node->sw != nullptr)
                _calculate_force_on_body(node->sw, body);
        }
    }
    else
    {
        // printf("This node is Invalid!\n");
        // printf("It has: mass: %f center_of_mass %f, %f!\n", node->total_mass, node->center_of_mass_x, node->center_of_mass_y);
        assert(false);
    }
}

/// @brief Calculate the forces within a quadtree
/// @param node the root node, to use for force calculations
/// @param body Body for which to calculate updated pos vel
void calculate_pos_vel_for_body(Node *root, Body *body)
{
    if (body->mass == -1)
        return;
    // Only uses node data to calculate stuff for other bodies
    _calculate_force_on_body(root, body);

    // Update position and velocity for the body
    update_pos_and_vel(body);
}

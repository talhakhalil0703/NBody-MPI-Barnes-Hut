#include "quadtree.h"
#include <cmath>

/// @brief Update the center of mass, and total mass of a node
/// @param node node to update
void _update_mass(Node &node)
{
    if (node.internal != true || node.body != nullptr)
    {
        // Node is not an internal node and therefore does not need these values to be updated.
        return;
    }
    // Assumed that the children nodes masses are already updated.
    float total_mass = 0;
    float center_of_mass_x = 0;
    float center_of_mass_y = 0;

    if (node.ne != nullptr)
    {
        total_mass += node.ne->total_mass;
        center_of_mass_x += node.ne->x_pos * node.ne->total_mass;
        center_of_mass_y += node.ne->y_pos * node.ne->total_mass;
    }

    if (node.se != nullptr)
    {
        total_mass += node.se->total_mass;
        center_of_mass_x += node.se->x_pos * node.se->total_mass;
        center_of_mass_y += node.se->y_pos * node.se->total_mass;
    }

    if (node.sw != nullptr)
    {
        total_mass += node.sw->total_mass;
        center_of_mass_x += node.sw->x_pos * node.sw->total_mass;
        center_of_mass_y += node.sw->y_pos * node.sw->total_mass;
    }

    if (node.nw != nullptr)
    {
        total_mass += node.nw->total_mass;
        center_of_mass_x += node.nw->x_pos * node.nw->total_mass;
        center_of_mass_y += node.nw->y_pos * node.nw->total_mass;
    }

    node.total_mass = total_mass;
    node.center_of_mass_x = center_of_mass_x / total_mass;
    node.center_of_mass_y = center_of_mass_y / total_mass;
}

/// @brief Check the limits of where the body should be placed and if the quadrant does not exist
/// create it and insert it there, if it does send the body into that quadrant to be placed
/// @param node node
/// @param body body to insert
void _check_limit_and_insert(Node &node, Body &body)
{
    // Assume this node is already subdivided
    float x_min = node.x_pos;
    float y_min = node.y_pos;

    if (body.x_pos < x_min)
    {
        // West Side
        if (body.y_pos < y_min)
        {
            // South West Side
            if (node.sw == nullptr)
            {
                node.sw = new Node;
                node.sw->x_lim = node.x_lim / 2;
                node.sw->y_lim = node.y_lim / 2;
                node.sw->x_pos = node.x_pos - node.sw->x_lim;
                node.sw->y_pos = node.y_pos - node.sw->y_lim;
            }

            _insert_into_node(*node.sw, body);
        }
        else
        {
            // North West Side
            //  subdivide
            if (node.nw == nullptr)
            {
                node.nw = new Node;
                node.nw->x_lim = node.x_lim / 2;
                node.nw->y_lim = node.y_lim / 2;
                node.nw->x_pos = node.x_pos - node.nw->x_lim;
                node.nw->y_pos = node.y_pos + node.nw->y_lim;
            }
            _insert_into_node(*node.nw, body);
        }
    }
    else
    {
        // East Side
        if (body.y_pos < y_min)
        {
            // South East Side
            if (node.se == nullptr)
            {
                node.se = new Node;
                node.se->x_lim = node.x_lim / 2;
                node.se->y_lim = node.y_lim / 2;
                node.se->x_pos = node.x_pos + node.se->x_lim;
                node.se->y_pos = node.y_pos - node.se->y_lim;
            }
            _insert_into_node(*node.se, body);
        }
        else
        {
            // North East Side
            if (node.ne == nullptr)
            {
                node.ne = new Node;
                node.ne->x_lim = node.x_lim / 2;
                node.ne->y_lim = node.y_lim / 2;
                node.ne->x_pos = node.x_pos + node.ne->x_lim;
                node.ne->y_pos = node.y_pos + node.ne->y_lim;
            }
            _insert_into_node(*node.ne, body);
        }
    }
}

/// @brief Insert a body into an external node, an external node is one that already contains a body
/// @param node node
/// @param body body to insert
void _insert_into_external_node(Node &node, Body &body)
{
    Body *other_body = node.body;
    node.internal = true;
    node.body = nullptr;

    // Check what limit the body should be at and insert other body
    _check_limit_and_insert(node, *other_body);
    // Check what limit the body should be at and insert current body
    _check_limit_and_insert(node, body);
    _update_mass(node);
    // Update Center of Mass
    // Update total mass
}

/// @brief inserts a body into an internal node, an internal node is one that represents a collection of bodies
/// @param node node
/// @param body body to insert
void _insert_into_internal_node(Node &node, Body &body)
{
    // Update center of mass
    // Update total mass
    // Check what limit the body should be at nw, ne. se, sw
    _check_limit_and_insert(node, body);
    _update_mass(node);
}

/// @brief Insert body into a node
/// @param node node
/// @param body body to insert
void _insert_into_node(Node &node, Body &body)
{
    if (node.body == nullptr && node.internal == false)
    {
        node.body = &body;
        node.total_mass = body.mass;
        node.center_of_mass_x = body.x_pos;
        node.center_of_mass_y = body.y_pos;
    }
    else if (node.internal == true)
    {
        _insert_into_internal_node(node, body);
    }
    else
    {
        _insert_into_external_node(node, body);
    }
}

/// @brief Given a vector of bodies create a quadtree
/// @param bodies vector of bodies
/// @return root Node of the quadtree
Node create_quadtree(Body * bodies, uint count)
{
    Node root;
    root.x_pos = XLIM / 2;
    root.y_pos = YLIM / 2;
    root.x_lim = XLIM / 2;
    root.y_lim = YLIM / 2;

    bool has_bodies = false;
    for (uint i = 0; i < count; i++)
    {
        if (bodies[i].mass != -1)
        {
            _insert_into_node(root, bodies[i]);
            has_bodies = true;
        }
    }

    root.empty = !has_bodies;
    return root;
}

void _calculate_force_on_body(Node node, Body &body)
{
    if (node.internal == false && node.body->index != body.index)
    {
        float dx = body.x_pos - node.center_of_mass_x;
        float dy = body.y_pos - node.center_of_mass_y;
        float d = sqrt(dx * dx + dy * dy);
        if (d < THETA)
            d = THETA;
        // Calculate force of this node onto the body
        // This node is a singular body
        // Add this net force onto b's net force
        float d3 = pow(d, 3);
        body.x_force += G * node.total_mass * body.mass * (dx) / d3;
        body.y_force += G * node.total_mass * body.mass * (dy) / d3;
    }
    else
    {
        // Can we treat this node as a singular body
        // Check if s/d < theta if so treat as a singular node
        float dx = body.x_pos - node.center_of_mass_x;
        float dy = body.y_pos - node.center_of_mass_y;
        float d = sqrt(dx * dx + dy * dy);
        if (d < THETA)
            d = THETA;
        // s in this case can be thought of the xlim or ylim
        if (node.x_lim / d < THETA)
        {                         // s/d < theta
            float d3 = pow(d, 3); // Power of 3, SQRT 2
            body.x_force += G * node.total_mass * body.mass * (dx) / d3;
            body.y_force += G * node.total_mass * body.mass * (dy) / d3;
        }
        else
        {
            // Recurse in each node for the body
            // recurse
            if (node.nw != nullptr)
                _calculate_force_on_body(*node.nw, body);
            if (node.ne != nullptr)
                _calculate_force_on_body(*node.ne, body);
            if (node.se != nullptr)
                _calculate_force_on_body(*node.se, body);
            if (node.sw != nullptr)
                _calculate_force_on_body(*node.sw, body);
        }
    }
}

/// @brief Calculate the forces within a quadtree
/// @param node the root node, to use for force calculations
/// @param body Body for which to calculate updated pos vel
void calculate_pos_vel_for_body(Node root, Body &body)
{
    if (body.mass == -1) return;
    _calculate_force_on_body(root, body);
    // Update position and velocity for the body
    update_pos_and_vel(body);
}

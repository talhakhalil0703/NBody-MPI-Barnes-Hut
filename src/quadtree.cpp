#include "quadtree.h"

void check_limit_and_insert(Node& node, Body& body){
    //Assume this node is already subdivided
    float x_min = node.x_pos;
    float y_min = node.y_pos;

    //Scale bodies numbers by MAX
    float x_body = body.x_pos/XLIM;
    float y_body = body.y_pos/YLIM;

    if (x_body < x_min) {
        //West Side
        if (y_body < y_min){
            //South West Side
            if (node.sw == nullptr) {
                node.sw = new Node;
                node.sw->x_lim = node.x_lim/2;
                node.sw->y_lim = node.y_lim/2;
                node.sw->x_pos = node.x_pos - node.x_lim/2;
                node.sw->y_pos = node.y_pos - node.y_lim/2;
            }

            insert_into_node(*node.sw, body);
        } else {
            //North West Side
            // subdivide
            if (node.nw == nullptr) {
                node.nw = new Node;
                node.nw->x_lim = node.x_lim/2;
                node.nw->y_lim = node.y_lim/2;
                node.nw->x_pos = node.x_pos - node.x_lim/2;
                node.nw->y_pos = node.y_pos + node.y_lim/2;
            }
            insert_into_node(*node.nw, body);

        }
    } else {
        //East Side
        if (y_body < y_min){
            //South East Side
            if (node.se == nullptr) {
                node.se = new Node;
                node.se->x_lim = node.x_lim/2;
                node.se->y_lim = node.y_lim/2;
                node.se->x_pos = node.x_pos + node.x_lim/2;
                node.se->y_pos = node.y_pos - node.y_lim/2;
            }
            insert_into_node(*node.se, body);

        } else {
            //North East Side
            if (node.ne == nullptr) {
                node.ne = new Node;
                node.ne->x_lim = node.x_lim/2;
                node.ne->y_lim = node.y_lim/2;
                node.ne->x_pos = node.x_pos + node.x_lim/2;
                node.ne->y_pos = node.y_pos + node.y_lim/2;
            }
            insert_into_node(*node.ne, body);
        }
    }
}

void insert_into_external_node(Node& node, Body& body){
    Body* other_body = node.body;
    node.internal = true;
    node.body = nullptr;

    // Check what limit the body should be at and insert other body
    check_limit_and_insert(node, *other_body);
    // Check what limit the body should be at and insert current body
    check_limit_and_insert(node, body);

    // Update Center of Mass
    // Update total mass
}

void insert_into_internal_node(Node& node, Body& body){
    // Update center of mass
    // Update total mass
    // Check what limit the body should be at nw, ne. se, sw
    check_limit_and_insert(node, body);
    // Then try inserting
}

void insert_into_node(Node& node, Body& body){
    if (node.body == nullptr && node.internal == false){
        node.body = &body;
    } else if (node.internal == true){
        insert_into_internal_node(node, body);
    } else {
        insert_into_external_node(node, body);
    }
}

Node create_quadtree(std::vector<Body> &bodies){
    Node root;
    root.x_pos = 0.5;
    root.y_pos = 0.5;
    root.x_lim = 0.5;
    root.y_lim = 0.5;

    for (uint i =0; i < bodies.size(); i++){
        insert_into_node(root, bodies[i]);
    }

    return root;
}
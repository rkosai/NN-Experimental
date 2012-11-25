#include <stdio.h>
#include <stdlib.h>

#include "network.h"

#define DEBUG 1
#define MEMORY_DEBUG 1

//forward declarations from node.h
struct NodeList* new_nodelist(struct Node*);
void nodelist_append(struct NodeList*, struct NodeList*);

struct Network* new_network(void)
{
    // allocate memory for network
    struct Network *n = (struct Network *) malloc(sizeof(struct Network));
    _verify_allocation(n, "Network");

    // Initialize object to be empty
    n->layers = NULL;
    n->next_node_id = 0;
    n->training_data = NULL;

    return n;
}

void add_network_layer(struct Network *n, int node_count)
{
    // Create a new NodeLayer
    struct NodeLayer *layer = (struct NodeLayer*) malloc(
        sizeof(struct NodeLayer));
    _verify_allocation(layer, "NodeLayer");

    // Add the node_layer to the list of existing layers
    if (n->layers == NULL) {
        n->layers = layer;
    }
    else {
        struct NodeLayer *l = n->layers;
        while (l->next != NULL) {
            l = l->next;
        }
        l->next = layer;
    }

    // Now that we have the layer, add all the nodes
    int i;
    for (i = 0; i < node_count; i++) {
        struct Node *node = make_node(n);
        struct NodeList *nl = new_nodelist(node);
        nodelist_append(layer->nodes, nl);
    }
}

struct Node* make_node(struct Network *n)
{
    struct Node *node = new_node();
    node->id = n->next_node_id;
    n->next_node_id++;
    return node;
}


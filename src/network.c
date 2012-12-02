#include <stdio.h>
#include <stdlib.h>

#include "network.h"
#include "node.c"

#define DEBUG 1
#define MEMORY_DEBUG 1

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

    // initialize layer
    layer->nodes = NULL;
    layer->next = NULL;

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
        if (layer->nodes == NULL) {
            layer->nodes = nl;
        }
        else {
            nodelist_append(layer->nodes, nl);
        }
    }
}

void nodelist_append(struct NodeList* list, struct NodeList *new_node)
{
    while (list->next != NULL) {
        list = list->next;
    }
    list->next = new_node;
}

struct Node* make_node(struct Network *n)
{
    struct Node *node = new_node();
    node->id = n->next_node_id;
    n->next_node_id++;
    return node;
}

void print_network(struct Network *n)
{
    struct NodeLayer *current_layer = (struct NodeLayer*) n->layers;
    int i = 0;
    while (current_layer != NULL) {
        printf ("LAYER %d\n", i);

        // For each layer, print each node
        struct NodeList *current_list = current_layer->nodes;
        while (current_list != NULL) {
            struct Node* node = current_list->node;
            print_node(node);
            current_list = current_list->next;
        }

        // Increment to next layer
        i++;
        current_layer = current_layer->next;
    }
}

void solidify_network(struct Network *n)
{
    // connect each layer to the next layer

    // connect up the inputs
}


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
    if (n->layers != NULL) {
        // connect each layer to the next layer
        struct NodeLayer *this_layer = n->layers;
        while (this_layer->next != NULL) {
            struct NodeLayer *next_layer = this_layer->next;

            // connect each node in this layer to each in next layer
            struct NodeList *this_nodelist = this_layer->nodes;

            while (this_nodelist != NULL) {
                struct NodeList *next_nodelist = next_layer->nodes;
                while (next_nodelist != NULL) {
                    double random = (rand() % 100000) / 100000.0;
                    add_input_edge(
                        next_nodelist->node,
                        this_nodelist->node,
                        random);

                    add_output_edge(
                        this_nodelist->node,
                        next_nodelist->node);

                    next_nodelist = next_nodelist->next;
                }
                this_nodelist = this_nodelist->next;
            }

            this_layer = this_layer->next;
        }

        // connect up the inputs
        struct NodeLayer *first_layer = n->layers;
        struct NodeList *current_node = first_layer->nodes;
        while (current_node != NULL) {
            add_input_edge(current_node->node, NULL, 1);
            current_node = current_node->next;
        }
    }
}

void add_training_data(struct Network *n, struct ValueList *input,
                       struct ValueList *output)
{
    struct TrainingDataList *node = (struct TrainingDataList*) malloc(
        sizeof(struct TrainingDataList));
    _verify_allocation(node, "TrainingDataList");

    node->inputs = input;
    node->outputs = output;
    node->next = NULL;

    // Add this to the end of the list
    if (n->training_data == NULL) {
        n->training_data = node;
    }
    else {
        struct TrainingDataList *current_node = n->training_data;
        while (current_node->next != NULL) {
            current_node = current_node->next;
        }
        current_node->next = node;
    }
}

void print_training_data(struct Network *n)
{
    if (n->training_data) {
        struct TrainingDataList *node = n->training_data;

        while (node != NULL) {
            struct ValueList *input = node->inputs;
            struct ValueList *output = node->outputs;
            printf("[ ");
            // print inputs
            while (input != NULL) {
                printf("%.1f ", input->value);
                input = input->next;
            }
            printf("][ ");
            // print outputs
            while (output != NULL) {
                printf("%.1f ", output->value);
                output = output->next;
            }
            printf("]\n");
            node = node->next;
        }
    }
    else {
        printf("No training data.\n");
    }
}

void train_network(struct Network *n)
{
    return;
}

struct ValueList* execute_network(struct Network *n, struct ValueList *input)
{

    // populate input values of first layer
    // assumes one input value on the front layer, per node
    struct NodeLayer *layer = n->layers;
    struct NodeList *nl = layer->nodes;
    while (nl != NULL) {
        struct Node *node = nl->node;
        struct NodeWeightList *nwl = node->input_references;
        nwl->value = input->value;
        nl = nl->next;
        input = input->next;
    }

    // for each of the layers, execute each node and
    // assign value to next layer
    while (layer != NULL) {

        // TBD
        layer = layer->next;

        // On the last layer, store the value to results
        if (layer == NULL) {

        }
    }

    struct ValueList *node = (struct ValueList *) malloc(
        sizeof(struct ValueList));
    _verify_allocation(node, "ValueList");
    return node;
}


#include <stdio.h>
#include <stdlib.h>

#include "node.h"

#define DEBUG 1
#define MEMORY_DEBUG 1

struct Node* new_node(void)
{
    struct Node *node = (struct Node*) malloc(sizeof(struct Node));
    _verify_allocation(node, "Node");

    //Initialize node
    node->id = -1;
    node->input_references = NULL;
    node->output_references = NULL;
    node->error = 0;
    node->output = 0;
    node->delta = NULL;

    return node;
}

struct NodeList* new_nodelist(struct Node* node)
{
    struct NodeList *list = (struct NodeList*) malloc(sizeof(struct NodeList));
    _verify_allocation(list, "NodeList");

    // Initialize list
    list->node = node;
    list->next = NULL;
    return list;

}

void _verify_allocation(void *a, char* source)
{
    if (a == NULL) {
        printf("ERROR: Could not allocate new memory for %s.\n", source);
        exit(1);
    }
    else if (MEMORY_DEBUG) {
        printf("Allocated memory for %s to address %p\n", source, a);
    }
}

void print_node(struct Node *n)
{
    printf("     - Node %d\n", n->id);
}

void add_input_edge(struct Node *node, struct Node *input_node, double weight)
{
    // create NodeWeightList item
    struct NodeWeightList *nwl = (struct NodeWeightList*) malloc(
        sizeof(struct NodeWeightList));
    _verify_allocation(nwl, "NodeWeightList");
    nwl->node = input_node;
    nwl->weight = weight;
    nwl->next = NULL;

    // append it to the list
    if (node->input_references == NULL) {
        node->input_references = nwl;
    }
    else {
        struct NodeWeightList *l = node->input_references;
        while (l->next != NULL) {
            l = l->next;
        }
        l->next = nwl;
    }
}

void add_output_edge(struct Node *node, struct Node *output_node)
{
    // create NodeList item
    struct NodeList *nl = (struct NodeList *) malloc(sizeof(struct NodeList));
    _verify_allocation(nl, "NodeList");
    nl->node = output_node;
    nl->next = NULL;

    // append it to the list
    if (node->output_references == NULL) {
        node->output_references = nl;
    }
    else {
        struct NodeList *l = node->output_references;
        while (l->next != NULL) {
            l = l->next;
        }
        l->next = nl;
    }
}

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

void nodelist_append(struct NodeList* list, struct NodeList *new_node)
{
    // Add the node_layer to the list of existing layers
    if (list == NULL) {
        list = new_node;
    }
    else {
        struct NodeList *l = list;
        while (l->next != NULL) {
            l = l->next;
        }
        l->next = new_node;
    }
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


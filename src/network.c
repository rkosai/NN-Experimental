#include <stdio.h>
#include <stdlib.h>

#include "network.h"

#define MEMORY_DEBUG 1

struct Network* new_network(void)
{
    // allocate memory for network
    struct Network *n = (struct Network *) malloc(sizeof(struct Network));
    _verify_allocation(n);

    // Initialize object to be empty
    n->layers = NULL;
    n->next_node_id = 0;
    n->training_data = NULL;

    return n;
}

void _verify_allocation(void *a)
{
    if (a == NULL) {
        printf("ERROR: Could not allocate new memory.\n");
        exit(1);
    }
    else if (MEMORY_DEBUG) {
        printf("Allocated memory to address %p\n", a);
    }
}


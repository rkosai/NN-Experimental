#include "node.c"
#include "network.c"

int main() {
    // Generate the network
    struct Network *n = new_network();
    add_network_layer(n, 2);
    add_network_layer(n, 4);
    add_network_layer(n, 1);

    print_network(n);
    //solidify_network();


    // TBD: Train the network

    // TBD: Test the network

    return 0;
}

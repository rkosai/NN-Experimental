#include "node.c"
#include "network.c"

int main() {
    // Generate the network
    struct Network *network = new_network();
    add_network_layer(network, 2);
    add_network_layer(network, 4);
    add_network_layer(network, 1);
    //solidify_network();

    //print_network();

    // TBD: Train the network

    // TBD: Test the network

    return 0;
}

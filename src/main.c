#include "network.c"

struct ValueList* make_input(double, double);
struct ValueList* make_output(double);

int main() {
    // Generate the network
    struct Network *n = new_network();
    add_network_layer(n, 2);
    add_network_layer(n, 4);
    add_network_layer(n, 1);

    solidify_network(n);
    print_network(n);

    // Train the network
    add_training_data(n, make_input(1, 1), make_output(0));
    add_training_data(n, make_input(1, 0), make_output(1));
    add_training_data(n, make_input(0, 1), make_output(1));
    add_training_data(n, make_input(0, 0), make_output(0));
    train_network(n);

    // TBD: Test the network
    printf("1 xor 1 = %.3f\n", execute_network(n, make_input(1, 1))->value);
    printf("1 xor 0 = %.3f\n", execute_network(n, make_input(1, 0))->value);
    printf("0 xor 1 = %.3f\n", execute_network(n, make_input(0, 1))->value);
    printf("0 xor 0 = %.3f\n", execute_network(n, make_input(0, 0))->value);

    return 0;
}

struct ValueList* make_input(double input1, double input2)
{
    struct ValueList *node1 = (struct ValueList *) malloc(
        sizeof(struct ValueList));
    _verify_allocation(node1, "ValueList");

    struct ValueList *node2 = (struct ValueList *) malloc(
        sizeof(struct ValueList));
    _verify_allocation(node2, "ValueList");

    node2->value = input2;
    node2->next = NULL;

    node1->value = input1;
    node1->next = node2;

    return node1;
}

struct ValueList* make_output(double output)
{
    struct ValueList *node1 = (struct ValueList *) malloc(
        sizeof(struct ValueList));
    _verify_allocation(node1, "ValueList");
    node1->value = output;
    node1->next = NULL;

    return node1;
}

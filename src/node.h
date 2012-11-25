struct Node;

// List of nodes
struct NodeList {
    struct Node *node;
    struct NodeList *next;
};

// Weights for each node
struct NodeWeightList {
    struct Node *node;
    double weight;
    struct NodeWeightList *next;
};

// Input, output values
struct ValueList {
    double value;
    struct ValueList *next;
};

struct Node {
    int id;
    struct NodeWeightList *intput_references;
    struct NodeList *output_references;
    struct ValueList *input_values;
    double error;
    double output;
    struct NodeWeightList *delta;
};

// Public methods
struct Node* new_node(void);


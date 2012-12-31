struct Node;

struct NodeList {
    struct Node *node;
    struct NodeList *next;
};

struct NodeWeightList {
    struct Node *node;
    double weight;
    double value;
    struct NodeWeightList *next;
};

struct Node {
    int id;
    struct NodeWeightList *input_references;
    struct NodeList *output_references;
    double error;
    double output;
    struct NodeWeightList *delta;
};

// Public methods
struct Node* new_node(void);

struct NodeList* new_nodelist(struct Node*);

void _verify_allocation(void*, char*);

void print_node(struct Node*);

void execute_node(struct Node*);

// Private methods
void add_input_edge(struct Node*, struct Node*, double);

void add_output_edge(struct Node*, struct Node*);

float sigmoid(float);

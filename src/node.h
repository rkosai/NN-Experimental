struct Node;

struct NodeList {
    struct Node *node;
    struct NodeList *next;
};

struct NodeWeightList {
    struct Node *node;
    double weight;
    struct NodeWeightList *next;
};

struct ValueList {
    double value;
    struct ValueList *next;
};

struct Node {
    int id;
    struct NodeWeightList *input_references;
    struct NodeList *output_references;
    struct ValueList *input_values;
    double error;
    double output;
    struct NodeWeightList *delta;
};

// Public methods
struct Node* new_node(void);

struct NodeList* new_nodelist(struct Node*);

void nodelist_append(struct NodeList*, struct NodeList*);

void _verify_allocation(void*, char*);

struct TrainingDataList {
    struct ValueList *inputs;
    struct ValueList *outputs;
};

struct Network {
    struct NodeList *layers;
    int next_node_id;
    struct TrainingDataList *training_data;
};

// Public methods

struct Network* new_network(void);

void add_network_layer(struct Network *, int);

void solidify_network(struct Network*);

void print_network(struct Network*);

// Private methods

struct Node* make_node(struct Network*);

struct NodeList* net_layer_by_index(struct Network*, int);

void _verify_allocation(void*);

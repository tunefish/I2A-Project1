typedef struct vertex {
    void *data;                 // pointer to data represented by this vertex
    int visited;                // visited flag
    struct vertex *neighbors;   // pointer to first element of linked list of neighbors
    struct vertex *next;        // pointer to next vertex in vertex list
} vertex_t, *vertex_p;

typedef struct graph {
    int num_vertices;           // total number of vertices
    vertex_p vertices;          // pointer to first element of linked list of vertices
} graph_t, *graph_p;

typedef struct species {
    char *name;                 // name of species
} species_t, *species_p;

// graph functions.. add_vertex, add_edge, remove_vertex, remove_node
void reset_visited(graph_p graph);

#ifndef GRAPH_H
#define GRAPH_H

typedef struct vertex {
    void *data;                 // pointer to data represented by this vertex
    struct edge *neighbors;     // pointer to first element of linked list of neighbors
    struct vertex *next;        // pointer to next vertex in vertex list
    
    /** DFS traversal flags **/
    
    int visited;                // visited flag
    int depth;                  // depth of this vertex from the root
    struct vertex *parent;      // parent in DFS tree
} vertex_t, *vertex_p;

typedef struct edge {
    vertex_p to;                // end vertex of this edge
    struct edge *next;          // pointer to next edge in edge list
} edge_t, *edge_p;

typedef struct graph {
    int num_vertices;           // total number of vertices
    vertex_p vertices;          // pointer to first element of linked list of vertices
} graph_t, *graph_p;

typedef struct species {
    char *name;                 // name of species
} species_t, *species_p;

// graph functions.. add_vertex, add_edge, remove_vertex, remove_node
void reset(graph_p graph);

#endif
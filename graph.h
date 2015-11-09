#ifndef GRAPH_H
#define GRAPH_H

#define VERTEX_VISITED 1
#define VERTEX_RED 2
#define VERTEX_BLUE 4

typedef struct vertex {
    void *data;                 // pointer to data represented by this vertex
    struct edge *neighbors;     // pointer to first element of linked list of neighbors
    struct vertex *next;        // pointer to next vertex in vertex list
    
    /** BFS traversal flags **/
    
    int flag;                   // different flags: visited, blue, red
    int depth;                  // depth of this vertex from starting vertex of DFS / BFS search
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
    unsigned int id;            // ID of species, so far not used
    char name[];                // name of species
} species_t, *species_p;

graph_p create_graph();
void free_graph(graph_p graph);

vertex_p add_vertex(graph_p graph, void *data);
void *remove_vertex(graph_p graph, vertex_p v);

edge_p add_edge(vertex_p v1, vertex_p v2);
void remove_edge(vertex_p v1, vertex_p v2);

void reset(graph_p graph);

#endif
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

/*
 * Creates an empty graph
 */
graph_p create_graph() {
    graph_p graph = (graph_p) malloc(sizeof(graph_t));
    graph->num_vertices = 0;
    graph->vertices = NULL;
}

/*
 * Adds an vertex with data to a graph
 */
vertex_p add_vertex(graph_p graph, void *data, int depth, vertex_p parent) {
    vertex_p v = (vertex_p) malloc(sizeof(vertex_t));
    v->data = data;
    v->neighbors = NULL;
    v->next = graph->vertices;
    
    v->visited = 0;
    v->depth = depth;
    v->parent = parent;
    
    graph->vertices = v;
    return v;
}

/*
 * Removes an vertex from a graph and returns its data
 */
void *remove_vertex(graph_p graph, vertex_p v) {
    if (graph->vertices == v) {
        graph->vertices = v->next;
    }
    
    vertex_p w = graph->vertices;
    
    // remove all incoming edges
    while (w) {
        edge_p e = v->neighbors;
        while (e) {
            if (e->to == v) {
                // there's a vertex w -> v, remove it
                remove_edge(w, v);
            }
            e = e->next;
        }
        
        // if v is the following vertex in the list of all vertices, remove it from the list and skip it
        if (w->next == v) {
            w->next = v->next;
        }
        
        w = w->next;
    }
    
    // remove all outgoing edges
    while (v->neighbors) {
        remove_edge(v, v->neighbors->to);
    }
    
    void *data = v->data;
    free(v);
    return data;
}

/*
 * Adds an edge from v1 to v2 into a graph and returns the pointer to the edge
 */
edge_p add_edge(vertex_p v1, vertex_p v2) {
    edge_p e = v1->neighbors;
    while (e) {
        if (e->to == v2) {
            // edge already exists
            return;
        }
    }
    
    e = (edge_p) malloc(sizeof(edge_t));
    e->to = v2;
    
    e->next = v1->neighbors;
    v1->neighbors = e;
    return e;
}

/*
 * Removes an edge from v1 to v2 from a graph
 */
void remove_edge(vertex_p v1, vertex_p v2) {
    edge_p e = v1->neighbors;
    
    if (e && e->to == v2) {
        v1->neighbors = e->next;
        free(e);
    } else {    
        while (e) {
            if (e->next && e->next->to == v2) {
                edge_p f = e->next;
                e->next = f->next;
                free(f);
                return;
            }
            
            e = e->next;
        }
    }
}

/*
 * Resets all flags set in every vertex of a graph after BFS
 */
void reset(graph_p graph) {
    vertex_p v = graph->vertices;
    
    while (v) {
        v->visited = 0;
        v->depth = 0;
        v->parent = NULL;
        
        v = v->next;
    }
}

/*
 * Frees the memory allocated for the graph and the data represented by each vertex
 */
void free_graph(graph_p graph) {
    vertex_p v = graph->vertices;
    
    while (v) {
        // remove all edges of this vertex
        edge_p e = v->neighbors;
        while (e) {
            edge_p f = e->next;
            free(e);
            e = f;
        }
        
        // remove vertex and its data
        vertex_p w = v->next;
        free(v->data);
        free(v);
        v = w;
    }
    
    free(graph);
}
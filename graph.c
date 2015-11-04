#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

graph_p create_graph() {
    graph_p graph = (graph_p) malloc(sizeof(graph_t));
    graph->num_vertices = 0;
    graph->vertices = NULL;
}

vertex_p add_vertex(graph_p graph, void *data) {
    vertex_p v = (vertex_p) malloc(sizeof(vertex_t));
    v->data = data;
    v->neighbors = NULL;
    v->next = graph->vertices;
    
    v->visited = 0;
    v->depth = 0;
    v->parent = NULL;
    
    graph->vertices = v;
    return v;
}

void remove_vertex(graph_p graph, vertex_p v) {
    vertex_p w = graph->vertices;
    
    // remove all vertices going to v;
    while (w) {
        edge_p e = v->neighbors;
        while (e) {
            if (e->to == v) {
                // there's a vertex w -> v, remove it
                remove_edge(graph, w, v);
            }
            e = e->next;
        }
        
        // if v is the following vertex in the list of all vertices, remove it from the list and skip it
        if (w->next == v) {
            w->next = v->next;
        }
        
        w = w->next;
    }
    
    if (graph->vertices == v) {
        graph->vertices = v->next;
    }
    
    // remove all outgoing edges from v
    edge_p e = v->neighbors;
    while (e) {
        remove_edge(graph, v, e->to);
    }
    
    free(v);
}

edge_p add_edge(graph_p graph, vertex_p v1, vertex_p v2) {
    edge_p e = (edge_p) malloc(sizeof(edge_t));
    e->to = v2;
    
    e->next = v1->neighbors;
    v1->neighbors = e;
}

void remove_edge(graph_p graph, vertex_p v1, vertex_p v2) {
    
}

void reset(graph_p graph) {
    
}

void free_graph(graph_p graph) {
    while (graph->vertices) {
        remove_vertex(graph, graph->vertices);
    }
    
    free(graph);
}
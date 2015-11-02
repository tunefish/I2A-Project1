#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

vertex_p find_vertex_by_name(graph_p graph, char *sp);

queue_p num_children(graph_p graph, char *sp, int order, int num) {
    if (order < 1) {
        order = 1;
    }
    if (num < 1) {
        num = -1;
    }
    
    vertex_p v = find_vertex_by_name(graph, sp);
    int stack_size = 1;

    if (!v) {
        return NULL;
    }

    int children = 0;
    reset(graph);
    queue_p result = (queue_p) malloc(sizeof(queue_t));
    
    queue_p queue = create_queue();
    enqueue(queue, v);
    
    while (!is_empty(queue) && num != 0) {
        v = dequeue(queue);
        edge_p e = v->neighbors;
        
        do {
            enqueue(result, e);
            num--;
            
            if (v->depth < order - 1) {
                e->to->depth = v->depth + 1;
                enqueue(queue, e->to);
            }
        } while ((e = e->next) && num != 0);
    }

    return result;
}

char* most_diverse_subspecies(graph_p graph, char *sp) {
    vertex_p v = find_vertex_by_name(graph, sp);

    if (!v) {
        return NULL;
    }

    reset(graph);

    return "";
}

char* lowest_common_ancestor(graph_p graph, char *sp1, char *sp2) {
    vertex_p v1 = find_vertex_by_name(graph, sp1);
    vertex_p v2 = find_vertex_by_name(graph, sp2);

    if (!v1 || !v2) {
        return NULL;
    }

    reset(graph);

    return "";
}

vertex_p find_vertex_by_name(graph_p graph, char *sp) {
    vertex_p v = graph->vertices;

    while (v->next && strcmp(((species_p) v->data)->name, sp) != 0) {
        v = v->next;
    }

    if (strcmp(((species_p) v->data)->name, sp) != 0) {
        return NULL;
    }

    return v;
}
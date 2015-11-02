#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

vertex_p find_vertex_by_name(graph_p graph, char *sp);

queue_p num_children(graph_p graph, char *sp, int order, int num) {
    if (order < 1) {
        // error in input: get direct neighbors
        order = 1;
    }
    if (num < 1) {
        // all values less than one: get all matching species
        num = -1;
    }
    
    vertex_p v = find_vertex_by_name(graph, sp);

    if (!v) {
        // initial species not found
        return NULL;
    }

    // reset depth and visited flags for all vertices
    reset(graph);
    
    // abuse queue structure as a list of results
    queue_p result = (queue_p) malloc(sizeof(queue_t));
    
    queue_p queue = create_queue();
    enqueue(queue, v);
    
    // classic BFS:
    // repeat as long as there are more species to discover or we reached max wanted results (for all results: num < 0)
    while (!is_empty(queue) && num != 0) {
        v = dequeue(queue);
        edge_p e = v->neighbors;
        
        if (!e) {
            continue; // no neighbors to process
        }
        
        do {
            if (!e->to->visited) {
                enqueue(result, e);
                num--;
                
                if (v->depth < order - 1) {
                    e->to->depth = v->depth + 1;
                    enqueue(queue, e->to);
                }
            }
        } while ((e = e->next) && num != 0); // repeat for all neighbors until max number of results is reached
    }
    
    free_queue(queue);
    return result;
}

vertex_p most_diverse_subspecies(graph_p graph, char *sp) {
    vertex_p v = find_vertex_by_name(graph, sp);

    if (!v) {
        return NULL;
    }

    reset(graph);
    
    vertex_p result = NULL;
    int result_neighbors = -1;
    
    queue_p queue = create_queue();
    enqueue(queue, v);
    
    // classic BFS (DFS works as well):
    // look for species with the most subtypes
    while (!is_empty(queue)) {
        v = dequeue(queue);
        edge_p e = v->neighbors;
        
        if (!e) {
            continue; // no neighbors to process
        }
        
        int neighbors = 0;
        do {
            if (!e->to->visited) {
                enqueue(queue, e->to);
            }
            neighbors++;
        } while((e = e->next));
        
        if (neighbors > result_neighbors) {
            result = v;
            result_neighbors = neighbors;
        }
    }

    free_queue(queue);
    return result;
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

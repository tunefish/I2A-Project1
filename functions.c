#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "functions.h"

vertex_p find_vertex_by_name(graph_p graph, char *sp);
vertex_p lowest_common_ancestor_internal(graph_p graph, vertex_p v1, vertex_p v2, vertex_p vstart);

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
    v->depth = 0;
    
    // classic BFS:
    // repeat as long as there are more species to discover or we reached max wanted results (for all results: num < 0)
    while (!is_empty(queue) && num != 0) {
        v = dequeue(queue);
        edge_p e = v->neighbors;
        
        while (e && num != 0) { // repeat for all neighbors until max number of results is reached
            if (!e->to->visited) {
                e->to->visited = 1;
                enqueue(result, e);
                num--;
                
                if (v->depth < order - 1) {
                    e->to->depth = v->depth + 1;
                    enqueue(queue, e->to);
                }
            }
            
            e = e->next;
        }
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
        
        int neighbors = 0;
        while (e) {
            if (!e->to->visited) {
                e->to->visited = 1;
                enqueue(queue, e->to);
            }
            neighbors++;
            e = e->next;
        }
        
        if (neighbors > result_neighbors) {
            result = v;
            result_neighbors = neighbors;
        }
    }

    free_queue(queue);
    return result;
}

vertex_p lowest_common_ancestor(graph_p graph, char *sp1, char *sp2, char *start) {
    vertex_p v1 = find_vertex_by_name(graph, sp1);
    vertex_p v2 = find_vertex_by_name(graph, sp2);
    vertex_p vstart = find_vertex_by_name(graph, start);

    if (!v1 || !v2 || vstart) {
        return NULL;
    }

    reset(graph);
    
    edge_p e = vstart->neighbors;
    
    vertex_p result = NULL;
    int depth = INT_MAX;
    while (e) {
        if (!e->to->visited) {
            e->to->visited = 1;
            e->to->depth = vstart->depth + 1;
            vertex_p n = lowest_common_ancestor_internal(graph, v1, v2, e->to);
            
            if (n->depth < depth) {
                depth = n->depth;
                result = n;
            }
        }
    }

    return result;
}

vertex_p lowest_common_ancestor_internal(graph_p graph, vertex_p v1, vertex_p v2, vertex_p vstart) {
    edge_p e = vstart->neighbors;
    
    vertex_p result = NULL;
    int depth = INT_MAX;
    while (e) {
        if (e->to == v1)
        if (!e->to->visited) {
            e->to->visited = 1;
            e->to->depth = vstart->depth + 1;
            vertex_p n = lowest_common_ancestor_internal(graph, v1, v2, e->to);
            
            if (n->depth < depth) {
                depth = n->depth;
                result = n;
            }
        }
    }

    return result;
}

vertex_p find_vertex_by_name(graph_p graph, char *sp) {
    vertex_p v = graph->vertices;
    
    while (v) {
        if (strcmp(((species_p) v->data)->name, sp) != 0) {
            return v;
        }
        
        v = v->next;
    }

    return NULL;
}

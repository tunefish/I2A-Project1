#include <stdlib.h>

#include "functions.h"

queue_p num_children(graph_p graph, char *sp, int order, int num) {
    if (order < 1) {
        // error in input: get direct neighbors
        order = 1;
    }
    if (num < 1) {
        // all values less than one: get all matching species
        num = -1;
    }
    
    vertex_p v = find_species_by_name(graph, sp);

    if (!v) {
        // initial species not found
        return NULL;
    }

    // reset depth, parents and flags for all vertices
    reset(graph);
    
    // abuse queue structure as a list of results
    queue_p result = create_queue();
    
    queue_p queue = create_queue();
    enqueue(queue, v);
    v->depth = 0;
    
    edge_p e;
    
    // classic BFS:
    // repeat as long as there are more descendants to discover and we did not reach the max wanted results (for all results: num < 0)
    while (!is_empty(queue) && num != 0) {
        v = dequeue(queue);
        e = v->neighbors;
        
        // repeat for all neighbors until max number of results is reached
        while (e && num != 0) {
            if (!e->to->flag & VERTEX_VISITED) {
                e->to->flag |= VERTEX_VISITED;
                enqueue(result, e->to);
                num--;
                
                // only continue BFS if max depth is not exceeded
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
    vertex_p start = find_species_by_name(graph, sp);
    
    if (!start) {
        // initial species not found
        return NULL;
    }

    // reset depth, parents and flags for all vertices
    reset(graph);
    
    vertex_p result = NULL;
    int result_neighbors = -1;
    
    queue_p queue = create_queue();
    enqueue(queue, start);
    
    vertex_p v;
    edge_p e;
    int neighbors;
    
    // classic BFS (DFS works as well):
    // look among all descendants for the one with the most subtypes
    while (!is_empty(queue)) {
        v = dequeue(queue);
        e = v->neighbors;
        
        neighbors = 0;
        while (e) {
            if (!e->to->flag & VERTEX_VISITED) {
                e->to->flag |= VERTEX_VISITED;
                enqueue(queue, e->to);
            }
            neighbors++;
            e = e->next;
        }
        
        // if a more diverse species is found, replace previous result
        if (start != v && neighbors > result_neighbors) {
            result = v;
            result_neighbors = neighbors;
        }
    }

    free_queue(queue);
    return result;
}

vertex_p lowest_common_ancestor(graph_p graph, char *sp1, char *sp2, char *start) {
    vertex_p v1 = find_species_by_name(graph, sp1);
    vertex_p v2 = find_species_by_name(graph, sp2);
    vertex_p vstart = find_species_by_name(graph, start);

    if (!v1 || !v2 || !vstart) {
        // at least one of the initial species couldn't be found
        return NULL;
    }

    // reset depth, parents and flags for all vertices
    reset(graph);
    
    queue_p queue = create_queue();
    enqueue(queue, vstart);
    
    vertex_p v;
    edge_p e;
    int not_both_found = VERTEX_RED | VERTEX_BLUE;
    
    // classic BFS:
    // explore descendants until v1 and v2 have been found; save parent in DFS tree for each vertex
    while (!is_empty(queue) && not_both_found) {
        v = dequeue(queue);
        e = v->neighbors;
        
        while (e && not_both_found) {
            if (!e->to->flag & VERTEX_VISITED) {
                e->to->flag |= VERTEX_VISITED;
                e->to->parent = v;
                
                if (e->to == v1) {
                    not_both_found &= VERTEX_RED;
                } else if (e->to == v2) {
                    not_both_found &= VERTEX_BLUE;
                }
                enqueue(queue, e->to);
            }
            e = e->next;
        }
    }
    free_queue(queue);
    
    if (not_both_found) {
        return NULL;
    }
    
    // mark all parents of v1 as blue
    while (v1) {
        v1->flag |= VERTEX_BLUE;
        v1 = v1->parent;
    }
    
    // the first vertex marked as blue is the lowest common ancestor
    while (v2) {
        if (v2->flag & VERTEX_BLUE) {
            return v2;
        }
        v2 = v2->parent;
    }

    // THIS SHOULD NEVER HAPPEN
    return NULL;
}

/*
 * Find a species struct in a graph by its name
 */
vertex_p find_species_by_name(graph_p graph, char *sp) {
    vertex_p v = graph->vertices;
    
    while (v) {
        if (!strcmp(((species_p) v->data)->name, sp)) {
            return v;
        }
        
        v = v->next;
    }

    return NULL;
}

#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

vertex_p find_vertex_by_name(graph_p graph, char *sp);

int num_children(graph_p graph, char *sp, int order, int num, char** result) {
    if (order < 1) {
        order = 1;
    }
    if (num < 1) {
        num = -1;
    }
    
    vertex_p stack[order];
    stack[0] = find_vertex_by_name(graph, sp);
    int stack_size = 1;

    if (!stack[0]) {
        return 0;
    }

    int children = 0;
    reset_visited(graph);
    result_size = num > 0 ? num : 1;
    result = malloc(result_size * sizeof(char*));
    
    while (stack_size > 0 && num != 0) {
        vertex_p v = stack[--stack_size];
    }

    return children;
}

char* most_diverse_subspecies(graph_p graph, char *sp) {
    vertex_p v = find_vertex_by_name(graph, sp);

    if (!v) {
        return NULL;
    }

    reset_visited(graph);

    return "";
}

char* lowest_common_ancestor(graph_p graph, char *sp1, char *sp2) {
    vertex_p v1 = find_vertex_by_name(graph, sp1);
    vertex_p v2 = find_vertex_by_name(graph, sp2);

    if (!v1 || !v2) {
        return NULL;
    }

    reset_visited(graph);

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

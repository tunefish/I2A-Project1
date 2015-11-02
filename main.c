#include <stdio.h>
#include <stdlib.h>

#include "main.h"

int main(int argc, void *argv) {
    // TODO: load file specified in first parameter

    // TODO: parse and store graph in graph
    graph_p *graph = malloc(sizeof(graph_t));

    int exit = 0;
    char *command;
    while (!exit) {
        // read command from user

        if (strcmp(command, "exit")) {
            exit = 1;
        } else if (strcmp(command, "open")) {
            // read file and load into graph
        } else if (strcmp(command, "close")) {
            // free memory allocated for graph
        } else if (strcmp(command, "subtypes of")) {
            // extract arguments and execute num_childern(graph, name, order, num)
        } else if (strcmp(command, "most diverse subtype of")) {
            // extract arguments and execute most_diverse_subspecies(graph, name)
        } else if (strcmp(command, "lowest common ancestor of")) {
            // extrac arguments and execute lowest_common_ancestor(graph, name1, name2)
        }
    }

    // free memory allocated for graph

    return 0;
}

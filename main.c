#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "graph.h"
#include "functions.h"

int main(int argc, void *argv) {
    // TODO: load file specified in first parameter

    // TODO: parse and store graph in graph
    graph_p graph = malloc(sizeof(graph_t));

    int exit = 0;
    int file_loaded = 0;
    char *command;
    while (!exit) {
        printf(" > ");
        char *command = read_from_console();
        
        // remove newline characted at the end of the string
        if (*(command + strlen(command) - 1) == '\n') {
            *(command + strlen(command) - 1) = '\0';
        }
        
        if (strcmp(command, "exit") == 0) {
            // exit program
            exit = 1;
            printf("Exit requested..");
            
        } else if (starts_with(command, "open")) {
            // opens a file
            if (strlen(command) > 5) {
                // copy filename into file
                char file[strlen(command) - 4];
                memcpy(file, command+5, strlen(command) - 4);
                
                printf("Opening file %s\n", file);
                if (file_loaded) {
                    // TODO: close opened file
                }
                // TODO: read file
                file_loaded = 1;
            }
            
        } else if (strcmp(command, "close") == 0) {
            // close currently opened file
            if (file_loaded) {
                printf("Closing file..\n");
                // TODO: free memory allocated for graph
                file_loaded = 0;
            } else {
                printf("Error: no file opened\n");
            }
            
        } else if (starts_with(command, "cite")) {
            // cite <NR> subtypes of <species> of order <NR> command
            if (file_loaded) {
                char *sp;
                int order, num;
                sscanf(command, "cite %d subtypes of %s of order %d", num, sp, order);
                queue_p result = num_children(graph, sp, order, num);
                
                if (!result) {
                    printf("Species %s not found\n", sp);
                } else {                
                    while (!is_empty(result)) {
                        printf("%s\n", ((species_p) ((vertex_p) dequeue(result))->data)->name);
                    }
                    free_queue(result);
                }
            } else {
                printf("Error: no file opened\n");
            }
            
        } else if (starts_with(command, "most diverse subtype of")) {
            // most diverse subtype of <species> command
            if (file_loaded) {
                char *sp;
                sscanf(command, "most diverse subtype of %s", sp);
                vertex_p result = most_diverse_subspecies(graph, sp);
                
                if (!result) {
                    printf("Species %s not found or it has no subspecies\n", sp);
                } else {
                    printf("%s\n", ((species_p) result->data)->name);
                }
            } else {
                printf("Error: no file opened\n");
            }
            
        } else if (starts_with(command, "lowest common ancestor of")) {
            // lowest common ancestor of <species> and <species>
            if (file_loaded) {
                char *sp1, *sp2;
                sscanf(command, "lowest common ancestor of %s and %s");
                lowest_common_ancestor(graph, sp1, sp2);
            } else {
                printf("Error: no file opened\n");
            }
        }
    }

    // TODO: free memory
    free(graph);

    return 0;
}

/*
 * Checks if pre is a prefix of str
 */
int starts_with(char *str, char *pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

/*
 * Reads a line from console
 */
char *read_from_console(void) {
    char *line = malloc(100), *linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if (line == NULL) {
        return NULL;
    }

    for (;;) {
        c = fgetc(stdin);
        if(c == EOF) {
            break;
        }

        if (--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if (linen == NULL) {
                free(linep);
                return NULL;
            }
            
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n') {
            break;
        }
    }

    *line = '\0';
    return linep;
}
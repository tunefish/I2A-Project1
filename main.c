#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

#include "main.h"
#include "graph.h"
#include "functions.h"

int main(int argc, void *argv) {
    graph_p graph = NULL;

    int exit = 0;
    char *command;
    while (!exit) {
        printf(" > ");
        char *command = read_line(stdin);
        
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
			//where is the file opened??
            if (strlen(command) > 5) {
                // copy filename into file
                char *file = (char*) malloc(strlen(command) - 4);
                memcpy(file, command+5, strlen(command) - 4);
                
                printf("Opening file %s\n", file);
                if (graph) {
                    free_graph(graph);
                    graph = NULL;
                }
                
                graph = create_graph();
                
                free(file);
            }
            
        } else if (strcmp(command, "close") == 0) {
            // close currently opened file
            if (graph) {
                printf("Closing file..\n");
                free_graph(graph);
                graph = NULL;
            } else {
                printf("Error: no file opened\n");
            }
            
        } else if (starts_with(command, "cite")) {
            // cite <NR> subtypes of <species> of order <NR> command
            if (graph) {
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
            if (graph) {
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
            if (graph) {
                char *sp1, *sp2, *start;
                sscanf(command, "lowest common ancestor of %s and %s starting from %s", sp1, sp2, start);
                vertex_p result = lowest_common_ancestor(graph, sp1, sp2, start);
                
                if (!result) {
                    printf("Couldn't find %s, %s or %s or %s and %s don't share a common ancestor which is a subspecies of %s\n", sp1, sp2, start, sp1, sp2, start);
                } else {
                    printf("%s\n", ((species_p) result->data)->name);
                }
            } else {
                printf("Error: no file opened\n");
            }
        }
    }

    if (graph) {
        free_graph(graph);
    }

    return 0;
}

/*
 * Checks if pre is a prefix of str
 */
int starts_with(char *str, char *pre) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

/*
 * Reads a line
 */
char *read_line(FILE *ptr) {
    char *line = malloc(128), *linep = line;
    size_t lenmax = 128, len = lenmax;
    int c;

    if (line == NULL) {
        return NULL;
    }

    for (;;) {
        c = fgetc(ptr);
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "functions.h"

int starts_with(char *str, char *pre);
char *read_line(FILE *ptr);
graph_p parse_file(char *file);
void sanitize_name(char* str);

int main(int argc, void *argv) {
    graph_p graph = NULL;

    int exit = 0;
    char *command;
    while (!exit) {
        printf(" > ");
        char *command = read_line(stdin);
        
        if (strcmp(command, "exit") == 0) {
            // exit program
            exit = 1;
            printf("Exit requested..");
            
        } else if (starts_with(command, "open")) {
            // opens a file
            if (strlen(command) > 5) {
                // copy filename into file
                char *file = (char*) malloc(strlen(command) - 4);
                memcpy(file, command+5, strlen(command) - 4);
                
                printf("Opening file %s\n", file);
                if (graph) {
                    free_graph(graph);
                    graph = NULL;
                }

				graph = parse_file(file);
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
                char *sp = (char *) malloc(strlen(command) - 30);
                int order, num;
                sscanf(command, "cite %d subtypes of %s of order %d", &num, sp, &order);
                queue_p result = num_children(graph, sp, order, num);
                
                if (!result) {
                    printf("Species %s not found\n", sp);
                } else {
                    while (!is_empty(result)) {
                        printf("%s\n", ((species_p) ((vertex_p) dequeue(result))->data)->name);
                    }
                    free_queue(result);
                }
                
                free(sp);
            } else {
                printf("Error: no file opened\n");
            }
            
        } else if (starts_with(command, "most diverse subtype of")) {
            // most diverse subtype of <species> command
            if (graph) {
                char *sp = (char*) malloc(strlen(command) - 23);
                memcpy(sp, command+24, strlen(command) - 23);
                vertex_p result = most_diverse_subspecies(graph, sp);
                
                if (!result) {
                    printf("Species %s not found or it has no subspecies\n", sp);
                } else {
                    printf("%s\n", ((species_p) result->data)->name);
                }
                
                free(sp);
            } else {
                printf("Error: no file opened\n");
            }
            
        } else if (starts_with(command, "lowest common ancestor of")) {
            // lowest common ancestor of <species> and <species> starting from <species> command
            if (graph) {
                char *sp1 = (char *) malloc(strlen(command) - 26);
                char *sp2 = (char *) malloc(strlen(command) - 31);
                char *start = (char *) malloc(strlen(command) - 46);
                
                sscanf(command, "lowest common ancestor of %s and %s starting from %s", sp1, sp2, start);
                vertex_p result = lowest_common_ancestor(graph, sp1, sp2, start);
                
                if (!result) {
                    printf("Couldn't find %s, %s or %s or %s and %s don't share a common ancestor which is a subspecies of %s\n", sp1, sp2, start, sp1, sp2, start);
                } else {
                    printf("%s\n", ((species_p) result->data)->name);
                }
                
                free(sp1);
                free(sp2);
                free(start);
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
 * Parses a file into the graph data structure
 */
graph_p parse_file(char *file) {
    graph_p graph = NULL;
    FILE * fr;
    
    // pointer to line in file
    char * line;
    
    // char pointer used while parsing with strtok
    char * name;

    fr = fopen(file, "r");		//opens file
    if (!fr) {
        printf("Error: file %s not found\n", file);
    } else {
        graph = create_graph();
        while ((line = read_line(fr))) {
            // split parent species from child species
            name = strtok(line, ":");
            
            // ignore empty lines // lines without ':'
            if (!name) {
                continue;
            }
            sanitize_name(name);
            
            // ignore empty names
            if (*name == '\0' || strlen(name) == 0) {
                continue;
            }
            
            vertex_p v_parent = find_species_by_name(graph, name);

			if (!v_parent) {
                species_p parent = (species_p) malloc(sizeof(species_t) + strlen(name) + 1);
                memcpy(parent->name, name, strlen(name) + 1);
                v_parent = add_vertex(graph, parent);
            }

            // get all children
            name = strtok(NULL, ":");
            
            name = strtok(name, ",");
            while (name != NULL) {
                sanitize_name(name);
                
                // ignore empty names
                if (*name == '\0' || strlen(name) == 0) {
                    name = strtok(NULL, ",");
                    continue;
                }
                
                // species_t contains a string of dynamic length => add length of string to size
                vertex_p v = find_species_by_name(graph, name);
                if (!v) {
                    species_p sp = (species_p) malloc(sizeof(species_t) + strlen(name) + 1);
                    memcpy(sp->name, name, strlen(name) + 1);
                    v = add_vertex(graph, sp);
                }
                add_edge(v_parent, v);
                
                // read next child's name
                name = strtok(NULL, ",");
            }
        }

        fclose(fr);
    }
    
    return graph;
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
        
        // end of string / file => return string
        if(c == EOF) {
            break;
        }

        // string buffer full => double the size
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

        // end of line => return string
        if((*line++ = c) == '\n') {
            break;
        }
    }

    *line = '\0';
    
    // nothing read => return NULL
    if (strlen(linep) == 0) {
        return NULL;
    }
    
    // remove newline characted at the end of the string
    if (*(line - 1) == '\n') {
        *(line - 1) = '\0';
    }
    
    return linep;
}

/*
 * Removes all whitespace caracters from a string
 */
void sanitize_name(char *str) {
    int i = 0;
    int j = 0;
    while (i < strlen (str)) {
        if (isalpha(str[i])) {
            str[j] = tolower(str[i]);
            i++;
            j++;
        } else {
            i++;
        }
    }
    
    str[j] = '\0';
}
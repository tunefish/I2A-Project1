#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERTEX_VISITED 1
#define VERTEX_RED 2
#define VERTEX_BLUE 4

typedef struct queue_element {
    void *data;
    struct queue_element *next;
} queue_element_t, *queue_element_p;

typedef struct queue {
    queue_element_p first;
    queue_element_p last;
} queue_t, *queue_p;

typedef struct vertex {
    void *data;                 // pointer to data represented by this vertex
    struct edge *neighbors;     // pointer to first element of linked list of neighbors
    struct vertex *next;        // pointer to next vertex in vertex list
    
    /** BFS traversal flags **/
    
    int flag;                   // different flags: visited, blue, red
    int depth;                  // depth of this vertex from starting vertex of DFS / BFS search
    struct vertex *parent;      // parent in DFS tree
} vertex_t, *vertex_p;

typedef struct edge {
    vertex_p to;                // end vertex of this edge
    struct edge *next;          // pointer to next edge in edge list
} edge_t, *edge_p;

typedef struct graph {
    int num_vertices;           // total number of vertices
    vertex_p vertices;          // pointer to first element of linked list of vertices
} graph_t, *graph_p;

typedef struct species {
    unsigned int id;            // ID of species, so far not used
    char name[];                // name of species
} species_t, *species_p;

queue_p create_queue();
void free_queue(queue_p q);
void enqueue(queue_p q, void *d);
void *dequeue(queue_p q);
int is_empty(queue_p q);

graph_p create_graph();
void free_graph(graph_p graph);
vertex_p add_vertex(graph_p graph, void *data);
void *remove_vertex(graph_p graph, vertex_p v);
edge_p add_edge(vertex_p v1, vertex_p v2);
void remove_edge(vertex_p v1, vertex_p v2);
void reset(graph_p graph);

vertex_p find_species_by_name(graph_p graph, char *sp);
queue_p num_children(graph_p graph, char *sp, int order, int num);
vertex_p most_diverse_subspecies(graph_p graph, char *sp);
vertex_p lowest_common_ancestor(graph_p graph, char *sp1, char *sp2, char *start);

int starts_with(char *str, char *pre);
char *read_line(FILE *ptr);
graph_p parse_file(char *file);
void sanitize_name(char* str);

queue_p create_queue() {
    queue_p q= (queue_p) malloc(sizeof(queue_t));
    q->first = NULL;
    q->last = NULL;
    return q;
}

void free_queue(queue_p q) {
    while (q->first) {
        dequeue(q);
    }
    
    free(q);
}

void enqueue(queue_p q, void *d) {
    queue_element_p e = (queue_element_p) malloc(sizeof(queue_element_t));
    e->data = d;
    e->next = NULL;
    
    if (!q->first) {
        q->first = e;
    } else {
        q->last->next = e;
    }
    q->last = e;
}

void *dequeue(queue_p q) {
    void *res = q->first->data;
    queue_element_p f = q->first;
    
    q->first = f->next;
    if (!f->next) {
        q->last = NULL;
    }
    
    free(f);
    
    return res;
}

int is_empty(queue_p q) {
    return q->first == NULL;
}

/*
 * Creates an empty graph
 */
graph_p create_graph() {
    graph_p graph = (graph_p) malloc(sizeof(graph_t));
    graph->num_vertices = 0;
    graph->vertices = NULL;
}

/*
 * Adds an vertex with data to a graph
 */
vertex_p add_vertex(graph_p graph, void *data) {
    vertex_p v = (vertex_p) malloc(sizeof(vertex_t));
    v->data = data;
    v->neighbors = NULL;
    v->next = graph->vertices;
    
    v->flag = 0;
    v->depth = 0;
    v->parent = NULL;
    
    graph->vertices = v;
    return v;
}

/*
 * Removes an vertex from a graph and returns its data
 */
void *remove_vertex(graph_p graph, vertex_p v) {
    if (graph->vertices == v) {
        graph->vertices = v->next;
    }
    
    vertex_p w = graph->vertices;
    
    // remove all incoming edges
    while (w) {
        edge_p e = v->neighbors;
        while (e) {
            if (e->to == v) {
                // there's a vertex w -> v, remove it
                remove_edge(w, v);
            }
            e = e->next;
        }
        
        // if v is the following vertex in the list of all vertices, remove it from the list and skip it
        if (w->next == v) {
            w->next = v->next;
        }
        
        w = w->next;
    }
    
    // remove all outgoing edges
    while (v->neighbors) {
        remove_edge(v, v->neighbors->to);
    }
    
    void *data = v->data;
    free(v);
    return data;
}

/*
 * Adds an edge from v1 to v2 into a graph and returns the pointer to the edge
 */
edge_p add_edge(vertex_p v1, vertex_p v2) {
    edge_p e = v1->neighbors;
    while (e) {
        if (e->to == v2) {
            // edge already exists
            return;
        }
        
        e = e->next;
    }
    
    e = (edge_p) malloc(sizeof(edge_t));
    e->to = v2;
    
    e->next = v1->neighbors;
    v1->neighbors = e;
    return e;
}

/*
 * Removes an edge from v1 to v2 from a graph
 */
void remove_edge(vertex_p v1, vertex_p v2) {
    edge_p e = v1->neighbors;
    
    if (e && e->to == v2) {
        v1->neighbors = e->next;
        free(e);
    } else {    
        while (e) {
            if (e->next && e->next->to == v2) {
                edge_p f = e->next;
                e->next = f->next;
                free(f);
                return;
            }
            
            e = e->next;
        }
    }
}

/*
 * Resets all flags set in every vertex of a graph after BFS
 */
void reset(graph_p graph) {
    vertex_p v = graph->vertices;
    
    while (v) {
        v->flag = 0;
        v->depth = 0;
        v->parent = NULL;
        
        v = v->next;
    }
}

/*
 * Frees the memory allocated for the graph and the data represented by each vertex
 */
void free_graph(graph_p graph) {
    vertex_p v = graph->vertices;
    
    while (v) {
        // remove all edges of this vertex
        edge_p e = v->neighbors;
        while (e) {
            edge_p f = e->next;
            free(e);
            e = f;
        }
        
        // remove vertex and its data
        vertex_p w = v->next;
        free(v->data);
        free(v);
        v = w;
    }
    
    free(graph);
}

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
    
    /*num_children_state_p state = (num_children_state_p) malloc(sizeof(num_children_state_t));
    state->num = num;
    state->order = order;
    state->results = create_queue();
    dfs(graph, v, state, NULL, &num_children_h_neighbor);*/

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
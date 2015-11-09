#include "graph.h"
#include "queue.h"

vertex_p find_species_by_name(graph_p graph, char *sp);
queue_p num_children(graph_p graph, char *sp, int order, int num);
vertex_p most_diverse_subspecies(graph_p graph, char *sp);
vertex_p lowest_common_ancestor(graph_p graph, char *sp1, char *sp2, char *start);

#include "graph.h"
#include "queue.h"

queue_p num_children(graph_p graph, char *sp, int order, int num);
char* most_diverse_subspecies(graph_p graph, char *sp);
char* lowest_common_ancestor(graph_p graph, char *sp1, char *sp2);
#include <stdlib.h>

#include "stack.h"

stack_p create_stack() {
    stack_p s = (stack_p) malloc(sizeof(stack_t));
    s->first = NULL;
    return s;
}

void free_stack(stack_p s) {
    while (s->first) {
        pop(s);
    }
    
    free(s);
}

void push(stack_p s, void *d) {
    stack_element_p e = (stack_element_p) malloc(sizeof(stack_element_t));
    e->data = d;
    e->next = s->first;
    s->first = e;
}

void *pop(stack_p s) {
    void *res = s->first->data;
    stack_element_p new_first = s->first->next;
    free(s->first);
    s->first = new_first;
    
    return res;
}

int is_empty(stack_p s) {
    return s->first == NULL;
}
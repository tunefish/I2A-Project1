#include <stdlib.h>

#include "queue.h"

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

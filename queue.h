typedef struct queue_element {
    void *data;
    struct queue_element *next;
} queue_element_t, *queue_element_p;

typedef struct queue {
    queue_element_p first;
    queue_element_p last;
} queue_t, *queue_p;

queue_p create_queue();
void free_queue(queue_p q);
void enqueue(queue_p q, void *d);
void *dequeue(queue_p q);
int is_empty(queue_p q);

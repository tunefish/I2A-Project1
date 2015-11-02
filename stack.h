typedef struct stack_element {
    void *data;
    struct stack_element *next;
} stack_element_t, *stack_element_p;

typedef struct stack {
    stack_element_p first;
} stack_t, *stack_p;

stack_p create_stack();
void free_stack(stack_p s);
void push(stack_p s, void *d);
void *pop(stack_p s);
int is_empty(stack_p s);
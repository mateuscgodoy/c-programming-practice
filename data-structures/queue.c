#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
    struct node *__previous;
    struct node *__next;
    char *value;
    void (*del)(struct node *self);
    void (*display)(struct node *self);
};

struct Queue
{
    struct node *__head;
    struct node *__tail;
    int __size;

    void (*enqueue)(struct Queue *self, char *value);
    char *(*dequeue)(struct Queue *self);
    void (*display)(struct Queue *self);
    void (*size)(struct Queue *size);
    void (*del)(struct Queue *self);
};

/* NODE FUNCTIONS */

void __node_del(struct node *self)
{
    free((void *)self->value);
    free((void *)self);
}

void __node_display(struct node *self)
{
    if (!self->value)
    {
        printf("Empty");
        return;
    }

    printf("Value: %s", self->value);
}

/* QUEUE FUNCTIONS */
void __Queue_enqueue(struct Queue *self, char *value)
{
    if (value == NULL)
    {
        printf("Error: please provide a value to enqueue.\n");
        return;
    }

    struct node *node = node_new(value);

    if (self->__size == 0)
    {
        self->__head = node;
    }
    if (self->__size == 1)
    {
        self->__head->__next = node;
    }

    node->__previous = self->__tail;
    self->__tail = node;
    self->__size++;
}

char *__Queue_dequeue(struct Queue *self)
{
    if (self->__size == 0)
    {
        printf("Error: the queue is empty. Operation aborted.\n");
        return NULL;
    }

    struct node *temp;
    char *value = (char *)malloc(strlen(self->__head->value) + 1);
    strcpy(value, self->__head->value);

    temp = self->__head->__previous;
    self->__head->del(self->__head);
    self->__head = temp;

    return value;
}

int __Queue_size(struct Queue *self)
{
    return self->__size;
}

void __Queue_display(struct Queue *self)
{
    struct node *node;
    int i = 1;

    node = self->__head;
    while (node)
    {
        printf("Position in Queue: %dth - ", i);
        node->display(node);
        printf("\n");
        node = node->__next;
        i++;
    }
}

void __Queue_del(struct Queue *self)
{
    struct node *node, *next;
    node = self->__head;
    while (node)
    {
        next = node->__next;
        node->del(node);
        node = next;
    }
    free((void *)self);
}

/* Public function that creates a new node.
If a value is provided it will already populate the node.
*/
struct node *node_new(char *value)
{
    struct node *node = (struct node *)malloc(sizeof(struct node));
    node->__next = NULL;
    node->__previous = NULL;
    if (value != NULL)
    {
        node->value = (char *)malloc(strlen(value) + 1);
        strcpy(node->value, value);
    }
    else
    {
        node->value = NULL;
    }

    node->display = &__node_display;
    node->del = &__node_del;

    return node;
}

/*
    Public function that creates a new queue.
*/
struct Queue *new_queue()
{
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    queue->__size = 0;
    queue->__head = NULL;
    queue->__tail = NULL;

    queue->enqueue = &__Queue_enqueue;
    queue->dequeue = &__Queue_dequeue;
    queue->size = &__Queue_size;
    queue->display = &__Queue_display;
    queue->del = &__Queue_del;

    return queue;
}

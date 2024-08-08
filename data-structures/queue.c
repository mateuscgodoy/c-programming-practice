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

void __node_del(struct node *self)
{
    free((char *)self->value);
    free((struct node *)self);
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

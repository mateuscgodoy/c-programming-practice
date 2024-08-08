/* Stack Version 1.0.0
    This version of stack does not free nodes on pop function.
    It just readjusts the Head reference.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snode
{
    char *task;
    struct snode *__next;
    struct snode *__previous;
};

struct snode_iter
{
    struct snode *__current;

    struct snode *(*next)(struct snode_iter *self);
    void (*del)(struct snode_iter *self);
};

struct Stack
{
    struct snode *__head;
    int __length;

    int (*length)(struct Stack *self);
    char *(*pop)(struct Stack *self);
    void (*push)(struct Stack *self, char *task);
    void (*display)(struct Stack *self);
    struct snode_iter *(*iter)(struct Stack *self);
    void (*del)(struct Stack *self);
};

struct snode *__StackIter_next(struct snode_iter *self)
{
    struct snode *output;
    if (self->__current == NULL)
        return NULL;
    output = self->__current;
    self->__current = self->__current->__previous;
    return output;
}

void __StackIter_del(struct snode_iter *self)
{
    free((void *)self);
}

struct snode_iter *__Stack_iter(struct Stack *self)
{
    struct snode_iter *iter = (struct snode_iter *)malloc(sizeof(struct snode_iter));
    iter->__current = self->__head;
    iter->del = &__StackIter_del;
    iter->next = &__StackIter_next;
    return iter;
}

void __Stack_push(struct Stack *self, char *task)
{
    struct snode *new = (struct snode *)malloc(sizeof(struct snode));
    new->task = (char *)malloc(strlen(task) + 1);
    strcpy(new->task, task);
    new->__next = NULL;
    new->__previous = self->__head;

    if (self->__head)
    {
        if (self->__head->__next != NULL)
        {
            self->__head->__next->__previous = new;
            new->__next = self->__head->__next;
        }
        else
            self->__head->__next = new;
    }
    self->__head = new;
    self->__length++;
}

void __Stack_display(struct Stack *self)
{
    int isFirstItem;
    struct snode *current = self->__head;

    if (current == NULL)
        printf("The stack is empty.\n");

    isFirstItem = 1;
    while (current)
    {
        if (isFirstItem)
        {
            printf("[TOP] Task: %s\n", current->task);
            isFirstItem = 0;
        }
        else
            printf("Task: %s\n", current->task);

        current = current->__previous;
    }
}

char *__Stack_pop(struct Stack *self)
{
    if (self->__length == 0)
    {
        printf("Error: You can not pop an item from an empty stack.\n");
        return NULL;
    }
    char *task = self->__head->task;
    self->__head = self->__head->__previous;
    self->__length--;
    return task;
}

int __Stack_length(struct Stack *self)
{
    return self->__length;
}

void __Stack_del(struct Stack *self)
{
    struct snode *current, *temp;

    current = self->__head;
    if (current->__next)
    {
        while (current->__next)
        {
            current = current->__next;
        }
    }
    while (current)
    {
        free(current->task);
        temp = current->__previous;
        free(current);
        current = temp;
    }
    free(self);
}

struct Stack *Stack_new()
{
    struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));
    stack->__head = NULL;
    stack->__length = 0;

    stack->display = &__Stack_display;
    stack->length = &__Stack_length;
    stack->del = &__Stack_del;
    stack->push = &__Stack_push;
    stack->pop = &__Stack_pop;
    stack->iter = &__Stack_iter;

    return stack;
}

int main(void)
{
    struct snode_iter *iter;
    struct snode *current;
    struct Stack *stack = Stack_new();

    stack->display(stack);
    printf("Stack length: %d\n", stack->length(stack));

    stack->push(stack, "Call mom");
    stack->push(stack, "Workout");
    stack->push(stack, "Find a job");

    stack->display(stack);
    printf("Stack length: %d\n", stack->length(stack));

    printf("Pop one: %s\n", stack->pop(stack));
    printf("Pop one: %s\n", stack->pop(stack));

    printf("\n");
    stack->display(stack);
    printf("Stack length: %d\n", stack->length(stack));

    stack->push(stack, "Capture all Pokemon");

    printf("\n");
    stack->display(stack);
    printf("Stack length: %d\n", stack->length(stack));

    printf("\nIterate\n");
    iter = stack->iter(stack);
    while (1)
    {
        current = iter->next(iter);
        if (current == NULL)
            break;
        printf("Task: %s\n", current->task);
    }

    iter->del(iter);
    stack->del(stack);
}

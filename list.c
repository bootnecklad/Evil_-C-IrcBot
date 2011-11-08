#include "list.h"

struct list* list_create(void)
{
    struct list* list = malloc(sizeof(struct list));
    struct node* new_node = malloc(sizeof(struct node));

    list->start = new_node;
    list->end = new_node;

    new_node->data = NULL;
    new_node->next = NULL;

    return list;
}

int list_isempty(struct list* list)
{
    if(list->start == list->end)
    {
        return 1;
    }

    return 0;
}

void list_add(struct list* list, void* data)
{
    struct node* new_node = malloc(sizeof(struct node));
    list->end->next = new_node;
    new_node->next = NULL;
    new_node->data = data;
    list->end = new_node;
}

void list_destroy(struct list* list)
{
    if(list == NULL)
    {
        return;
    }

    struct node* cur = list->start;
    struct node* next;

    do
    {
        next = cur->next;
        free(cur);
        cur = next;
    } while(cur != NULL);

    free(list);
}

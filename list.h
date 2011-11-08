#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>

struct node
{
    void* data;
    struct node* next;
};

struct list
{
    struct node* start;
    struct node* end;
};

struct list* list_create(void);
int  list_isempty(struct list* list);
void list_add(struct list* last, void* n);
void list_destroy(struct list* list);

#endif // __LIST_H__

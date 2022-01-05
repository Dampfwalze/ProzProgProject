#include "linked_list.h"

#include <stdlib.h>

Element *_get(LinkedList *list, unsigned int index)
{
    Element *current = list->first;
    for (size_t i = 0; i < index && current != NULL; i++, current = current->next)
        ;
    return current;
}

LinkedList *linkedList_create()
{
    LinkedList *list = malloc(sizeof(LinkedList)); //{NULL, NULL, NULL};
    list->first = NULL;
    list->last = NULL;
    list->current = NULL;
    return list;
}

int linkedList_add(LinkedList *list, void *element)
{
    Element *e = malloc(sizeof(Element));
    if (e == NULL)
        return 1;
    e->element = element;

    if (list->last != NULL)
    {
        list->last->next = e;
        e->prev = list->last;
    }
    else
    {
        e->prev = NULL;
        list->first = e;
    }
    e->next = NULL;
    list->last = e;
}

int linkedList_addAt(LinkedList *list, void *element, unsigned int index)
{
    Element *e = malloc(sizeof(Element));
    if (e == NULL)
        return 1;
    e->element = element;

    Element *next = _get(list, index);
    Element *prev = next->prev;

    e->next = next;
    e->prev = prev;

    if (next != NULL)
        next->prev = e;
    else
        list->last = e;
    if (prev != NULL)
        prev->next = e;
    else
        list->first = e;
}

int _remove(LinkedList *list, Element *element)
{
    if (element == NULL)
        return 1;
    if (element->prev != NULL)
        element->prev->next = element->next;
    else
        list->first = element->next;
    if (element->next != NULL)
        element->next->prev = element->prev;
    else
        list->last = element->prev;
    free(element);
}

int linkedList_remove(LinkedList *list, void *element)
{
    Element *current = list->first;
    while (current != NULL)
    {
        if (current->element == element)
            return _remove(list, current);
        current = current->next;
    }
    return 1;
}

int linkedList_removeAll(LinkedList *list, void *element)
{
    Element *current = list->first;
    unsigned int count = 0;
    while (current != NULL)
    {
        if (current->element == element)
        {
            Element *c = current->next;
            _remove(list, current);
            current = c;
            count++;
        }
        else
            current = current->next;
    }
    return count;
}

int linkedList_removeAt(LinkedList *list, unsigned int index)
{
    return _remove(list, _get(list, index));
}

void *linkedList_get(LinkedList *list, unsigned int index)
{
    return _get(list, index)->element;
}

void *linkedList_getFirst(LinkedList *list)
{
    return list->first;
}

void *linkedList_getLast(LinkedList *list)
{
    return list->last;
}

void *linkedList_begin(LinkedList *list)
{
    list->current = list->first;
    if (list->current != NULL)
        return list->current->element;
    else
        return NULL;
}

void *linkedList_end(LinkedList *list)
{
    list->current = list->last;
    if (list->current != NULL)
        return list->current->element;
    else
        return NULL;
}

void *linkedList_next(LinkedList *list)
{
    if (list->current != NULL)
        list->current = list->current->next;
    if (list->current != NULL)
        return list->current->element;
    else
        return NULL;
}

void *linkedList_prev(LinkedList *list)
{
    if (list->current != NULL)
        list->current = list->current->prev;
    if (list->current != NULL)
        return list->current->element;
    else
        return NULL;
}

void *linkedList_beginAt(LinkedList *list, int index)
{
    list->current = _get(list, index);
    if (list->current != NULL)
        return list->current->element;
    else
        return NULL;
}

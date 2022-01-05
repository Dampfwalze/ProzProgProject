
typedef struct Element Element;

struct Element
{
    void *element;
    Element *next;
    Element *prev;
};

typedef struct
{
    Element *first;
    Element *last;
    Element *current;
} LinkedList;

LinkedList *linkedList_create();
int linkedList_add(LinkedList *list, void *element);
int linkedList_addAt(LinkedList *list, void *element, unsigned int index);
int linkedList_remove(LinkedList *list, void *element);
int linkedList_removeAll(LinkedList *list, void *element);
int linkedList_removeAt(LinkedList *list, unsigned int index);
void *linkedList_get(LinkedList *list, unsigned int index);
void *linkedList_getFirst(LinkedList *list);
void *linkedList_getLast(LinkedList *list);

void *linkedList_begin(LinkedList *list);
void *linkedList_end(LinkedList *list);
void *linkedList_next(LinkedList *list);
void *linkedList_prev(LinkedList *list);
void *linkedList_beginAt(LinkedList *list, int index);

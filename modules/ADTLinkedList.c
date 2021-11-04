#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ADTLinkedList.h"

struct listnode {
    Pointer value;          //General list
    ListNode next;
};

struct list {
    ListNode dummyNode;
    CompareFunc compare;
    int size;
};


List list_create(CompareFunc compare){
    List list = malloc(sizeof(*list));
    
    list->dummyNode = malloc(sizeof(*(list->dummyNode)));
    list->dummyNode->next = NULL;
    list->dummyNode->value = NULL;
    list->compare = compare;
    list->size = 0;

    return list;
}


int list_size(List list) {
    assert(list);
    return list->size;
}


ListNode list_first(List list) {
    assert(list);
    return list->dummyNode->next;
}


void list_insert(List list, Pointer value){
    assert(list);

    ListNode node = list->dummyNode;
    
    while (node->next){
        node = node->next;
    }

    ListNode new = malloc(sizeof(*new));
    new->value = value;
    new->next = NULL;
    
    node->next = new;
    list->size++;
}


ListNode list_find(List list, Pointer value) {
    assert(list);

    for (ListNode node = list_first(list); node != NULL; node = node->next) {
        if (list->compare && !list->compare(value, node->value))
            return node;
    }

    return NULL;
}


Pointer list_node_value(ListNode node) {
    assert(node);
    return node->value;
}


ListNode list_find_next(ListNode node) {
    assert(node);
    return node->next;
}


void list_destroy(List list, DestroyFunc destroy) {         //Opos kai dipote NULL stin destroy
    assert(list);

    ListNode node = list_first(list);

    while (node) {

        ListNode next = node->next;

        if(destroy)
            destroy(node->value);
            
        free(node);
        node = next;
    }

    free(list->dummyNode);
    free(list);
}
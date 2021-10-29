#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ADTLinkedList.h"

struct listnode {
    Pointer value;          //General list
    ListNode next;
};

struct list {
    ListNode dummyNode;
    int size;
};


List list_create(){
    List list = malloc(sizeof(*list));
    
    list->dummyNode = malloc(sizeof(*(list->dummyNode)));
    list->dummyNode->next = NULL;
    list->dummyNode->value = NULL;
    list->size = 0;

    return list;
}

int list_size(List list) {
    return list->size;
}

ListNode list_first(List list) {
    return list->dummyNode->next;
}

void list_insert(List list, Pointer value){
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


ListNode list_find(List list, CompareFunc compare, Pointer value) {
    for (ListNode node = list_first(list); node != NULL; node = node->next) {
        if (!compare(value, node->value))
            return node;
    }

    return NULL;
}

Pointer list_node_value(ListNode node) {
    return node->value;
}

ListNode list_find_next(ListNode node) {
        return node->next;
}

void list_destroy(List list, DestroyFunc destroy) {         //Opos kai dipote NULL stin destroy
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
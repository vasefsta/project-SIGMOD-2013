/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTLinkedList.c     
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ADTLinkedList.h"
#include "core.h"

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
    List list = malloc(sizeof(*list));                              // Allocate memory for list
    
    list->dummyNode = malloc(sizeof(*(list->dummyNode)));           // Initialize values
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


Pointer list_insert(List list, Pointer value){
    assert(list);

    ListNode exist = list_find(list, value);

    if (exist)
        return list_node_value(exist);

    ListNode node = list->dummyNode;
    
    while (node->next){                                 // Find last node in list
        node = node->next;
    }

    ListNode new = malloc(sizeof(*new));                // Create new ListNode
    new->value = value;
    new->next = NULL;
    
    node->next = new;                                   // Append in list
    list->size++;

    return NULL;
}


ListNode list_find(List list, Pointer value) {
    assert(list);

    for (ListNode node = list_first(list); node != NULL; node = node->next) {               //Traverse through list
        if (list->compare && list->compare(value, node->value) == 0)
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


void list_destroy(List list, DestroyFunc destroy) {
    assert(list);

    ListNode node = list_first(list);

    while (node) {                          // Traverse through list

        ListNode next = node->next;

        if(destroy && node->value)
            destroy(node->value);
            
        free(node);
        node = next;
    }

    free(list->dummyNode);
    free(list);
}
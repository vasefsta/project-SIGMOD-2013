/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTLinkedList.c     
*/

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


void list_insert(List list, Pointer value){
    assert(list);

    ListNode node = list->dummyNode->next;

    ListNode new = malloc(sizeof(*new));                // Create new ListNode
    new->value = value;

    if(node == NULL){
        list->dummyNode->next = new;
        new->next = NULL;
    } else {
        new->next = node;
        list->dummyNode->next = new;                                   // Append in list
    }
    
    list->size++;
}

ErrorCode list_remove(List list, DestroyFunc destroy, Pointer value) {
    assert(list);

    ListNode node = list->dummyNode;
    ListNode next = node->next;

    while (next) {
        if (list->compare && (list->compare(next->value, value) == 0)) {
            if (destroy)
                destroy(next->value);

            node->next = next->next;
            
            free(next);

            return EC_SUCCESS;
        }

        node = next;
        next = next;
    }

    return EC_NO_AVAIL_RES;
}

Pointer list_remove_first(List list) {
    assert(list);

    ListNode dummynode = list->dummyNode;
    ListNode first_node = dummynode->next;

    if (!first_node)
        return NULL;

    Pointer value = first_node->value;

    dummynode->next = first_node->next;
    free(first_node); 

    return value;
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
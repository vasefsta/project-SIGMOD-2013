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
    Pointer value;          //Node value.
    ListNode next;          // Next listnode.
};

struct list {
    ListNode dummyNode;     // Very first node in list.
    CompareFunc compare;    // Function to compare ListNodes.
    int size;               // Number of elemenets in list.
};


List list_create(CompareFunc compare){
    // Create an empty list with first element a dummynode.
    List list = malloc(sizeof(*list));
    list->dummyNode = malloc(sizeof(*(list->dummyNode)));
    list->dummyNode->next = NULL;
    list->dummyNode->value = NULL;
    list->compare = compare;
    list->size = 0;

    // Return this list.
    return list;
}


int list_size(List list) {
    // If list exists, return number of elements in it.
    assert(list);
    return list->size;
}

ListNode list_first(List list) {
    // If list exists, return first node in it
    // which is the next of dummyNode.
    assert(list);
    return list->dummyNode->next;
}


void list_insert(List list, Pointer value){
    // If list exists, continue.
    assert(list);

    // Get first node of list.
    ListNode node = list->dummyNode->next;

    // Create a new ListNode with value = value
    ListNode new = malloc(sizeof(*new));
    new->value = value;
    new->next = NULL;

    // If dummyNode has no next node then assign new as the next.
    if(node == NULL){
        list->dummyNode->next = new;
        new->next = NULL;
    // If dummyNode has a next node then insert new in front of this node.
    } else {
        new->next = node;
        list->dummyNode->next = new;
    }
    
    // Increase list's size.
    list->size++;
}

ErrorCode list_remove(List list, DestroyFunc destroy, Pointer value) {
    // If list exists continue.
    assert(list);

    // Get first and Second nodes from list.
    ListNode node = list->dummyNode;
    ListNode next = node->next;

    // While a next node exists.
    while (next) {
        // If node has same value as value
        if (list->compare && (list->compare(next->value, value) == 0)) {
            // If destroy function exists, destory value with this
            if (destroy)
                destroy(next->value);

            // Connect next node
            node->next = next->next;
            
            // Free current node
            free(next);

            // Decrease list size.
            list->size--;
            
            return EC_SUCCESS;
        }
        // Move to next node.
        node = next;
        next = next->next;
    }

    return EC_NO_AVAIL_RES;
}

Pointer list_remove_first(List list) {
    // If list exists continue.
    assert(list);

    // Get first node.
    ListNode dummynode = list->dummyNode;
    ListNode first_node = dummynode->next;

    // If first does not exist return NULL.
    if (!first_node)
        return NULL;

    // Get value of node.
    Pointer value = first_node->value;

    // connect dummy's next to first's next.
    dummynode->next = first_node->next;
    // Free first node.
    free(first_node); 

    list->size--;

    // Return it's value.
    return value;
}

ListNode list_find(List list, Pointer value) {
    // If list exists continue.
    assert(list);

    // For every node in list.
    for (ListNode node = list_first(list); node != NULL; node = node->next) {
        // If node has same value as node return it
        if (list->compare && list->compare(value, node->value) == 0)
            return node;
    }

    // If no node was found return NULL.
    return NULL;
}


Pointer list_node_value(ListNode node) {
    // If node exists return it's value.
    assert(node);
    return node->value;
}


ListNode list_find_next(ListNode node) {
    // If node exist's, return node's next.
    assert(node);
    return node->next;
}

void list_destroy(List list, DestroyFunc destroy) {
    // If node exist's continue.
    assert(list);

    // Get first node.
    ListNode node = list_first(list);

    // For every node in list.
    while (node) { 
        // Get next node.
        ListNode next = node->next;

        // If destroy function exist's destroy node's value.
        if(destroy && node->value)
            destroy(node->value);

        // Free node.
        free(node);
        node = next;
    }
    // Free dummy node and list.
    free(list->dummyNode);
    free(list);
}
/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTLinkedList.h     
*/


#pragma once

#include "common_types.h"


typedef struct listnode* ListNode;

typedef struct list* List;

// Create an empty list.
List list_create(CompareFunc compare);

// Get Number of elements in list.
int list_size(List list);

// Get first element of list(Next of dummyNode).
ListNode list_first(List list);                                

// Insert value at the start of list.
void list_insert(List list, Pointer value);

// Remove node with specific value from list.
ErrorCode list_remove(List list, DestroyFunc destroy, Pointer value);

// Remove first element of list.
Pointer list_remove_first(List list);

// Return the value of node.
Pointer list_node_value(ListNode node);                         

// Return Listnode with value value if exists
// else return NULL.
ListNode list_find(List list, Pointer value);      

// Return next node of node.
ListNode list_find_next(ListNode node);                      

// Destroy every ListNode and list.
void list_destroy(List list, DestroyFunc destoy);

/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTLinkedList.h     
*/


#pragma once

#include "common_types.h"


typedef struct listnode* ListNode;

typedef struct list* List;

List list_create(CompareFunc compare);                          // Return pointer to a list

int list_size(List list);                                       // Return number of elements in list

ListNode list_first(List list);                                 // Return first ListNode from list

void list_insert(List list, Pointer value);                // Insert value in list.

ErrorCode list_remove(List list, DestroyFunc destroy, Pointer value);

Pointer list_remove_first(List list);

Pointer list_node_value(ListNode node);                         // Return value of node.

ListNode list_find(List list, Pointer value);                   // Return ListNode with value from list.

ListNode list_find_next(ListNode node);                         // Return next ListNode of node.

void list_destroy(List list, DestroyFunc destoy);               // Destroy list with destroy function.

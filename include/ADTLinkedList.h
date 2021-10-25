#pragma once

#include "common_types.h"


typedef struct listnode* ListNode;

typedef struct list* List;

List list_create();

int list_size(List list);

ListNode list_first(List list);

void list_insert(List list, Pointer value);

void list_destroy(List list, DestroyFunc destoy);

ListNode list_find(List list, CompareFunc compare, Pointer value);

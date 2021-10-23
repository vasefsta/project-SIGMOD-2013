#pragma once

#include "common_types.h"


typedef struct listnode* Listnode;


struct listnode {
    Pointer value;
    Listnode next;
};


Listnode list_create();

void list_insert(Listnode list, Pointer value);

void list_destroy(Listnode list, DestroyFunc destoy);

Listnode list_find(Listnode list, CompareFunc compare, Pointer value);

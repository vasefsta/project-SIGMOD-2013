#include <stdio.h>
#include <stdlib.h>

#include "ADTLinkedList.h"



Listnode list_create(){
    Listnode new = malloc(sizeof(*new));
    new->next = NULL;
    new->value = NULL;

    return new;
}

void list_insert(Listnode list, Pointer value){
    
    if(list->value == NULL){
        list->value = value;
    } else {
        while (list->next){
            list = list->next;
        }
        Listnode new = malloc(sizeof(*new));
        new->value = value;
        new->next = NULL;

        list->next = new;
    }
}


Listnode list_find(Listnode list, CompareFunc compare, Pointer value) {
    for (Listnode node = list; list != NULL; node = node->next) {
        if (!compare(value, node->value))
            return node;
    }

    return NULL;
}

void list_destroy(Listnode list, DestroyFunc destroy ) {         //Opos kai dipote NULL stin destroy
    
    while (list) {

        Listnode next = list->next;

        if(destroy)
            destroy(list->value);
            
        free(list);
        list = next;
    }

}
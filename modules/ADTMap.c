/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTMap.c     
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "ADTMap.h"

struct mapnode {
    Pointer value;
    MapNode next;
};

struct map {
    MapNode hashtable;
    unsigned int size;                                  // Size of map
    unsigned int capacity;                              // Numver of elements
    CompareFunc compare_function;
    HashFunc hash_function;
};


Map map_create( CompareFunc compare_function, int size) {
    size *= 1.2;

    Map map = malloc(sizeof(*map));                     // Allocate map
    map->size = size;
    map->capacity = 0;
    map->compare_function = compare_function;
    map->hashtable = malloc(sizeof(struct mapnode)*size);   // Initialize map

    for (int i = 0; i < size; i++) {                        // Initialize every position of map
        map->hashtable[i].value = NULL;
        map->hashtable[i].next = NULL;
    }

    return map;
}

void map_set_hash_function(Map map, HashFunc hash_function) {
    assert(map);
    map->hash_function = hash_function;
}

int map_capacity(Map map) {
    assert(map);
    return map->capacity;
}

ErrorCode map_insert(Map map, Pointer value) {
    assert(map);
    assert(value);
    unsigned int position = map->hash_function(value);          // Get position from hash function
    position %= map->size;                                      // Modulo position with size to be in range

    MapNode node = &map->hashtable[position];
    if(node->value == NULL) {                                    // If node is null
        node->value = value;
    } else {
        while(node->next) {                                     // find end of bucket's list
            node = node->next;
        }

        MapNode new = malloc(sizeof(*new));                     // Create new map node
        new->value = value;
        new->next = NULL;

        node->next = new;                                       // Append in end of list
    }

    map->capacity++;
    return EC_SUCCESS;
}

int map_destroy(Map map, DestroyFunc destroy){
    assert(map);

    for(int i = 0; i < map->size; i++){
        MapNode node =  map->hashtable[i].next;                 // Get next element in bucket

        if(destroy && map->hashtable[i].value){                 // Destroy first element if exists
            destroy(map->hashtable[i].value);
        }

        while (node){                                           // Traverse through bucket list
            MapNode next = node->next;

            if (destroy){
                destroy(node->value);
            }
            free(node);
            node = next;
        }
    }

    free(map->hashtable);
    free(map);

    return 1;
}

Pointer map_find(Map map, Pointer value){
    assert(map);
    assert(value);
    
    unsigned int position = map->hash_function(value);              // Get position from hash function 
    position %= map->size;                                          // Get position in range

    MapNode node = &map->hashtable[position];                       // Get first element in position

    while (node){                                                   // Traverse through list
        if(node->value){
            if(!(map->compare_function(value, node->value))){       // If node->value is same as value
                return node->value;
            }
        }
        node = node->next;
    }
    return NULL;
}


unsigned int hash_string(String value) {     	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    unsigned int hash = 5381;
    
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash  33) + s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo  32.
    

    return hash;
}
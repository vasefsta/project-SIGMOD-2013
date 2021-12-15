/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTMap.c     
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "ADTMap.h"
#include "ADTEntryList.h"

struct mapnode {
    Pointer value;
    MapNode next;
};

struct map {
    MapNode hashtable;                                  // Map.
    unsigned int size;                                  // Full Size of map.
    unsigned int capacity;                              // Number of elements.
    CompareFunc compare_function;                       // Function to compare elements of Map.
    HashFunc hash_function;                             // HashFunction.
};


Map map_create( CompareFunc compare_function, int size) {
    
    // Create an empty map.
    Map map = malloc(sizeof(*map));
    map->size = size;
    map->capacity = 0;
    map->compare_function = compare_function;
    map->hashtable = malloc(sizeof(struct mapnode)*size);

    // Initialize map.
    for (int i = 0; i < size; i++) {
        map->hashtable[i].value = NULL;
        map->hashtable[i].next = NULL;
    }

    // Return map.
    return map;
}

void map_set_hash_function(Map map, HashFunc hash_function) {
    // If map exists, set hash_function to map.
    assert(map);
    map->hash_function = hash_function;
}

int map_capacity(Map map) {
    // If map exists, return number of elements in Map.
    assert(map);
    return map->capacity;
}

Pointer map_node_value(MapNode node) {
    // If mapnode exists, return value of mapnode. 
    assert(node);
    return node->value;
}

ErrorCode map_insert(Map map, Pointer value) {
    // If map and node exists, continue.
    assert(map);
    assert(value);
    
    // Get position from hash function
    unsigned int position = map->hash_function(value);          
    // Modulo position with size to be in range
    position %= map->size;                                      

    // Get mapnode.
    MapNode node = &map->hashtable[position];

    // If node is NULL assign it the value.
    if(node->value == NULL) {                                    
        node->value = value;
    } else {       
        // Create new map node and insert it at the beggining.
        MapNode new = malloc(sizeof(*new));                     
        new->value = value;
        new->next = node->next;
        node->next = new;
    }

    // Increase number of elements in map.
    map->capacity++;

    return EC_SUCCESS;
}


int map_destroy(Map map, DestroyFunc destroy){
    // If map exists, continue.
    assert(map);

    for(int i = 0; i < map->size; i++){
        // Get second element from bucket.
        MapNode node =  map->hashtable[i].next;

        // Destroy first element if exists
        if(destroy && map->hashtable[i].value){                
            destroy(map->hashtable[i].value);
        }

        // For every node (Second to last)
        while (node){                              
            // Get next node.             
            MapNode next = node->next;

            // If destroy function exists, destroy node->value.
            if (destroy && map->hashtable[i].value){
                destroy(node->value);
            }
            // Free node.
            free(node);
            
            // Move to next node.
            node = next;
        }
    }

    // Free map memory.
    free(map->hashtable);
    free(map);

    return 1;
}

Pointer map_find(Map map, Pointer value){
    // If mao and value exists, continue.
    assert(map);
    assert(value);
    
    // Get position from hash function.
    unsigned int position = map->hash_function(value);              
    // Modulo position with size to be in range.
    position %= map->size;

    // Get first element in position.
    MapNode node = &map->hashtable[position];                       

    // For every node in bucket
    while (node){ 
        // If node value exists.
        if(node->value){
            // If node->value is same as value return node->value.
            int res = (map->compare_function(value, node->value));
            if(map->compare_function && res == 0){       
                return node->value;
            }
        }
        // Move to next node.
        node = node->next;
    }
    // If no node was found return NULL.
    return NULL;
}


unsigned int hash_string(String value) {     	
    // djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    unsigned int hash = 5381;
    
    for (char* s = value; *s != '\0'; s++)
        // hash = (hash  33) + s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo  32.
		hash = (hash << 5) + hash + *s;			
    

    return hash;
}
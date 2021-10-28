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
    unsigned int size;
    unsigned int capacity;
    CompareFunc compare_function;
    HashFunc hash_function;
};


Map map_create( CompareFunc compare_function, int size) {
    size *= 1.2;

    Map map = malloc(sizeof(*map));
    map->size = size;
    map->capacity = 0;
    map->compare_function = compare_function;
    map->hashtable = malloc(sizeof(struct mapnode)*size);

    for (int i = 0; i < size; i++) {
        map->hashtable[i].value = NULL;
        map->hashtable[i].next = NULL;
    }

    return map;
}

void map_set_hash_function(Map map, HashFunc hash_function) {
    map->hash_function = hash_function;
}

int map_capacity(Map map) {
    return map->capacity;
}

void map_insert(Map map, Pointer value) {
    assert(value);

    unsigned int position = map->hash_function(value);
    position %= map->size; 

    MapNode node = &map->hashtable[position];

    if(node->value == NULL){
        node->value = value;
    } else {
        while (node->next){
            node = node->next;
        }

        MapNode new = malloc(sizeof(*new));
        new->value = value;
        new->next = NULL;

        node->next = new; 
    }

    map->capacity++;
}

int map_destroy(Map map, DestroyFunc destroy){

    for(int i = 0; i < map->size; i++){
        MapNode node =  map->hashtable[i].next;

        if(destroy && map->hashtable[i].value){
            destroy(map->hashtable[i].value);
        }

        while (node){
            MapNode next = node->next;
            
            if (destroy){
                destroy(node->value);
                free(node);
            }
            node = next;
        }
    }

    free(map->hashtable);
    free(map);

    return 1;
}

int map_find(Map map, Pointer value){
    unsigned int position = map->hash_function(value);
    position %= map->size;

    MapNode node = &map->hashtable[position];


    while (node){
        if(node->value){
            if(!(map->compare_function(value, node->value))){
                return 1;
            }
        }
        node = node->next;
    }
    return 0;
}


unsigned int hash_string(String value) {     	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    unsigned int hash = 5381;
    
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash  33) + s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo  32.
    

    return hash;
}
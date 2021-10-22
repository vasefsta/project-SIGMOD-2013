#include <stdlib.h>

#include "ADTMap.h"

MapNode map_create(int size) {
    MapNode map = malloc(sizeof(*map)*size);

    for (int i = 0; i < size; i++) {
        map[i].query = NULL;
        map[i].next = NULL;
    }

    return map;
}

int map_insert(MapNode map, Query query) {
    int position = hash_string(query);

    MapNode temp = &map[position];

    while (map[position].query != NULL){
        temp = map[position].next;

    }

}


int map_destroy(DestroyFunc destroy) {

}


int hash_string(String value) {     //Voithitiki sto readme apo chatzi

	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    int hash = 5381;
    
    for (char s = value; s != '\0'; s++)
		hash = (hash << 5) + hash + s;			// hash = (hash  33) + s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo  32.
    
    return hash;
}
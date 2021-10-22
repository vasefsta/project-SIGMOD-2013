#include <stdlib.h>
#include <assert.h>

#include "ADTMap.h"



MapNode map_create(int size) {
    size *= 1.2;

    MapNode map = malloc(sizeof(*map)*size);

    for (int i = 0; i < size; i++) {
        map[i].query = NULL;
        map[i].next = NULL;
    }

    return map;
}

void map_insert(MapNode map, Query query, int size) {
    assert(query);

    int position = hash_string(query);
    position %= size;

    MapNode node = &map[position];

    if(map[position].query == NULL){

        map[position].query = query;
        
    } else {

        while (node->next){
            node = node->next;
        }

        MapNode new = malloc(sizeof(*new));
        new->query = query;
        new->next = NULL;

        node->next = new; 
    }

}

int map_destroy(MapNode map, int size){

    for(int i = 0; i < size; i++){
        MapNode node =  map[i].next;

        while (node){
            MapNode next = node->next;
            
            free(node->query->length);
            free(node->query);
            free(node);

            node = next;
        }

    }

    free(map);

    return 1;
}

unsigned int hash_string(Query query) {     	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική

    String value = query->words;
    
    int hash = 5381;
    
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash  33) + s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo  32.
    

    return hash;
}
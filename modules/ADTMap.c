#include <stdlib.h>
#include <assert.h>

#include "ADTMap.h"

struct mapnode {
    Pointer value;
    MapNode next;
};


MapNode map_create(int size) {
    size *= 1.2;

    MapNode map = malloc(sizeof(*map)*size);

    for (int i = 0; i < size; i++) {
        map[i].value = NULL;
        map[i].next = NULL;
    }

    return map;
}

void map_insert(MapNode map, Pointer value, int size) {
    assert(value);

    int position = hash_string(value);
    position %= size;

    MapNode node = &map[position];

    if(map[position].value == NULL){

        map[position].value = value;
        
    } else {

        while (node->next){
            node = node->next;
        }

        MapNode new = malloc(sizeof(*new));
        new->value = value;
        new->next = NULL;

        node->next = new; 
    }

}

int map_destroy(MapNode map, DestroyFunc destroy, int size){

    for(int i = 0; i < size; i++){
        MapNode node =  map[i].next;

        while (node){
            MapNode next = node->next;
            
            if (destroy)
                destroy(node->value);
            
            // free(node->query->words);
            // free(node->query);
            free(node);

            node = next;
        }

    }

    free(map);

    return 1;
}

unsigned int hash_string(String value) {     	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    int hash = 5381;
    
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash  33) + s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo  32.
    

    return hash;
}
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

    String tmp_query = map[position].query;
    MapNode node = &map[position];

    while (tmp_query){
        node = node->next;
        tmp_query = node->query;
    }

    node->query = query;
}

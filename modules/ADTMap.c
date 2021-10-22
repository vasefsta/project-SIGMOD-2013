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

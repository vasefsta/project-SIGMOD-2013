#include "common_types.h"

typedef struct mapnode* MapNode;

typedef struct mapnode {
    Query query;
    MapNode next;
};


MapNode map_create(int size);

int map_insert(MapNode map, Query query);

int map_destroy(DestroyFunc destroy);

unsigned int hash_string(Query query);
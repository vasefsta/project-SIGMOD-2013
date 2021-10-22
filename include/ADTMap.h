#include "common_types.h"

typedef struct mapnode* MapNode;

struct mapnode {
    Query query;
    MapNode next;
};


MapNode map_create(int size);

void map_insert(MapNode map, Query query, int size);

int map_destroy(MapNode map, int size);

unsigned int hash_string(Query query);
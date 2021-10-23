#include "common_types.h"

typedef struct mapnode* MapNode;



MapNode map_create(int size);

void map_insert(MapNode map, Pointer value, int size);

int map_destroy(MapNode map, DestroyFunc destroy, int size);

unsigned int hash_string(String value);
#include "common_types.h"
#include "core.h"

typedef struct mapnode* MapNode;

typedef struct map* Map;

typedef unsigned int (*HashFunc)(Pointer value);


Map map_create(CompareFunc compare_function, int size);

ErrorCode map_insert(Map map, Pointer value);

int map_capacity(Map map);

int map_destroy(Map map, DestroyFunc destroy);

Pointer map_find(Map map, Pointer value);

void map_set_hash_function(Map map, HashFunc hash);

unsigned int hash_string(String value);
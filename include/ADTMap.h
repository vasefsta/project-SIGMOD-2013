/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTMap.h     
*/

#include "common_types.h"
#include "core.h"

typedef struct mapnode* MapNode;

typedef struct map* Map;

typedef unsigned int (*HashFunc)(Pointer value);


Map map_create(CompareFunc compare_function, int size);   // Return Pointer to a map

ErrorCode map_insert(Map map, Pointer value);             // Insert value in map

int map_capacity(Map map);                                // Return number of elements in map

int map_destroy(Map map, DestroyFunc destroy);            // Destroy map and map_nodes with destroy

Pointer map_find(Map map, Pointer value);                 // Find element in map with value value

void map_set_hash_function(Map map, HashFunc hash);       // Set hash as hashfunction of map

void map_print(Map map);                                  // Print elements in map;

unsigned int hash_string(String value);                   // Hashfunction for strings
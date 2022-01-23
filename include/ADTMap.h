/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTMap.h     
*/

#include "common_types.h"
#include "core.h"

typedef struct mapnode* MapNode;

typedef struct map* Map;

typedef unsigned int (*HashFunc)(Pointer value);

// Create an empty hashtable with size size and compare function.
Map map_create(CompareFunc compare_function, int size);

// Insert value in map. Value is inserted first in the bucket.
ErrorCode map_insert(Map map, Pointer value);             

// Get number of elements in map
int map_capacity(Map map);                                

// Destroy map and map_nodes with destroy
int map_destroy(Map map, DestroyFunc destroy);                                 

// Find element in map with value value.
// If such value exists return in, else returns NULL.
Pointer map_find(Map map, Pointer value);                 

// Return node's value.
Pointer map_node_value(MapNode node);

// Set hash as hashfunction of map
void map_set_hash_function(Map map, HashFunc hash);

// Hash function for strings
unsigned int hash_string(String value);                   
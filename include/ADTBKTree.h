/*
 *  Authors: Vasiliki Efstathiou, Nikos Eftychiou
 *  File   : ADTBKTree.h     
*/

#pragma once        

#include "core.h"
#include "common_types.h"
#include "ADTEntryList.h"

#include "ADTMap.h"


// A bktree is represented from the type of BKTree.
typedef struct bktree* BKTree;

typedef struct bknode* BKNode;


// Create and returns an empty BKTree with Matchtype = type, 
// (MT_HAMMING_DIST or MT_EDIT_DIST is defined in MatchType)
// If the matchtype is wrong then it returns NULL.
// 
BKTree bk_create(MatchType type);                                       


// Insert a new entry in the bktree.
// -> If the new entry already exists in the bktree, then 
// we insert the only query of the new entry in the payload of the 
// entry we just found.
// -> If the new entry does not exist in the bktree, then the new entry
// becomes child of the parent node.
// 
ErrorCode bk_insert(BKTree bktree, Entry value);                        


// Returns the number of entries in the bktree.
// 
int bk_size(BKTree bktree);


// Returns the entry that exists in the node (BKNode->entry).
// If the entry does not exist then it returns NULL.
// 
Entry bk_node_value(BKNode node);                                       


// Destroys recursively the bktree. The entries are destroyed by 
// the function destroy. When bktree is destroyed, it can't be used again.
// 
void bk_destroy(BKTree bktree, DestroyFunc destroy);  


// map_result stores Special elements, complete_queries stores all QueryID's that are fully matched.
// Searches at tree to find an entry with entry->word = word. If such entry exists then adds entry's queries
// to map_result. If a Special element is found where Special->query->length = number of elements in Special->words
// then the query is added in complete_queries.
// 
int bk_find(BKTree bktree, Map map_result, List complete_queries, CompareFunc compare_query, String word, int n);    


// Search in the bktree if the entry exists.
// -> If it does then it returns the exist entry.
// -> If it does not then it returns NULL.
// 
Entry bk_find_entry(BKTree bktree, String word);

////////////////////////////////////////////////////////////////////////
// Helpful functions that return the destance of value1 and value2
////////////////////////////////////////////////////////////////////////

// Returns the hamming distance of value1 and value2
// 
int hamming_distance(Pointer value1, Pointer value2);                   


// Returns the edit distance of value1 and value2
// 
int edit_distance(Pointer value1, Pointer value2);                      
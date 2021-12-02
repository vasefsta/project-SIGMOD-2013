/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTBKTree.h     
*/

#pragma once

#include "core.h"
#include "common_types.h"
#include "ADTEntryList.h"

#include "ADTMap.h"


typedef struct bktree* BKTree;

typedef struct bknode* BKNode;

BKTree bk_create(MatchType type);                                       // Returns a BKTree (MT_HAMMING_DIST or MT_EDIT_DIST is defined in MatchType)

ErrorCode bk_insert(BKTree bktree, Entry value);                        // Inserts value at bktree

Entry bk_node_value(BKNode node);                                       // If node is not NULL returns it

void bk_destroy(BKTree bktree, DestroyFunc destroy);                    // Destroy bktree (and nodes with destroy)

int bk_find(BKTree bktree, Map map_result, List complete_queries, CompareFunc compare_query, String word, int n);    // Find entries from bktree with threshold n from word and insert them in entrylist. 

Entry bk_find_entry(BKTree bktree, String word);

int hamming_distance(Pointer value1, Pointer value2);                   // Returns the hamming distance of value1 and value2

int edit_distance(Pointer value1, Pointer value2);                      // Returns the edit distance of value1 and value2
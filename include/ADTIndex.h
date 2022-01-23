/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTIndex.h     
*/

#pragma once

#include "common_types.h"
#include "core.h"
#include "ADTEntryList.h"
#include "ADTMap.h"

// An index is represented from the type of Index.
typedef struct index* Index;

// Return pointer to an empty index with size and compare function
// 
Index create_index(MatchType matchtype, CompareFunc compare,  int size);            

// Get all entries from entrylist and build a Map or BKTree from them.
// 
ErrorCode build_entry_index(Index index, const EntryList entrylist);                

// If index is a bktree then calls bk_find to complete the task.
// If index is a Map then it checks if this word makes any queries 
// fully match the document. If this happens then inserts these queries 
// to complete_queries.
// 
ErrorCode lookup_entry_index(Index index, String word, int threshold, 
Map map_result, List complete_queries, CompareFunc compare_queries);

// Return's the number of elements in index.
// 
int size_index(Index index);

// Return's index's index.
// 
Pointer index_index(Index index);

// Destroys index and destroyes index's elements with DestroyFunc.
// 
ErrorCode destroy_entry_index(Index index, DestroyFunc destroy);
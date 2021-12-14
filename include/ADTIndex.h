/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTIndex.h     
*/

#pragma once

#include "common_types.h"
#include "core.h"
#include "ADTEntryList.h"
#include "ADTMap.h"

typedef struct index* Index;

Index create_index(MatchType matchtype, CompareFunc compare,  int size);            // Return pointer to an empty index with size and compare function

ErrorCode build_entry_index(Index index, const EntryList entrylist);                // Insert entrylist's entries in index

ErrorCode lookup_entry_index(Index index, String word, int threshold,               // Insert entries with threshold in range in result
Map map_result, List complete_queries, CompareFunc compare_queries);

int size_index(Index index);

Pointer index_index(Index index);

Pointer find_index(Index index, Entry entry);

ErrorCode destroy_entry_index(Index index, DestroyFunc destroy);                                         // Destroy index
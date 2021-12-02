/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : misc.h     
*/

#pragma once

#include "common_types.h"
#include "ADTEntryList.h"
#include "ADTMap.h"

struct special{
    Query query;
    int times;
};

typedef struct special *Special;

String path_of_doc(String namedoc);                                  // create the path of the given document

int hash_func(Query query);                                          // Hash string of query

List find_complete_queries(EntryList entrylist, CompareFunc compare_query);                 // Return List of queries that fully match from entrylist

Query convert_to_query(String string);                               // Return Pointer to query with string as word 

String *Seperate_sentence(Query query);                              // Return Array of Strings with each position being one word from query

List deduplicated_words(String filename);                            // Read a file with filename and create a list with each word (no duplicate words are inserted)

List deduplicated_words_map(String doc_string);                         // Read a file with filename and create a list with each word (no duplicate words are inserted)

Map map_of_queries(String filename, EntryList entrylist);            // Return Pointer to a map of queries read from filename and fill entrylist with entries with unique words for queries

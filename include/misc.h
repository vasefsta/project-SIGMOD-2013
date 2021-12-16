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
    List words;
};

typedef struct special *Special;

int hash_func(Query query);                                          // Hash string of query

Query convert_to_query(String string);                               // Return Pointer to query with string as word 

String *Seperate_sentence(Query query);                              // Return Array of Strings with each position being one word from query

List deduplicated_words_map(String doc_string);                         // Read a file with filename and create a list with each word (no duplicate words are inserted)

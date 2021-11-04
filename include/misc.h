#pragma once

#include "common_types.h"
#include "ADTEntryList.h"
#include "ADTMap.h"

int compare_queries(Query q1, Query q2);

int hash_func(Query query);

int compare_entries(Entry e1, Entry e2);

const void destroy_query(Query q);

List find_complete_queries(EntryList entrylist);

Query convert_to_query(String string);

String *Seperate_sentence(Query query);

List deduplicated_words(String filename);

Map map_of_queries(String filename, EntryList entrylist);

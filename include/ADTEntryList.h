#pragma once

#include "common_types.h"
#include "ADTLinkedList.h"
#include "core.h"

struct entry{
    String word;                //Holds keyword for queries who have the same keyword
    List payload;               //List that holds queries that have the same keyword "word"
};

typedef struct entry* Entry;

typedef List EntryList;

Entry create_entry(String word);

ErrorCode destroy_entry(Entry entry);

EntryList create_entry_list();

unsigned int get_number_entries(EntryList entrylist);

ErrorCode add_entry(EntryList entrylist, Entry new_entry);

Entry get_first(EntryList entrylist);

Entry get_next(EntryList entrylist, Entry current_entry);

Entry find_entry(EntryList entrylist, Entry current_entry);

ErrorCode destroy_entry_list(EntryList entrylist);


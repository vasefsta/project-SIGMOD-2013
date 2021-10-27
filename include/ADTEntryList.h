#pragma once

#include "common_types.h"
#include "ADTLinkedList.h"
#include "core.h"

struct entry{
    String word;
    List payload;
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

Pointer list_find_next(List list, CompareFunc conpare, Pointer value);

ErrorCode destroy_entry_list(EntryList entrylist);


#pragma once

#include "common_types.h"
#include "ADTLinkedList.h"
#include "core.h"

typedef struct entry* Entry;

typedef List EntryList;

Entry create_entry(String word);

String get_entry_word(Entry entry);

List get_entry_payload(Entry entry);

ErrorCode destroy_entry(Entry entry);

EntryList create_entry_list();

unsigned int get_number_entries(EntryList entrylist);

ErrorCode add_entry(EntryList entrylist, Entry new_entry);

Entry get_first(EntryList entrylist);

Entry get_next(EntryList entrylist, Entry current_entry);

Entry find_entry(EntryList entrylist, Entry current_entry);

ErrorCode destroy_entry_list(EntryList entrylist, DestroyFunc destroy_value);

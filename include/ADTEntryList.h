/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTEntryList.h     
*/


#pragma once

#include "common_types.h"
#include "ADTLinkedList.h"
#include "core.h"


/***************
*    Entry     *
***************/

typedef struct entry* Entry;


Entry create_entry(String word, CompareFunc compare);                           // Return a pointer to an entry with word and compare function

String get_entry_word(Entry entry);                                             // Return String to entry word value

List get_entry_payload(Entry entry);                                            // Return List of entry's payload

ErrorCode destroy_entry(Entry entry);                                           // Destroy entry


/***************
*  Entrylist   *
***************/

typedef List EntryList;


EntryList create_entry_list(CompareFunc compare);                               // Returns pointer to entrylist with compare function

unsigned int get_number_entries(EntryList entrylist);                           // Returns number of entries in entrylist

ErrorCode add_entry(EntryList entrylist, Entry new_entry);                      // Add new_entry in entrylist

Entry get_first(EntryList entrylist);                                           // Get first entry from entrylist

Entry get_next(EntryList entrylist, Entry current_entry);                       // Get entry next to current_entry from entrylist

Entry find_entry(EntryList entrylist, Entry current_entry);                     // Get entry from entrylist with word same as current_entry

ErrorCode destroy_entry_list(EntryList entrylist, DestroyFunc destroy_value);   // Destroy entrylist and entries with destroy_value

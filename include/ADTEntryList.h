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

struct entry{
    String word;                //Holds keyword for queries who have the same keyword
    List payload;               //List that holds queries that have the same keyword "word"
};

typedef struct entry* Entry;


Entry create_entry(String word, CompareFunc compare);                           // Return a pointer to an entry with word and compare function


/***************
*  Entrylist   *
***************/

typedef List EntryList;


EntryList create_entry_list(CompareFunc compare);                               // Returns pointer to entrylist with compare function

unsigned int get_number_entries(EntryList entrylist);                           // Returns number of entries in entrylist

Entry add_entry(EntryList entrylist, Entry new_entry);                      // Add new_entry in entrylist

Entry get_first(EntryList entrylist);                                           // Get first entry from entrylist

Entry get_next(EntryList entrylist, Entry current_entry);                       // Get entry next to current_entry from entrylist

Entry find_entry(EntryList entrylist, Entry current_entry);                     // Get entry from entrylist with word same as current_entry

ErrorCode destroy_entry_list(EntryList entrylist, DestroyFunc destroy_value);   // Destroy entrylist and entries with destroy_value

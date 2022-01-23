/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTEntryList.c     
*/


#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "ADTEntryList.h"


Entry create_entry(String word, CompareFunc compare) {    

    // Create new entry
    Entry entry = malloc(sizeof(*entry));                   

    // Assign entry_word the word.
    entry->word = word;
    // Create empty list for payload
    entry->payload = list_create(compare);                  

    return entry;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


EntryList create_entry_list(CompareFunc compare) {
    // Create a new entrylist and return it.
    EntryList entrylist = list_create(compare);
    return entrylist;
}


unsigned int get_number_entries(EntryList entrylist) {
    // return the size of entrylist.
    return list_size(entrylist);
}


ErrorCode add_entry(EntryList entrylist, Entry new_entry){
    // Insert new_entry to entrylist.
    list_insert(entrylist, new_entry);
    return EC_SUCCESS;
}


Entry get_first(EntryList entrylist){
    // If entrylist has entrys returns the first one else returns NULL.
    ListNode node = list_first(entrylist);
    if(node == NULL)
        return NULL;
        
    return list_node_value(node);
}


Entry get_next(EntryList entrylist, Entry current_entry){
    // Checks if entry exists in entrylist.
    ListNode node = list_find(entrylist, current_entry);

    // If no node was found return NULL.
    if (!node)                                      
        return NULL;
    // Else if this entry is not the last one return next entry in entrylist.
    else {
        node = list_find_next(node);

        if (node) {
            Entry entry = list_node_value(node);
            return entry;
        } else 
            return NULL;
    }
}


Entry find_entry(EntryList entrylist, Entry current_entry) {
    // If an entrylist exists.
    assert(entrylist);
    
    // Check if current_entry exists in entrylist.
    ListNode node = list_find(entrylist, current_entry);

    // If current_entry exists return it. 
    if (node) {
        Entry entry = list_node_value(node);
        return entry;
    } else
        // Else return NULL.
        return NULL;
}


ErrorCode destroy_entry_list(EntryList entrylist, DestroyFunc destroy_value) {
    // If an entrylist exists.
    assert(entrylist);
    // Destroy entrylist.
    list_destroy(entrylist, (DestroyFunc)destroy_value);
    return EC_SUCCESS;
}


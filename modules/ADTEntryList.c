#include <stdlib.h>
#include "ADTEntryList.h"


int compare_entries(Entry entry1, Entry entry2) {
    return(strcmp(entry1->word, entry2->word));
}   


Entry create_entry(String word) {      // NA APOFASISOUME AN PREPEI NA EPISTREFETAI KAI TO ERROR
    Entry entry = malloc(sizeof(*entry));

    entry->word = word;
    entry->payload = list_create();

    return entry;
}

ErrorCode destroy_entry(Entry entry) {
    free(entry->word);
    list_destroy(entry->payload, NULL);
}

EntryList create_entry_list() {
    EntryList entrylist = list_create();

    return entrylist;
}

unsigned int get_number_entries(EntryList entrylist) {
    return list_size(entrylist);
}

ErrorCode add_entry(EntryList entrylist, Entry new_entry){
    list_insert(entrylist, new_entry);
}

Entry get_first(EntryList entrylist){
    return list_first(entrylist);
}

Entry get_next(EntryList entrylist, Entry current_entry){
    return list_find_next(entrylist, compare_entries, current_entry);
}

ErrorCode destroy_entry_list(EntryList entrylist){
    list_destroy(entrylist, destroy_entry);
}


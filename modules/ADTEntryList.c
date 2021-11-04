#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ADTEntryList.h"

struct entry{
    String word;                //Holds keyword for queries who have the same keyword
    List payload;               //List that holds queries that have the same keyword "word"
};


Entry create_entry(String word) {      // NA APOFASISOUME AN PREPEI NA EPISTREFETAI KAI TO ERROR
    Entry entry = malloc(sizeof(*entry));

    entry->word = word;
    entry->payload = list_create();

    return entry;
}


String get_entry_word(Entry entry){
    assert(entry);
    return entry->word;
}


List get_entry_payload(Entry entry){
    assert(entry);
    return entry->payload;
}


ErrorCode destroy_entry(Entry entry) {
    assert(entry);

    free(entry->word);
    list_destroy(entry->payload, NULL);
    free(entry);

    return EC_SUCCESS; // na to elexw xana me to ti tha epistrefei epd den xeroume sigoura an i list_destroy leitourgei swsta
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////


EntryList create_entry_list() {
    EntryList entrylist = list_create();

    return entrylist;
}


unsigned int get_number_entries(EntryList entrylist) {
    return list_size(entrylist);
}


ErrorCode add_entry(EntryList entrylist, Entry new_entry){
    list_insert(entrylist, new_entry);
    return EC_SUCCESS;      // na to elexw xana me to ti tha epistrefei epd den xeroume sigoura an i list_insert leitourgei swsta
}


Entry get_first(EntryList entrylist){
    ListNode node = list_first(entrylist);
    if(node == NULL)
        return NULL;
        
    return list_node_value(node);
}


Entry get_next(EntryList entrylist, Entry current_entry, CompareFunc compare){
    for (ListNode node = list_first(entrylist); 
        node != NULL; 
        node = list_find_next(node)) {
            Entry entry = list_node_value(node);
            if (!compare(entry, current_entry)){
                node = list_find_next(node);
                
                if(node == NULL)
                    return NULL;
                Entry entry = list_node_value(node);
                return entry;
            }
    }
    return NULL;
}


Entry find_entry(EntryList entrylist, Entry current_entry, CompareFunc compare) {
    assert(entrylist);

    ListNode node = list_find(entrylist, (CompareFunc)compare, current_entry);

    if (node) {
        Entry entry = list_node_value(node);
        return entry;
    } else
        return NULL;
}


ErrorCode destroy_entry_list(EntryList entrylist, DestroyFunc destroy_value) {
    assert(entrylist);
    list_destroy(entrylist, (DestroyFunc)destroy_value);//Evala null dame je edulepsen anti destroy entry
    return EC_SUCCESS; // na to elexw xana me to ti tha epistrefei epd den xeroume sigoura an i destroy_entry_list leitourgei swsta
}


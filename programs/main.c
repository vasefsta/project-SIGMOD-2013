/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : main.c     
*/

#include "common_types.h"
#include "core.h"
#include "ADTLinkedList.h"
#include "ADTEntryList.h"
#include "ADTMap.h"
#include "ADTIndex.h"
#include "misc.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(EXACT)

int main(){

    puts("");
    
    EntryList entrylist = create_entry_list((CompareFunc)compare_entries);
    EntryList result = create_entry_list((CompareFunc)compare_entries);
    
    puts("Inserting queries in hashtable and entrylist...");
    Map map = map_of_queries("../misc/queries.txt", entrylist);
    puts("");


    puts("Deduplicating \"Document1\"...");
    List list = deduplicated_words("../misc/documents/Document1");
    puts("");

    Index index_exact = create_index(MT_EXACT_MATCH, (CompareFunc)compare_entries, 100);

    puts("Building index...");
    build_entry_index(index_exact, entrylist);
    puts("");

    puts("Looking for words in index...");   
    for(ListNode node = list_first(list); node != NULL; node = list_find_next(node)){
        String word = list_node_value(node);
        lookup_entry_index(index_exact, word, 0, result, (CompareFunc) compare_queries);
    }
    puts("");

    List complete_list = find_complete_queries(result);


    for(ListNode node = list_first(complete_list); node != NULL; node = list_find_next(node)){
        Query query = list_node_value(node);
        printf("%s is a complete query for Document1...\n\n",query->words);
    }

    list_destroy(complete_list, NULL);

//###################################################################
    
    list_destroy(result, (DestroyFunc) NULL);

    list_destroy(list, (DestroyFunc) free);

    destroy_entry_index(index_exact);

    destroy_entry_list(entrylist, (DestroyFunc) destroy_entry);

    map_destroy(map,(DestroyFunc) destroy_query);

    return 0;
}

#elif defined(HAMMING)

int main(){

    puts("");
    
    EntryList entrylist = create_entry_list((CompareFunc)compare_entries);
    EntryList result = create_entry_list((CompareFunc)compare_entries);
    
    puts("Inserting queries in hashtable and entrylist...");
    Map map = map_of_queries("../misc/queries.txt", entrylist);
    puts("");


    puts("Deduplicating \"Document1\"...");
    List list = deduplicated_words("../misc/documents/Document1");
    puts("");

    Index index_hamming = create_index(MT_HAMMING_DIST, (CompareFunc)compare_entries, 100);

    puts("Building index...");
    build_entry_index(index_hamming, entrylist);
    puts("");

    puts("Looking for words in index...");   
    for(ListNode node = list_first(list); node != NULL; node = list_find_next(node)){
        String word = list_node_value(node);
        lookup_entry_index(index_hamming, word, 0, result, (CompareFunc) compare_queries);
    }
    puts("");

    List complete_list = find_complete_queries(result);


    for(ListNode node = list_first(complete_list); node != NULL; node = list_find_next(node)){
        Query query = list_node_value(node);
        printf("%s is a complete query for Document1...\n\n",query->words);
    }

    list_destroy(complete_list, NULL);

//###################################################################
    
    list_destroy(result, (DestroyFunc) NULL);

    list_destroy(list, (DestroyFunc) free);

    destroy_entry_index(index_hamming);

    destroy_entry_list(entrylist, (DestroyFunc) destroy_entry);

    map_destroy(map,(DestroyFunc) destroy_query);

    return 0;

}

#elif defined(EDIT)

int main(){
    
    puts("");
    
    EntryList entrylist = create_entry_list((CompareFunc)compare_entries);
    EntryList result = create_entry_list((CompareFunc)compare_entries);
    
    puts("Inserting queries in hashtable and entrylist...");
    Map map = map_of_queries("../misc/queries.txt", entrylist);
    puts("");


    puts("Deduplicating \"Document1\"...");
    List list = deduplicated_words("../misc/documents/Document1");
    puts("");

    Index index_edit = create_index(MT_EDIT_DIST, NULL, 100);

    puts("Building index...");
    build_entry_index(index_edit, entrylist);
    puts("");

    puts("Looking for words in index...");   
    for(ListNode node = list_first(list); node != NULL; node = list_find_next(node)){
        String word = list_node_value(node);
        lookup_entry_index(index_edit, word, 4, result, NULL);
    }
    puts("");

    List complete_list = find_complete_queries(result);


    for(ListNode node = list_first(complete_list); node != NULL; node = list_find_next(node)){
        Query query = list_node_value(node);
        printf("%s is a complete query for Document1...\n\n",query->words);
    }

    list_destroy(complete_list, NULL);

//###################################################################
    
    list_destroy(result, (DestroyFunc) NULL);

    list_destroy(list, (DestroyFunc) free);

    destroy_entry_index(index_edit);

    destroy_entry_list(entrylist, (DestroyFunc) destroy_entry);

    map_destroy(map,(DestroyFunc) destroy_query);

    return 0;

}


#endif
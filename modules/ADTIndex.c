/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTIndex.c     
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ADTIndex.h"
#include "ADTMap.h"
#include "ADTBKTree.h"
#include "misc.h"

unsigned int hash_function(Entry entry) {
    // Hash entry->word and return hash_value.
	return hash_string(entry->word);
}

struct index {
    Pointer index;          // Holds a Pointer to a map or a bktree.               
    MatchType matchtype;    // MT_EXACT_MATCH or MT_EDIT_DIST or MT_HAMMING_DIST.
    int size;               // Number of elements in index.
};  


Index create_index(MatchType matchtype, CompareFunc compare, int size) {
    // Create empty index
    Index index = malloc(sizeof(*index));                                   

    // If matchtype is MT_EXACT_MATCH create map for index
    if (matchtype == MT_EXACT_MATCH){
        index->index = (Map) map_create((CompareFunc) compare, size);       
        map_set_hash_function(index->index, (HashFunc) hash_function);
    }
    // If matchtype is MT_HAMMING_DIST or MT_EDIT_DIST create a bktree for index.
    else if (matchtype == MT_HAMMING_DIST || matchtype == MT_EDIT_DIST)
        index->index = bk_create(matchtype);                                
    
    // Assign values.
    index->matchtype = matchtype;
    index->size = 0;

    return index;
}

ErrorCode build_entry_index(Index index, const EntryList entrylist) {
    ErrorCode errcode;

    // For every entry in entrylist.
    for (Entry entry = get_first(entrylist); entry != NULL; entry = get_next(entrylist, entry)) {

            // If matchtype = MT_EXACT_MATCH insert in map.
            if (index->matchtype == MT_EXACT_MATCH)
                errcode = map_insert((Map)index->index, entry);       

            // If matchtype = MT_EDIT_DIST or MT_HAMMING_DIST insert in bktree.
            else if (index->matchtype == MT_HAMMING_DIST || index->matchtype == MT_EDIT_DIST)
                errcode = bk_insert((BKTree) index->index, entry);

            // If an error occured exit.
            if (errcode == EC_FAIL)
                return EC_FAIL;
            
            // Increase index's size.
            index->size++;
    }
    return EC_SUCCESS;
}


ErrorCode lookup_entry_index(Index index, String word, int threshold, Map map_result, List complete_queries, CompareFunc compare_query) {

    // If matchtype is MT_EDIT_DIST or MT_HAMMING_DIST call bk_find.
    if (index->matchtype == MT_EDIT_DIST || index->matchtype == MT_HAMMING_DIST)
        bk_find((BKTree)index->index, map_result, complete_queries, compare_query, word, threshold);
    
    // if matchtype is MT_EXACT_MATCH. 
    else if (index->matchtype == MT_EXACT_MATCH) {
        // Create a dummy entry with entry.word = word.
        struct entry entry;
        entry.word = word;
        entry.payload = NULL;

        // Check if entry exists in index.
        Entry res = map_find((Map)index->index, &entry);                 
        
        // If entry exists
        if(res != NULL) {
            // For every Query in entry's payload.
            for (ListNode node = list_first(res->payload); node != NULL; node = list_find_next(node)) {
                Query query = list_node_value(node);
                
                // Create a dummy special.
                struct special tmpspecial;
                tmpspecial.query = query;
                tmpspecial.words = NULL;

                // Check if Special exists in map_result.
                Special special = map_find(map_result, &tmpspecial);

                // If no special was found.
                if (!special) {
                    // Create a new Special.
                    special = malloc(sizeof(*special));
                    special->query = query;
                    special->words = list_create((CompareFunc) strcmp);
                    
                    // Insert it to map_result.
                    map_insert(map_result, special);

                    // Insert entry's word to special's words.
                    list_insert(special->words, entry.word);

                    // If size of special words is equal to query's length 
                    // means that the query fully matches this document.
                    if (list_size(special->words) == special->query->length) {
                        // Create a QueryID.
                        QueryID* queryid = malloc(sizeof(*queryid));
                        *queryid = query->queryID;
                        // Insert queryID to complete_queries list.
                        list_insert(complete_queries, queryid);
                    }

                }
                // If entry.word does not exist in special->words.
                // We check this so we do not store duplicate words
                // in special->words.
                else if (!list_find(special->words, entry.word)){
                    // Insert word to special->words/
                    list_insert(special->words, entry.word);

                    // If size of special words is equal to query's length 
                    // means that the query fully matches this document.
                    if (list_size(special->words) == special->query->length) {
                        // Create a QueryID.
                        QueryID* queryid = malloc(sizeof(*queryid));
                        *queryid = query->queryID;
                        // Insert queryID to complete_queries list.
                        list_insert(complete_queries, queryid);
                    }
                }
                
            }   
        } else
            return EC_NO_AVAIL_RES;  
    }
    return EC_SUCCESS;
}


int size_index(Index index) {
    // Return index's size.
    return index->size;
}

Pointer index_index(Index index){
    // Return index's index (Bktree or Map).
    return index->index;
}

ErrorCode destroy_entry_index(Index index, DestroyFunc destroy) {
    // If matchtype is MT_EDIT_DIST or MT_HAMMING_DIST
    if (index->matchtype == MT_EDIT_DIST || index->matchtype == MT_HAMMING_DIST) 
        // Destroy bktree.
        bk_destroy((BKTree)index->index, destroy);

    // If matchtype is MT_EXACT_MATCH
    else 
        // Destroy Map.
        map_destroy((Map)index->index, destroy);

    // Free index.
    free(index);

    return EC_SUCCESS;
}
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
	return hash_string(entry->word);
}

struct index {
    Pointer index;
    MatchType matchtype;
    int size;
};


Index create_index(MatchType matchtype, CompareFunc compare, int size) {
    Index index = malloc(sizeof(*index));                                   // Create empty index

    if (matchtype == MT_EXACT_MATCH){
        index->index = (Map) map_create((CompareFunc) compare, size);       // Create map for index
        map_set_hash_function(index->index, (HashFunc) hash_function);
    }
    else if (matchtype == MT_HAMMING_DIST || matchtype == MT_EDIT_DIST)
        index->index = bk_create(matchtype);                                // Create bk tree for index
    
    index->matchtype = matchtype;
    index->size = 0;

    return index;
}

ErrorCode build_entry_index(Index index, const EntryList entrylist) {
    ErrorCode errcode;
    for (Entry entry = get_first(entrylist);                            // Traverse through entrylist
        entry != NULL; 
        entry = get_next(entrylist, entry)) {

            if (index->matchtype == MT_EXACT_MATCH)
                errcode = map_insert((Map)index->index, entry);         
            else if (index->matchtype == MT_HAMMING_DIST || index->matchtype == MT_EDIT_DIST)
                errcode = bk_insert((BKTree) index->index, entry);

            if (errcode == EC_FAIL)
                return EC_FAIL;
            
            index->size++;
    }

    

    return EC_SUCCESS;
}


ErrorCode lookup_entry_index(Index index, String word, int threshold, Map map_result, List complete_queries, CompareFunc compare_query) {

    if (index->matchtype == MT_EDIT_DIST || index->matchtype == MT_HAMMING_DIST)
        bk_find((BKTree)index->index, map_result, complete_queries, compare_query, word, threshold);
    else {
        struct entry entry;

        entry.word = word;
        entry.payload = NULL;

        Entry res = map_find((Map)index->index, &entry);                 // Check if it exists in index      // Exact match
        
        if(res != NULL) {                                                // If not
            for (ListNode node = list_first(res->payload); node != NULL; node = list_find_next(node)) {
                Query query = list_node_value(node);
                
                struct special tmpspecial;

                tmpspecial.query = query;
                tmpspecial.times = 0;

                Special special = map_find(map_result, &tmpspecial);

                if (!special) {
                    special = malloc(sizeof(*special));

                    special->query = query;
                    special->times = 1;

                    map_insert(map_result, special);
                
                } else if (tmpspecial.times < tmpspecial.query->length)
                    special->times++;

                if (tmpspecial.times == tmpspecial.query->length) 
                    list_insert(complete_queries, &query->queryID);
                
            }   
        } else 
            return EC_NO_AVAIL_RES;
    }

    return EC_SUCCESS;
}


int size_index(Index index) {
    return index->size;
}

Pointer index_index(Index index){
    return index->index;
}

ErrorCode destroy_entry_index(Index index, DestroyFunc destroy) {
    if (index->matchtype == MT_EDIT_DIST || index->matchtype == MT_HAMMING_DIST) 
        bk_destroy((BKTree)index->index, destroy);
    else 
        map_destroy((Map)index->index, destroy);

    free(index);

    return EC_SUCCESS;
}
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ADTIndex.h"
#include "ADTMap.h"
#include "ADTBKTree.h"

unsigned int hash_function(String word) {
	return hash_string(word);
}


Index create_index(MatchType matchtype, CompareFunc compare, int size) {
    Index index = malloc(sizeof(*index));

    if (matchtype == MT_EXACT_MATCH){
        index->index = (Map) map_create((CompareFunc) compare, size);
        map_set_hash_function(index->index, (HashFunc) hash_function);
    }
    else if (matchtype == MT_HAMMING_DIST || matchtype == MT_EDIT_DIST)
        index->index = bk_create(matchtype);
    
    index->matchtype = matchtype;

    return index;
}

ErrorCode build_entry_index(Index index, const EntryList entrylist) {
    ErrorCode errcode;
    for (Entry entry = get_first(entrylist); 
        entry != NULL; 
        entry = get_next(entrylist, entry)) {

            if (index->matchtype == MT_EXACT_MATCH)
                errcode = map_insert((Map) index->index, entry); 
            else if (index->matchtype == MT_HAMMING_DIST || index->matchtype == MT_EDIT_DIST)
                errcode = bk_insert((BKTree) index, entry);

            if (errcode == EC_FAIL)
                return EC_FAIL;
    }

    return EC_SUCCESS;
}


ErrorCode lookup_entry_index(Index index, String word, int threshold, EntryList result) {
    if (index->matchtype == MT_EDIT_DIST || index->matchtype == MT_HAMMING_DIST)
        bk_find((BKTree)index, result, word, threshold);
    else {
        String word2 = strdup(word);
        Entry entry = create_entry(word2);
        Entry res = map_find((Map)index->index, entry);
        if(res != NULL){
            puts("AAAAAAAAaa");
            list_insert(result, res);
        }

        destroy_entry(entry);
    }

    return EC_SUCCESS;
}

ErrorCode destroy_entry_index(Index index) {
    if (index->matchtype == MT_EDIT_DIST || index->matchtype == MT_HAMMING_DIST) 
        bk_destroy((BKTree)index->index, NULL);
    else 
        map_destroy((Map)index->index, NULL);

    free(index);

    return EC_SUCCESS;
}
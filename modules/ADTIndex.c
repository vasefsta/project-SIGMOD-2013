#include "stdlib.h"

#include "ADTIndex.h"
#include "ADTMap.h"
#include "ADTBKTree.h"


ErrorCode build_entry_index(Index index, const EntryList entrylist) {
    ErrorCode errcode;
    for (Entry entry = get_first(entrylist); 
        entry != NULL; 
        entry = get_next(entrylist, entry)) {

            if (index->matchtype == MT_EXACT_MATCH)
                errcode = map_insert((Map) index, entry); 
            else if (index->matchtype == MT_HAMMING_DIST || index->matchtype == MT_EXACT_MATCH)
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
        Entry entry = create_entry(word);
        Entry res = map_find((Map)index->index, entry);

        list_insert(result, res);

        destroy_entry(entry);
    }

    return EC_SUCCESS;
}

ErrorCode destroy_entry_index(Index index) {
    if (index->matchtype == MT_EDIT_DIST || index->matchtype == MT_HAMMING_DIST) 
        bk_destroy((BKTree)index->index, NULL);
    else 
        map_destroy((Map)index, NULL);
}
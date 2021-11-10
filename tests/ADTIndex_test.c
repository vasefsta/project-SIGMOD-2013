#include <stdlib.h>
#include <string.h>

#include "acutest.h"
#include "ADTIndex.h"
#include "ADTEntryList.h"
#include "misc.h"

char Array[93][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
"sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
"rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip", "feta", "football", "basketball", "handball", "soft", "park", "walk",
"popular", "micro", "chips", "ketchup", "mayo", "clothes", "longsleeve", "jacket", "player", "barney", "dinosaurs", "phone", "socks", "manager", "pirates",
"carribean", "spray", "hookah", "chair", "sofa", "basket", "apple", "banana", "orange", "peach", "strawberry", "blue", "pink", "yellow", "brown", "black",
"white", "shadow", "jumbo", "public", "private", "glass", "plastic", "balcony", "floor", "plug", "piano", "electricity", "carbon", "bars", "portrait" }; 


const void destroy_entries(Entry entry){
    list_destroy(get_entry_payload(entry), NULL);
    free(entry);
}

void test_create(void) {
    Index index = create_index(MT_EDIT_DIST, (CompareFunc)compare_entries, 0);

    TEST_ASSERT(index != NULL);

    destroy_entry_index(index);
}


void test_build(void) {
    Index index = create_index(MT_EDIT_DIST, (CompareFunc)compare_entries, 0);

    TEST_ASSERT(index != NULL);

    EntryList entrylist = create_entry_list((CompareFunc)compare_entries);

    int N = 20;

    for (int i = 0; i < N; i++) {
        Entry entry = create_entry(Array[i], NULL);
        add_entry(entrylist, entry);
    }



    ErrorCode errcode = build_entry_index(index, entrylist);

    TEST_ASSERT(size_index(index) == N);
    TEST_ASSERT(errcode == EC_SUCCESS);

    for (int i = 0; i < N; i++) {
        EntryList result = create_entry_list((CompareFunc)compare_entries);
        
        lookup_entry_index(index, Array[i], 0, result, (CompareFunc)compare_queries);

        Entry entry = create_entry(strdup(Array[i]), NULL);

        Entry existentry = find_entry(result, entry);

        TEST_ASSERT(existentry != NULL);

        destroy_entry(entry);
        destroy_entry_list(result, NULL);

    }

    destroy_entry_list(entrylist, (DestroyFunc)destroy_entries);

    destroy_entry_index(index);


}


TEST_LIST = {

	{ "index_create", test_create },
    { "index_build", test_build },
    // { "bktree_insert_hamming", test_insert_hamming },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
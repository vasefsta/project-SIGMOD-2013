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


void test_create(void) {
    Index index = create_index(MT_EDIT_DIST, (CompareFunc)compare_entries, 0);

    TEST_ASSERT(index != NULL);
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



    build_entry_index(index, entrylist);

    TEST_ASSERT(size_index(index) == N);

    

}


TEST_LIST = {

	{ "index_create", test_create },
    { "index_build", test_build },
    // { "bktree_insert_hamming", test_insert_hamming },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
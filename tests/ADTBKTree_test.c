#include "acutest.h"
#include "ADTBKTree.h"
#include "ADTEntryList.h"

char Array[93][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
"sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
"rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip", "feta", "football", "basketball", "handball", "soft", "park", "walk",
"popular", "micro", "chips", "ketchup", "mayo", "clothes", "longsleeve", "jacket", "player", "barney", "dinosaurs", "phone", "socks", "manager", "pirates",
"carribean", "spray", "hookah", "chair", "sofa", "basket", "apple", "banana", "orange", "peach", "strawberry", "blue", "pink", "yellow", "brown", "black",
"white", "shadow", "jumbo", "public", "private", "glass", "plastic", "balcony", "floor", "plug", "piano", "electricity", "carbon", "bars", "portrait" }; 


void test_create(){
    BKTree bktree = bk_create(MT_EDIT_DIST);

    TEST_ASSERT(bktree != NULL);

    bk_destroy(bktree, NULL);
}


void test_insert(){
    BKTree bktree = bk_create(MT_EDIT_DIST);

    TEST_ASSERT(bktree != NULL);

    int N = 50;
    Entry entriesArray[N];
    for (int i = 0; i < 50; i++) {
        entriesArray[i] = create_entry(Array[i]);

        bk_insert(bktree, entriesArray[i]);
    }

    for (int i = 0; i < 50; i++) {
        BKNode result = bk_find(bktree, entriesArray[i]);
        TEST_ASSERT(result != NULL);
    }
}



TEST_LIST = {

	{ "bktree_create", test_create },
    { "bktree_insert", test_insert },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
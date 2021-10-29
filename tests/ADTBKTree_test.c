#include "acutest.h"
#include "ADTBKTree.h"
#include "ADTEntryList.h"

char Array[93][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
"sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
"rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip", "feta", "football", "basketball", "handball", "soft", "park", "walk",
"popular", "micro", "chips", "ketchup", "mayo", "clothes", "longsleeve", "jacket", "player", "barney", "dinosaurs", "phone", "socks", "manager", "pirates",
"carribean", "spray", "hookah", "chair", "sofa", "basket", "apple", "banana", "orange", "peach", "strawberry", "blue", "pink", "yellow", "brown", "black",
"white", "shadow", "jumbo", "public", "private", "glass", "plastic", "balcony", "floor", "plug", "piano", "electricity", "carbon", "bars", "portrait" }; 


const void destroy_entries(Entry entry){
    list_destroy(entry->payload, NULL);
    free(entry);
}


void test_create(){
    BKTree bktree = bk_create(MT_EDIT_DIST);

    TEST_ASSERT(bktree != NULL);

    bk_destroy(bktree, NULL);
}


void test_insert_edit(){
    BKTree bktree = bk_create(MT_EDIT_DIST);

    TEST_ASSERT(bktree != NULL);

    int N = 50;
    Entry entriesArray[N];
    for (int i = 0; i < N; i++) {
        entriesArray[i] = create_entry(Array[i]);

        bk_insert(bktree, entriesArray[i]);
    }

    EntryList entrylist = create_entry_list();

    int threshold = 0;

    Entry entry = create_entry("guitar");

    bk_find(bktree, entry, entrylist, threshold);

    printf("%d num of entries \n", get_number_entries(entrylist));

    bk_destroy(bktree, (DestroyFunc) destroy_entries);

    list_destroy(entrylist, NULL);

    list_destroy(entry->payload, NULL);

    free(entry);

}

void test_insert_hamming(){
    BKTree bktree = bk_create(MT_EDIT_DIST);

    TEST_ASSERT(bktree != NULL);

    int N = 50;
    Entry entriesArray[N];
    for (int i = 0; i < N; i++) {
        entriesArray[i] = create_entry(Array[i]);

        bk_insert(bktree, entriesArray[i]);
    }

    EntryList entrylist = create_entry_list();

    int threshold = 0;

    Entry entry = create_entry("guitar");

    bk_find(bktree, entry, entrylist, threshold);

    printf("%d num of entries \n", get_number_entries(entrylist));

    bk_destroy(bktree, (DestroyFunc) destroy_entries);

    list_destroy(entrylist, NULL);

    list_destroy(entry->payload, NULL);

    free(entry);

}



TEST_LIST = {

	{ "bktree_create", test_create },
    { "bktree_insert_edit", test_insert_edit },
    { "bktree_insert_hamming", test_insert_hamming },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
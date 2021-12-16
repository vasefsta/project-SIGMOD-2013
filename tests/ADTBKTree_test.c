/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTBKTree_test.c     
*/


#include "acutest.h"
#include "ADTBKTree.h"
#include "ADTEntryList.h"


// holds words that will be store in entries
char Array[93][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
"sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
"rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip", "feta", "football", "basketball", "handball", "soft", "park", "walk",
"popular", "micro", "chips", "ketchup", "mayo", "clothes", "longsleeve", "jacket", "player", "barney", "dinosaurs", "phone", "socks", "manager", "pirates",
"carribean", "spray", "hookah", "chair", "sofa", "basket", "apple", "banana", "orange", "peach", "strawberry", "blue", "pink", "yellow", "brown", "black",
"white", "shadow", "jumbo", "public", "private", "glass", "plastic", "balcony", "floor", "plug", "piano", "electricity", "carbon", "bars", "portrait" }; 

char Hamming_Array[12][7] = {"flower","coffee","summer","autumn","winter","spring","diving","sotira","poetry",
"forest","guitar","simple"};


// Compares two entries
int compare_entry(Entry e1, Entry e2){
    return (strcmp(e1->word, e2->word));
}

// Compares two queries
int compare_query(QueryID* q1, QueryID* q2) {
    return *q1 - *q2;
}

// Destroy an entry
const void destroy_entry(Entry entry){
    list_destroy(entry->payload, NULL);
    free(entry);
}


void test_create() {
    BKTree bktree = bk_create(MT_EDIT_DIST);        // creates an empty bktree

    TEST_ASSERT(bktree != NULL);                    // checks if bktree is created

    bk_destroy(bktree, NULL);       // destroy bktree
}


// Initialiaze bktree with the first 50 entries from Array
// and then checks if all of them are exist in the bktree.
//   
void test_insert_edit() {
    BKTree bktree = bk_create(MT_EDIT_DIST);    // creates an empty bktree with matchtype MT_EDIT_DIST

    TEST_ASSERT(bktree != NULL);        // test if bktree is created

    int N = 50;     // size of entriesArray and bktree
    Entry entriesArray[N];      // holds entries

     
    for (int i = 0; i < N; i++) {
        entriesArray[i] = create_entry(Array[i], NULL); // initialiaze entriesArray with new entries
        bk_insert(bktree, entriesArray[i]);     // insert an entry in bktree
    }

    for (int i = 0; i < N; i++) {
        
        Entry entry = bk_find_entry(bktree, entriesArray[i]->word);     // search for entry which has got word 
                                                                        // equal to entriesArray[i]->word
        
        TEST_ASSERT(entry != NULL);         // checks if entry exists in bktree  

        int res = compare_entry(entry, entriesArray[i]);        

        TEST_ASSERT(res == 0);      // checks if entries are equal
    }

    bk_destroy(bktree, (DestroyFunc) destroy_entry);    // destroy bktree 
}

// void test_insert_hamming(){
//     BKTree bktree = bk_create(MT_HAMMING_DIST);

//     TEST_ASSERT(bktree != NULL);                                        // Test if bktree is created

//     int N = 50;
//     Entry entriesArray[N];
//     for (int i = 0; i < N; i++) {
//         entriesArray[i] = create_entry(Array[i], NULL);

//         bk_insert(bktree, entriesArray[i]);
//     }

//     int threshold = 0;

//     for (int i = 0; i < N; i++) {
//         EntryList entrylist = create_entry_list((CompareFunc)compare_entry);
        
//         bk_find(bktree, entrylist, (CompareFunc) compare_query, entriesArray[i]->word, threshold);

//         TEST_ASSERT(get_number_entries(entrylist) == 1);                // Test if entry was inserted in entrylist

//         Entry entry = find_entry(entrylist, entriesArray[i]);

//         TEST_ASSERT(entry != NULL);                                     // Test if it was the right entry

//         list_destroy(entrylist, NULL);
//     }


//     Entry entry = create_entry("guitar", NULL);       // mas emfanise tis lexeis me mikos 6 pou exoun threshold =20 apo thn guitar
//     threshold = 20;

//     EntryList entrylist = create_entry_list((CompareFunc)compare_entry);

//     bk_find(bktree, entrylist, (CompareFunc) compare_query, entry->word, threshold);

//     // DIORTHOMA
//     Entry entriesArrayHamming[N];

//     for (int i = 0; i < 12; i++) {

//         entriesArrayHamming[i] = create_entry(Hamming_Array[i], NULL);

//         Entry entry = find_entry(entrylist, entriesArrayHamming[i]);

//         TEST_ASSERT(entry != NULL);                                 //  Test if entry was found in entrylist

//         destroy_entry(entriesArrayHamming[i]);
//     }

//     TEST_ASSERT(get_number_entries(entrylist) == 12);               // Test if entries were exactly 12 (Calculated from array before)

//     bk_destroy(bktree, (DestroyFunc) destroy_entry);

//     list_destroy(entrylist, NULL);

//     list_destroy(entry->payload, NULL);

//     free(entry);
// }



TEST_LIST = {

	{ "bktree_create", test_create },
    { "bktree_insert_edit", test_insert_edit },
    // { "bktree_insert_hamming", test_insert_hamming },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
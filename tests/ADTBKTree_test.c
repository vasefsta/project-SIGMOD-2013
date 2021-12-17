/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTBKTree_test.c     
*/


#include "acutest.h"
#include "ADTBKTree.h"
#include "ADTEntryList.h"
#include "misc.h"


// holds words that will be store in entries
char Array[93][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
"sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
"rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip", "feta", "football", "basketball", "handball", "soft", "park", "walk",
"popular", "micro", "chips", "ketchup", "mayo", "clothes", "longsleeve", "jacket", "player", "barney", "dinosaurs", "phone", "socks", "manager", "pirates",
"carribean", "spray", "hookah", "chair", "sofa", "basket", "apple", "banana", "orange", "peach", "strawberry", "blue", "pink", "yellow", "brown", "black",
"white", "shadow", "jumbo", "public", "private", "glass", "plastic", "balcony", "floor", "plug", "piano", "electricity", "carbon", "bars", "portrait" }; 

// char Queries[][] = {}
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

// Compare function for specials.
int compare_special(Special s1, Special s2){
    return s1->query->queryID - s2->query->queryID;
}



void test_create() {
    BKTree bktree = bk_create(MT_EDIT_DIST);        // creates an empty bktree

    TEST_ASSERT(bktree != NULL);                    // checks if bktree is created

    bk_destroy(bktree, NULL);       // destroy bktree
}


// Initialiaze bktree with the first 50 entries from Array
// and then checks if all of them are exist in the bktree.
//   
void test_insert() {

/////////////////////////////////// checking bktree with matchtype = MT_HAMMING_DIST //////////////////////////////
    
    BKTree bktree = bk_create(MT_EDIT_DIST);    // creates an empty bktree 

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


    //////////////////////////// checking bktree with matchtype = MT_HAMMING_DIST //////////////////////////////

    bktree = bk_create(MT_HAMMING_DIST);    // creates an empty bktree 

    TEST_ASSERT(bktree != NULL);        // test if bktree is created

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

void test_find(void) {
    BKTree bktree = bk_create(MT_EDIT_DIST);    // creates an empty bktree 

    TEST_ASSERT(bktree != NULL);        // test if bktree is created

    // Create queries.
    Query query1 = convert_to_query("watemelon");
    query1->queryID = 1;
    query1->length = 1;
    query1->match_dist = 0;
    query1->match_type = MT_EDIT_DIST;


    Query query2 = convert_to_query("table fire");
    query2->queryID = 2;
    query2->length = 2;
    query2->match_dist = 0;
    query2->match_type = MT_HAMMING_DIST;

    
    Query query3 = convert_to_query("watermelon table fire");
    query3->queryID = 3;
    query1->length = 3;
    query1->match_dist = 0;
    query1->match_type = MT_EXACT_MATCH;

    
    Query query4 = convert_to_query("table work school");
    query4->queryID = 4;
    query1->length = 3;
    query1->match_dist = 0;
    query1->match_type = MT_EDIT_DIST;

    Entry entry1 = create_entry("watermelon", (CompareFunc)compare_query);
    Entry entry2 = create_entry("table", (CompareFunc)compare_query);
    Entry entry3 = create_entry("fire", (CompareFunc)compare_query);
    Entry entry4 = create_entry("work", (CompareFunc)compare_query);
    Entry entry5 = create_entry("school", (CompareFunc)compare_query);

    list_insert(entry1->payload, query1);
    list_insert(entry1->payload, query3);

    list_insert(entry2->payload, query2);
    list_insert(entry2->payload, query3);
    list_insert(entry2->payload, query4);

    list_insert(entry3->payload, query2);
    list_insert(entry3->payload, query3);

    list_insert(entry4->payload, query4);

    list_insert(entry5->payload, query4);


    // Create structures that help bk_find work.
    Map map_result = map_create((CompareFunc) compare_special, 20);
    List complete_list = list_create((CompareFunc)compare_query);


    bk_destroy(bktree, (DestroyFunc) destroy_entry);    // destroy bktree 

}

TEST_LIST = {

	{ "bktree_create", test_create },
    { "bktree_insert", test_insert },
    { "bktree_find", test_find },
    // { "bktree_insert_hamming", test_insert_hamming },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
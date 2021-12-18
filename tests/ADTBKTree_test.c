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

// Hash fucntion for Query.
int hash_queries(Query query){
    return query->queryID;
}

// Hash fucntion for Special.
int hash_special(Special special){
    return special->query->queryID;
}

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

const void destroy_query(Query query) {
    free(query->words);
    free(query);
}

const void destroy_special(Special special) {
    list_destroy(special->words, NULL);
    free(special);

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

void test_bk_find(void) {

    // Create queries.
    String s = strdup("watemelon");
    Query query1 = convert_to_query(s);
    query1->queryID = 1;
    query1->length = 1;
    query1->match_dist = 0;
    query1->match_type = MT_EDIT_DIST;

    free(s);
    s = strdup("table fire");
    Query query2 = convert_to_query(s);
    query2->queryID = 2;
    query2->length = 2;
    query2->match_dist = 0;
    query2->match_type = MT_HAMMING_DIST;

    free(s);
    s = strdup("watermelon table fire");
    Query query3 = convert_to_query(s);
    query3->queryID = 3;
    query3->length = 3;
    query3->match_dist = 0;
    query3->match_type = MT_EXACT_MATCH;

    free(s);
    s = strdup("table work school");    
    Query query4 = convert_to_query(s);
    query4->queryID = 4;
    query4->length = 3;
    query4->match_dist = 0;
    query4->match_type = MT_EDIT_DIST;
    free(s);

    Map map_queries = map_create((CompareFunc)compare_query, 10);
    map_set_hash_function(map_queries, (HashFunc) hash_queries);
    map_insert(map_queries, query1);
    map_insert(map_queries, query2);
    map_insert(map_queries, query3);
    map_insert(map_queries, query4);


    // Create the entries for entrylist.
    Entry entry1 = create_entry("watermelon", (CompareFunc)compare_query);
    Entry entry2 = create_entry("table", (CompareFunc)compare_query);
    Entry entry3 = create_entry("fire", (CompareFunc)compare_query);
    Entry entry4 = create_entry("work", (CompareFunc)compare_query);
    Entry entry5 = create_entry("school", (CompareFunc)compare_query);

    // Insert queries to entries.
    list_insert(entry1->payload, query1);
    list_insert(entry1->payload, query3);

    list_insert(entry2->payload, query2);
    list_insert(entry2->payload, query3);
    list_insert(entry2->payload, query4);

    list_insert(entry3->payload, query2);
    list_insert(entry3->payload, query3);

    list_insert(entry4->payload, query4);

    list_insert(entry5->payload, query4);

    // Create the entrylist to build bktree.
    EntryList entrlylist = create_entry_list((CompareFunc)compare_entry);
    add_entry(entrlylist, entry1);
    add_entry(entrlylist, entry2);
    add_entry(entrlylist, entry3);
    add_entry(entrlylist, entry4);
    add_entry(entrlylist, entry5);

    // Create structures that help bk_find work.
    Map map_result = map_create((CompareFunc) compare_special, 20);
    map_set_hash_function(map_result, (HashFunc)hash_special);
    List complete_list = list_create((CompareFunc)compare_query);

    // Create an empty bktree  
    BKTree bktree = bk_create(MT_EDIT_DIST);     
    TEST_ASSERT(bktree != NULL);        // Test if bktree is created

    // Insert entries to bk tree.
    bk_insert(bktree, entry1);
    bk_insert(bktree, entry2);
    bk_insert(bktree, entry3);
    bk_insert(bktree, entry4);
    bk_insert(bktree, entry5);

    // We seperate the sentence "watermelon table fire waork school"
    // and for every word we check if we have any new complete queries.

    // Query 1 should be added.
    bk_find(bktree, map_result, complete_list, (CompareFunc)compare_query, "watermelon", 0);
    TEST_ASSERT(list_size(complete_list) == 1);
    ListNode listnode = list_first(complete_list);
    QueryID *qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);

    // No Query should be added.
    bk_find(bktree, map_result, complete_list, (CompareFunc)compare_query, "table", 0);
    TEST_ASSERT(list_size(complete_list) == 1);
    listnode = list_first(complete_list);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);

    // Queries 2 and 3 should be added.
    // Since we insert a new element at the front of the list the first element
    // must be 2 and after that should be 3 now.
    bk_find(bktree, map_result, complete_list, (CompareFunc)compare_query, "fire", 0);
    TEST_ASSERT(list_size(complete_list) == 3);
    listnode = list_first(complete_list);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 2);
    listnode = list_find_next(listnode);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 3);
    listnode = list_find_next(listnode);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);

    
    // No query should be added.
    bk_find(bktree, map_result, complete_list, (CompareFunc)compare_query, "work", 0);
    TEST_ASSERT(list_size(complete_list) == 3);
    listnode = list_first(complete_list);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 2);
    listnode = list_find_next(listnode);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 3);
    listnode = list_find_next(listnode);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);
    
    // Query 4 should be added.
    // Since we insert a new element at the front of the list the first element
    // must be 4 now
    bk_find(bktree, map_result, complete_list, (CompareFunc)compare_query, "school", 0);
    TEST_ASSERT(list_size(complete_list) == 4);
    listnode = list_first(complete_list);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 4);
    listnode = list_find_next(listnode);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 2);
    listnode = list_find_next(listnode);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 3);
    listnode = list_find_next(listnode);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);


    // Free allocated memory.
    bk_destroy(bktree, (DestroyFunc) destroy_entry);
    map_destroy(map_result, (DestroyFunc)destroy_special);
    map_destroy(map_queries, (DestroyFunc)destroy_query);
    list_destroy(complete_list, (DestroyFunc)free);
    destroy_entry_list(entrlylist, (DestroyFunc)NULL);
}

void test_bk_find_entry(void){
    // Create an empty bktree;
    BKTree bktree = bk_create(MT_EDIT_DIST);     
    TEST_ASSERT(bktree != NULL);        // Test if bktree is created.

     // Create queries.
    String s = strdup("watemelon");
    Query query1 = convert_to_query(s);
    query1->queryID = 1;
    query1->length = 1;
    query1->match_dist = 0;
    query1->match_type = MT_EDIT_DIST;

    free(s);
    s = strdup("table fire");
    Query query2 = convert_to_query(s);
    query2->queryID = 2;
    query2->length = 2;
    query2->match_dist = 0;
    query2->match_type = MT_HAMMING_DIST;

    free(s);
    s = strdup("watermelon table fire");
    Query query3 = convert_to_query(s);
    query3->queryID = 3;
    query3->length = 3;
    query3->match_dist = 0;
    query3->match_type = MT_EXACT_MATCH;

    free(s);
    s = strdup("table work school");    
    Query query4 = convert_to_query(s);
    query4->queryID = 4;
    query4->length = 3;
    query4->match_dist = 0;
    query4->match_type = MT_EDIT_DIST;
    free(s);

    Map map_queries = map_create((CompareFunc)compare_query, 10);
    map_set_hash_function(map_queries, (HashFunc) hash_queries);
    map_insert(map_queries, query1);
    map_insert(map_queries, query2);
    map_insert(map_queries, query3);
    map_insert(map_queries, query4);


    // Create the entries for entrylist.
    Entry entry1 = create_entry("watermelon", (CompareFunc)compare_query);
    Entry entry2 = create_entry("table", (CompareFunc)compare_query);
    Entry entry3 = create_entry("fire", (CompareFunc)compare_query);
    Entry entry4 = create_entry("work", (CompareFunc)compare_query);
    Entry entry5 = create_entry("school", (CompareFunc)compare_query);

    // Insert queries to entries.
    list_insert(entry1->payload, query1);
    list_insert(entry1->payload, query3);

    list_insert(entry2->payload, query2);
    list_insert(entry2->payload, query3);
    list_insert(entry2->payload, query4);

    list_insert(entry3->payload, query2);
    list_insert(entry3->payload, query3);

    list_insert(entry4->payload, query4);

    list_insert(entry5->payload, query4);

    // Create the entrylist to build bktree.
    EntryList entrlylist = create_entry_list((CompareFunc)compare_entry);
    add_entry(entrlylist, entry1);
    add_entry(entrlylist, entry2);
    add_entry(entrlylist, entry3);
    add_entry(entrlylist, entry4);
    add_entry(entrlylist, entry5);

    // Insert entries to bk tree.
    bk_insert(bktree, entry1);
    bk_insert(bktree, entry2);
    bk_insert(bktree, entry3);
    bk_insert(bktree, entry4);
    bk_insert(bktree, entry5);

    // Check if entry with word "watermelon" exists in bktree.
    Entry entry = bk_find_entry(bktree, "watermelon");
    TEST_ASSERT(entry != NULL);
    TEST_ASSERT(list_size(entry->payload) == 2);
    ListNode listnode = list_first(entry->payload);
    Query q = list_node_value(listnode);
    TEST_ASSERT(q->queryID == 3);
    listnode = list_find_next(listnode);
    q = list_node_value(listnode);
    TEST_ASSERT(q->queryID == 1);

    // Check if entry with word "table" exists in bktree.
    entry = bk_find_entry(bktree, "table");
    TEST_ASSERT(entry != NULL);
    TEST_ASSERT(list_size(entry->payload) == 3);
    listnode = list_first(entry->payload);
    q = list_node_value(listnode);
    TEST_ASSERT(q->queryID == 4);
    listnode = list_find_next(listnode);
    q = list_node_value(listnode);
    TEST_ASSERT(q->queryID == 3);
    listnode = list_find_next(listnode);
    q = list_node_value(listnode);
    TEST_ASSERT(q->queryID == 2);
    
    // Check if entry with word "fire" exists in bktree.
    entry = bk_find_entry(bktree, "fire");
    TEST_ASSERT(entry != NULL);
    TEST_ASSERT(list_size(entry->payload) == 2);
    listnode = list_first(entry->payload);
    q = list_node_value(listnode);
    TEST_ASSERT(q->queryID == 3);
    listnode = list_find_next(listnode);
    q = list_node_value(listnode);
    TEST_ASSERT(q->queryID == 2);

    // Check if entry with word "work" exists in bktree.
    entry = bk_find_entry(bktree, "work");
    TEST_ASSERT(entry != NULL);
    TEST_ASSERT(list_size(entry->payload) == 1);
    listnode = list_first(entry->payload);
    q = list_node_value(listnode);
    TEST_ASSERT(q->queryID == 4);


    // Check if entry with word "school" exists in bktree.
    entry = bk_find_entry(bktree, "school");
    TEST_ASSERT(entry != NULL);
    TEST_ASSERT(list_size(entry->payload) == 1);
    listnode = list_first(entry->payload);
    q = list_node_value(listnode);
    TEST_ASSERT(q->queryID == 4);

    bk_destroy(bktree, (DestroyFunc) destroy_entry);
    map_destroy(map_queries, (DestroyFunc) destroy_query);
    destroy_entry_list(entrlylist, (DestroyFunc) NULL);
}

TEST_LIST = {

	{ "bktree_create", test_create },
    { "bktree_insert", test_insert },
    { "bktree_find",  test_bk_find },
    { "bktree_find_entry",  test_bk_find_entry },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
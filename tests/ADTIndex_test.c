#include <stdlib.h>
#include <string.h>

#include "acutest.h"
#include "ADTIndex.h"
#include "ADTEntryList.h"
#include "misc.h"
#include "ADTBKTree.h"

char Array[93][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
"sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
"rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip", "feta", "football", "basketball", "handball", "soft", "park", "walk",
"popular", "micro", "chips", "ketchup", "mayo", "clothes", "longsleeve", "jacket", "player", "barney", "dinosaurs", "phone", "socks", "manager", "pirates",
"carribean", "spray", "hookah", "chair", "sofa", "basket", "apple", "banana", "orange", "peach", "strawberry", "blue", "pink", "yellow", "brown", "black",
"white", "shadow", "jumbo", "public", "private", "glass", "plastic", "balcony", "floor", "plug", "piano", "electricity", "carbon", "bars", "portrait" }; 


// Hash function for special.
int hash_special(Special s1){
    return s1->query->queryID;
}

// Hash function for query.
int hash_queries(Query query){
    return query->queryID;
}

// Compare function for query ids.
int compare_query(QueryID* q1, QueryID* q2) {                           
    return *q1 - *q2;
}

// Compare function for specials.
int compare_special(Special s1, Special s2){
    return s1->query->queryID - s2->query->queryID;
}


//Compare function for entries.
int compare_entry(Entry e1, Entry e2){
    return (strcmp(e1->word, e2->word));
}

// Destroy function for entries.
const void destroy_entry_payload(Entry entry){
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


// Destroy function for entries.
const void destroy_entry(Entry entry) {
    list_destroy(entry->payload, NULL);
    free(entry);
}


void test_create(void) {
    // Create an index of type MT_EDIT_DIST.
    Index index = create_index(MT_EDIT_DIST, (CompareFunc)compare_entry, 0);

    // Test if index is created.
    TEST_ASSERT(index != NULL);

    // Free allocated memory.
    destroy_entry_index(index, NULL);
}


// Test for build_lookup for MT_EXACT_MATCH.
void test_build_lookup_exact(void) {

    // Create an index of type MT_EXACT_MATCH.
    Index index = create_index(MT_EXACT_MATCH, (CompareFunc)compare_entry, 20);

    // Test if index is created.
    TEST_ASSERT(index != NULL);

    // Create an empty EntryList.
    EntryList entrylist = create_entry_list((CompareFunc)compare_entry);

    // Add 20 entries with words from Array in the EntryList.
    int N = 20;
    for (int i = 0; i < N; i++) {
        Entry entry = create_entry(Array[i], NULL);
        add_entry(entrylist, entry);
    }
    // Build index.
    ErrorCode errcode = build_entry_index(index, entrylist);

    // Test index size.
    TEST_ASSERT(size_index(index) == N);
    
    // Test error code returned from function.
    TEST_ASSERT(errcode == EC_SUCCESS);

    // Get the index->index.
    Map map_index = index_index(index);

    for (int i = 0; i < N; i++){
        // Create a dummy entry with word = Array[i].
        struct entry dummyEntry;
        dummyEntry.word = Array[i];
        // Check if Entry with word = Array[i] exists in map_index.
        MapNode node = map_find(map_index, &dummyEntry);
        
        // Test if an entry was found
        TEST_ASSERT(node != NULL);

        // Get the found entry and test if 
        // dummyString is the correct word.
        String dummyString = map_node_value(node);
        TEST_ASSERT(dummyString != NULL);
        TEST_ASSERT(strcmp(dummyString, Array[i]) == 0);

    }

    //Free allocated memory.
    destroy_entry_list(entrylist, (DestroyFunc)destroy_entry_payload);
    destroy_entry_index(index, NULL);


}


// Test for build_lookup for MT_EDIT_DIST.
void test_build_lookup_edit(void) {

    // Create an index of type MT_EDIT_DIST.
    Index index = create_index(MT_EDIT_DIST, (CompareFunc)compare_entry, 20);

    // Test if index is created.
    TEST_ASSERT(index != NULL);

    // Create an empty EntryList.
    EntryList entrylist = create_entry_list((CompareFunc)compare_entry);

    // Add 20 entries with words from Array in the EntryList.
    int N = 20;
    for (int i = 0; i < N; i++) {
        Entry entry = create_entry(Array[i], NULL);
        add_entry(entrylist, entry);
    }
    // Build index.
    ErrorCode errcode = build_entry_index(index, entrylist);

    // Test index size.
    TEST_ASSERT(size_index(index) == N);
    
    // Test error code returned from function.
    TEST_ASSERT(errcode == EC_SUCCESS);

    // Get the index->index.
    BKTree tree = index_index(index);

    for (int i = 0; i < N; i++){
        // Check if this string is in tree.
        Entry dummyEntry = bk_find_entry(tree, Array[i]);

        // Test if dummyString exists and is the correct word.
        TEST_ASSERT(dummyEntry != NULL);
        TEST_ASSERT(strcmp(dummyEntry->word, Array[i]) == 0);
    }

    // Free allocated memory.
    destroy_entry_list(entrylist, (DestroyFunc)destroy_entry_payload);
    destroy_entry_index(index, NULL);

}


// Test for build_lookup for MT_HAMMING_DIST.
void test_build_lookup_hamming(void) {

    // Create an index of type MT_HAMMING_DIST.
    Index index = create_index(MT_HAMMING_DIST, (CompareFunc)compare_entry, 20);

    // Test if index is created.
    TEST_ASSERT(index != NULL);

    // Create an empty EntryList.
    EntryList entrylist = create_entry_list((CompareFunc)compare_entry);

    // Add 20 entries with words from Array in the EntryList.
    int N = 20;
    for (int i = 0; i < N; i++) {
        Entry entry = create_entry(Array[i], NULL);
        add_entry(entrylist, entry);
    }
    // Build index.
    ErrorCode errcode = build_entry_index(index, entrylist);

    // Test index size.
    TEST_ASSERT(size_index(index) == N);
    
    // Test error code returned from function.
    TEST_ASSERT(errcode == EC_SUCCESS);

    // Get the index->index.
    BKTree tree = index_index(index);

    for (int i = 0; i < N; i++){
        // Check if this string is in tree.
        Entry dummyEntry = bk_find_entry(tree, Array[i]);

        // Test if dummyString exists and is the correct word.
        TEST_ASSERT(dummyEntry != NULL);
        TEST_ASSERT(strcmp(dummyEntry->word, Array[i]) == 0);
    }

    // Free allocated memory.
    destroy_entry_list(entrylist, (DestroyFunc)destroy_entry_payload);
    destroy_entry_index(index, NULL);

}

// Test for build_lookup for MT_EXACT_MATCH.
void test_lookup_entry_index_exact(void) {

    // Create an index of type MT_EXACT_MATCH.
    Index index = create_index(MT_EXACT_MATCH, (CompareFunc)compare_entry, 20);

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

    build_entry_index(index, entrlylist);

    // Create structures that help bk_find work.
    Map map_result = map_create((CompareFunc) compare_special, 20);
    map_set_hash_function(map_result, (HashFunc)hash_special);
    List complete_list = list_create((CompareFunc)compare_query);

    lookup_entry_index(index, "watermelon", 0, map_result, complete_list, (CompareFunc)compare_query);
    TEST_ASSERT(list_size(complete_list) == 1);
    ListNode listnode = list_first(complete_list);
    QueryID *qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);

     // No Query should be added.
    lookup_entry_index(index, "table", 0, map_result, complete_list, (CompareFunc)compare_query);
    TEST_ASSERT(list_size(complete_list) == 1);
    listnode = list_first(complete_list);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);

    // Queries 2 and 3 should be added.
    // Since we insert a new element at the front of the list the first element
    // must be 2 and after that should be 3 now.
    lookup_entry_index(index, "fire", 0, map_result, complete_list, (CompareFunc)compare_query);
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
    lookup_entry_index(index, "work", 0, map_result, complete_list, (CompareFunc)compare_query);
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
    lookup_entry_index(index, "school", 0, map_result, complete_list, (CompareFunc)compare_query);
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
    destroy_entry_index(index, (DestroyFunc) destroy_entry);
    map_destroy(map_result, (DestroyFunc)destroy_special);
    map_destroy(map_queries, (DestroyFunc)destroy_query);
    list_destroy(complete_list, (DestroyFunc)free);
    destroy_entry_list(entrlylist, (DestroyFunc)NULL);
}

// Test for build_lookup for MT_EXACT_MATCH.
void test_lookup_entry_index_edit(void) {

    // Create an index of type MT_EXACT_MATCH.
    Index index = create_index(MT_EDIT_DIST, (CompareFunc)compare_entry, 20);

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

    build_entry_index(index, entrlylist);

    // Create structures that help bk_find work.
    Map map_result = map_create((CompareFunc) compare_special, 20);
    map_set_hash_function(map_result, (HashFunc)hash_special);
    List complete_list = list_create((CompareFunc)compare_query);

    lookup_entry_index(index, "watermelon", 0, map_result, complete_list, (CompareFunc)compare_query);
    TEST_ASSERT(list_size(complete_list) == 1);
    ListNode listnode = list_first(complete_list);
    QueryID *qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);

     // No Query should be added.
    lookup_entry_index(index, "table", 0, map_result, complete_list, (CompareFunc)compare_query);
    TEST_ASSERT(list_size(complete_list) == 1);
    listnode = list_first(complete_list);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);

    // Queries 2 and 3 should be added.
    // Since we insert a new element at the front of the list the first element
    // must be 2 and after that should be 3 now.
    lookup_entry_index(index, "fire", 0, map_result, complete_list, (CompareFunc)compare_query);
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
    lookup_entry_index(index, "work", 0, map_result, complete_list, (CompareFunc)compare_query);
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
    lookup_entry_index(index, "school", 0, map_result, complete_list, (CompareFunc)compare_query);
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
    destroy_entry_index(index, (DestroyFunc) destroy_entry);
    map_destroy(map_result, (DestroyFunc)destroy_special);
    map_destroy(map_queries, (DestroyFunc)destroy_query);
    list_destroy(complete_list, (DestroyFunc)free);
    destroy_entry_list(entrlylist, (DestroyFunc)NULL);
}

// Test for build_lookup for MT_EXACT_MATCH.
void test_lookup_entry_index_hamming(void) {

    // Create an index of type MT_EXACT_MATCH.
    Index index = create_index(MT_HAMMING_DIST, (CompareFunc)compare_entry, 20);

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

    build_entry_index(index, entrlylist);

    // Create structures that help bk_find work.
    Map map_result = map_create((CompareFunc) compare_special, 20);
    map_set_hash_function(map_result, (HashFunc)hash_special);
    List complete_list = list_create((CompareFunc)compare_query);

    lookup_entry_index(index, "watermelon", 0, map_result, complete_list, (CompareFunc)compare_query);
    TEST_ASSERT(list_size(complete_list) == 1);
    ListNode listnode = list_first(complete_list);
    QueryID *qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);

     // No Query should be added.
    lookup_entry_index(index, "table", 0, map_result, complete_list, (CompareFunc)compare_query);
    TEST_ASSERT(list_size(complete_list) == 1);
    listnode = list_first(complete_list);
    qid = list_node_value(listnode);
    TEST_ASSERT(*qid == 1);
    listnode = list_find_next(listnode);
    TEST_ASSERT(listnode == NULL);

    // Queries 2 and 3 should be added.
    // Since we insert a new element at the front of the list the first element
    // must be 2 and after that should be 3 now.
    lookup_entry_index(index, "fire", 0, map_result, complete_list, (CompareFunc)compare_query);
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
    lookup_entry_index(index, "work", 0, map_result, complete_list, (CompareFunc)compare_query);
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
    lookup_entry_index(index, "school", 0, map_result, complete_list, (CompareFunc)compare_query);
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
    destroy_entry_index(index, (DestroyFunc) destroy_entry);
    map_destroy(map_result, (DestroyFunc)destroy_special);
    map_destroy(map_queries, (DestroyFunc)destroy_query);
    list_destroy(complete_list, (DestroyFunc)free);
    destroy_entry_list(entrlylist, (DestroyFunc)NULL);
}

TEST_LIST = {

	{ "index_create", test_create },
    { "index_build_lookup_exact", test_build_lookup_exact },
    { "index_build_lookup_edit", test_build_lookup_edit },
    { "index_build_lookup_hamming", test_build_lookup_hamming },
    { "index_lookup_entry_index_exact", test_lookup_entry_index_exact },
    { "index_lookup_entry_index_edit", test_lookup_entry_index_edit },
    { "index_lookup_entry_index_hamming", test_lookup_entry_index_hamming },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
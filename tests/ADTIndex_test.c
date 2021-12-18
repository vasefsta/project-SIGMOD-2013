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

// Destroy function for entries.
const void destroy_entry(Entry entry) {
    free(entry->word);                              
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

    // Free allocated memory.
    destroy_entry_list(entrylist, (DestroyFunc)destroy_entry_payload);
    destroy_entry_index(index, NULL);


}

TEST_LIST = {

	{ "index_create", test_create },
    { "index_build_lookup_exact", test_build_lookup_exact },
    { "index_build_lookup_edit", test_build_lookup_edit },
    { "index_build_lookup_hamming", test_build_lookup_hamming },
    { "index_lookup_entry_index", test_lookup_entry_index_exact },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
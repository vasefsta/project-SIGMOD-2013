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


int compare_query(QueryID* q1, QueryID* q2) {
    return *q1 - *q2;
}


int compare_entry(Entry e1, Entry e2){
    return (strcmp(e1->word, e2->word));
}


const void destroy_entry_payload(Entry entry){
    list_destroy(entry->payload, NULL);
    free(entry);
}


const void destroy_entry(Entry entry) {
    free(entry->word);                              
    list_destroy(entry->payload, NULL);
    free(entry);
}


void test_create(void) {
    Index index = create_index(MT_EDIT_DIST, (CompareFunc)compare_entry, 0);

    TEST_ASSERT(index != NULL);

    destroy_entry_index(index, NULL);
}


void test_build_lookup_exact(void) {
    // EXACT
    Index index = create_index(MT_EXACT_MATCH, (CompareFunc)compare_entry, 20);

    TEST_ASSERT(index != NULL);

    EntryList entrylist = create_entry_list((CompareFunc)compare_entry);

    int N = 20;

    for (int i = 0; i < N; i++) {
        Entry entry = create_entry(Array[i], NULL);
        add_entry(entrylist, entry);
    }



    ErrorCode errcode = build_entry_index(index, entrylist);

    TEST_ASSERT(size_index(index) == N);
    TEST_ASSERT(errcode == EC_SUCCESS);

    for (int i = 0; i < N; i++) {
        EntryList result = create_entry_list((CompareFunc)compare_entry);
        
        lookup_entry_index(index, Array[i], 0, result, (CompareFunc)compare_query);

        Entry entry = create_entry(strdup(Array[i]), NULL);

        Entry existentry = find_entry(result, entry);

        TEST_ASSERT(existentry != NULL);

        destroy_entry(entry);

        if(i+1 < N) {                                                       // Test for lookup entry
            entry = create_entry(strdup(Array[i+1]), NULL);
            existentry = find_entry(result, entry);
            TEST_ASSERT(existentry == NULL);
            destroy_entry(entry);
        } 
        
        destroy_entry_list(result, NULL);

    }

    destroy_entry_list(entrylist, (DestroyFunc)destroy_entry_payload);

    destroy_entry_index(index, NULL);


}


void test_build_lookup_edit(void) {
    Index index = create_index(MT_EDIT_DIST, (CompareFunc)compare_entry, 20);

    TEST_ASSERT(index != NULL);

    EntryList entrylist = create_entry_list((CompareFunc)compare_entry);

    int N = 20;

    for (int i = 0; i < N; i++) {
        Entry entry = create_entry(Array[i], NULL);
        add_entry(entrylist, entry);
    }



    ErrorCode errcode = build_entry_index(index, entrylist);

    TEST_ASSERT(size_index(index) == N);
    TEST_ASSERT(errcode == EC_SUCCESS);

    for (int i = 0; i < N; i++) {
        EntryList result = create_entry_list((CompareFunc)compare_entry);
        
        lookup_entry_index(index, Array[i], 0, result, (CompareFunc)compare_query);

        Entry entry = create_entry(strdup(Array[i]), NULL);

        Entry existentry = find_entry(result, entry);

        TEST_ASSERT(existentry != NULL);

        destroy_entry(entry);

        if(i+1 < N) {                                               // Test for lookup entry
            entry = create_entry(strdup(Array[i+1]), NULL);
            existentry = find_entry(result, entry);
            TEST_ASSERT(existentry == NULL);
            destroy_entry(entry);
        } 


        destroy_entry_list(result, NULL);

    }

   
    EntryList result = create_entry_list((CompareFunc)compare_entry);
        
    lookup_entry_index(index, Array[0], 4, result, (CompareFunc)compare_query);

    TEST_ASSERT(get_number_entries(result) == 7);

    destroy_entry_list(result, NULL);

    destroy_entry_list(entrylist, (DestroyFunc)destroy_entry_payload);

    destroy_entry_index(index, NULL);

}


void test_build_lookup_hamming(void) {
        Index index = create_index(MT_HAMMING_DIST, (CompareFunc)compare_entry, 20);

    TEST_ASSERT(index != NULL);

    EntryList entrylist = create_entry_list((CompareFunc)compare_entry);

    int N = 20;

    for (int i = 0; i < N; i++) {
        Entry entry = create_entry(Array[i], NULL);
        add_entry(entrylist, entry);
    }

    ErrorCode errcode = build_entry_index(index, entrylist);

    TEST_ASSERT(size_index(index) == N);
    TEST_ASSERT(errcode == EC_SUCCESS);

    for (int i = 0; i < N; i++) {
        EntryList result = create_entry_list((CompareFunc)compare_entry);
        
        lookup_entry_index(index, Array[i], 0, result, (CompareFunc)compare_query);

        Entry entry = create_entry(strdup(Array[i]), NULL);

        Entry existentry = find_entry(result, entry);

        TEST_ASSERT(existentry != NULL);

        destroy_entry(entry);

        if(i+1 < N) {                                                   // Test for lookup entry
            entry = create_entry(strdup(Array[i+1]), NULL);
            existentry = find_entry(result, entry);
            TEST_ASSERT(existentry == NULL);
            destroy_entry(entry);
        } 

        destroy_entry_list(result, NULL);

    }

    EntryList result = create_entry_list((CompareFunc)compare_entry);
        
    lookup_entry_index(index, Array[0], 4, result, (CompareFunc)compare_query);

    TEST_ASSERT(get_number_entries(result) == 3);

    destroy_entry_list(result, NULL);


    destroy_entry_list(entrylist, (DestroyFunc)destroy_entry_payload);

    destroy_entry_index(index, NULL);
}



TEST_LIST = {

	{ "index_create", test_create },
    { "index_build_lookup_exact", test_build_lookup_exact },
    { "index_build_lookup_exact", test_build_lookup_edit },
    { "index_build_lookup_exact", test_build_lookup_hamming },
    // { "bktree_insert_hamming", test_insert_hamming },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
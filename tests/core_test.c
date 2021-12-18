#include <stdlib.h>
#include <string.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "core_test.h"
#include "misc.h"
#include "ADTMap.h"
#include "ADTIndex.h"
#include "ADTEntryList.h"
#include "ADTBKTree.h"
#include "ADTLinkedList.h"


void test_CreateIndex(void) {
    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); 
    
    TEST_ASSERT(get_index_edit() != NULL);
    TEST_ASSERT(get_index_exact() != NULL);
    TEST_ASSERT(get_index_hamming() != NULL);
    TEST_ASSERT(get_map_queries() != NULL);

    TEST_ASSERT(DestroyIndex() == EC_SUCCESS);
}


void test_StartQuery(void) {
    
    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); 

    struct query query;

    query.words = strdup("This query looks good");
    query.length = 4;

    TEST_ASSERT(StartQuery(1, query.words, 0, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, query.words, 0, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(3, query.words, 1, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(4, query.words, 1, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(5, query.words, 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(6, query.words, 2, 0) == EC_SUCCESS);

    Map map_queries = get_map_queries();

    TEST_ASSERT(map_capacity(map_queries) == 6);

    for (int i = 1; i < 7; i++) {
        struct query tmpquery;

        tmpquery.queryID = i;
        tmpquery.words = NULL;

        Query existquery = map_find(map_queries, &tmpquery);

        TEST_ASSERT(existquery != NULL);

        TEST_ASSERT(existquery->queryID == i);
    }

    
    String* wordsarr = Seperate_sentence(&query);
    
    // check if queries are inserted in index-exact
    Map map_exact = index_index(get_index_exact());

    TEST_ASSERT(map_capacity(map_exact) == 4);

    for (int i = 0; i < 4; i++) {
        struct entry entry;

        entry.word = wordsarr[i];
        entry.payload = NULL;

        Entry existentry = map_find(map_exact, &entry);

        TEST_ASSERT(existentry != NULL);
        TEST_ASSERT(list_size(existentry->payload) == 2);

        for (int j = 1; j < 3; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res != NULL);
        }
    }


    BKTree bktree_hamming = index_index(get_index_hamming());

    TEST_ASSERT(bk_size(bktree_hamming) == 4);

    for (int i = 0; i < 4; i++) {
        Entry existentry = bk_find_entry(bktree_hamming, wordsarr[i]);

        TEST_ASSERT(existentry != NULL);

        for (int j = 3; j < 5; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res != NULL);
        }        
    }

    BKTree bktree_edit = index_index(get_index_edit());

    TEST_ASSERT(bk_size(bktree_edit) == 4);

    for (int i = 0; i < 4; i++) {
        Entry existentry = bk_find_entry(bktree_edit, wordsarr[i]);

        TEST_ASSERT(existentry != NULL);

        for (int j = 5; j < 7; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res != NULL);
        }        
    }

    for (int i = 0; i < 4; i++) {
        free(wordsarr[i]);
    }

    free(wordsarr);
    free(query.words);

    TEST_ASSERT(DestroyIndex() == EC_SUCCESS);
}

void test_EndQuery(void) {
    struct query query;

    query.words = strdup("This query looks good");
    query.length = 4;

    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); 
    
    TEST_ASSERT(StartQuery(1, query.words, 0, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, query.words, 0, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(3, query.words, 1, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(4, query.words, 1, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(5, query.words, 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(6, query.words, 2, 0) == EC_SUCCESS);

    TEST_ASSERT(EndQuery(1) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(1) == EC_NO_AVAIL_RES);

    TEST_ASSERT(EndQuery(2) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(2) == EC_NO_AVAIL_RES);

    TEST_ASSERT(EndQuery(3) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(3) == EC_NO_AVAIL_RES);

    TEST_ASSERT(EndQuery(4) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(4) == EC_NO_AVAIL_RES);

    TEST_ASSERT(EndQuery(5) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(5) == EC_NO_AVAIL_RES);

    TEST_ASSERT(EndQuery(6) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(6) == EC_NO_AVAIL_RES);

    String* wordsarr = Seperate_sentence(&query);
    
    // check if queries are inserted in index-exact
    Map map_exact = index_index(get_index_exact());

    for (int i = 0; i < 4; i++) {
        struct entry entry;

        entry.word = wordsarr[i];
        entry.payload = NULL;

        Entry existentry = map_find(map_exact, &entry);

        for (int j = 1; j < 3; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res == NULL);
        }
    }


    BKTree bktree_hamming = index_index(get_index_hamming());

    for (int i = 0; i < 4; i++) {
        Entry existentry = bk_find_entry(bktree_hamming, wordsarr[i]);

        for (int j = 3; j < 5; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res == NULL);
        }        
    }



    BKTree bktree_edit = index_index(get_index_edit());

    TEST_ASSERT(bk_size(bktree_edit) == 4);

    for (int i = 0; i < 4; i++) {
        Entry existentry = bk_find_entry(bktree_edit, wordsarr[i]);

        TEST_ASSERT(existentry != NULL);

        for (int j = 5; j < 7; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res == NULL);
        }        
    }

    for (int i = 0; i < 4; i++) {
        free(wordsarr[i]);
    }

    free(wordsarr);
    free(query.words);

    TEST_ASSERT(DestroyIndex() == EC_SUCCESS);
}

void test_MatchDocument(void) {

    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); 
    
    TEST_ASSERT(StartQuery(1, "This query looks good", 0, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, "This query looks good", 0, 0) == EC_SUCCESS);


    TEST_ASSERT(StartQuery(3, "This query looks good", 1, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(4, "This query looks good", 1, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(5, "This query looks good", 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(6, "This query looks good", 2, 0) == EC_SUCCESS);

    TEST_ASSERT(MatchDocument(1, "This were lovely query good looks") == EC_SUCCESS);

    TEST_ASSERT(DestroyIndex() == EC_SUCCESS);
}

void test_GetNextAvailRes(void) {
    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); 
    
    TEST_ASSERT(StartQuery(1, "This query looks good", 0, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, "This query looks good", 0, 0) == EC_SUCCESS);


    TEST_ASSERT(StartQuery(3, "This query looks good", 1, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(4, "This query looks good", 1, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(5, "This query looks good", 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(6, "This query looks good", 2, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(7, "Hey there! What's up?", 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(8, "Hey there! What's up?", 2, 0) == EC_SUCCESS);

    TEST_ASSERT(MatchDocument(1, "This were lovely query good looks") == EC_SUCCESS);

    unsigned int p_doc_id = 0;
    unsigned int p_num_res = 0;
    unsigned int* p_query_id = NULL;

    GetNextAvailRes(&p_doc_id, &p_num_res, &p_query_id);

    TEST_ASSERT(p_num_res == 6);

    for(int i = 0; i < p_num_res; i++){
       TEST_ASSERT(p_query_id[i] == i+1);
    }

    free(p_query_id);

    TEST_ASSERT(EndQuery(1) == EC_SUCCESS);

    TEST_ASSERT(MatchDocument(1, "This were lovely query good looks") == EC_SUCCESS);

    p_doc_id = 0;
    p_num_res = 0;
    p_query_id = NULL;

    GetNextAvailRes(&p_doc_id, &p_num_res, &p_query_id);

    TEST_ASSERT(p_num_res == 5);

    for(int i = 0; i < p_num_res; i++){
       TEST_ASSERT(p_query_id[i] == i+2);
    }

    free(p_query_id);

    
    TEST_ASSERT(DestroyIndex() == EC_SUCCESS);

}

TEST_LIST = {

	{ "CreateIndex", test_CreateIndex },
    { "StartQuery", test_StartQuery },
    { "EndQuery", test_EndQuery},
    { "MatchDocument", test_MatchDocument},
    { "GetNextAvailRes", test_GetNextAvailRes},

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
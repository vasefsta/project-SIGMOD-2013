#include <stdlib.h>
#include <string.h>

#include "acutest.h"			
#include "core_test.h"
#include "misc.h"
#include "ADTMap.h"
#include "ADTIndex.h"
#include "ADTEntryList.h"
#include "ADTBKTree.h"
#include "ADTLinkedList.h"


void test_CreateIndex(void) {
    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); // initialize all the neseccary structures
    
    TEST_ASSERT(get_index_edit() != NULL);      // tests if index for edit is created
    TEST_ASSERT(get_index_exact() != NULL);     // tests if index for exact is created
    TEST_ASSERT(get_index_hamming() != NULL);   // tests if index for hamming is created
    TEST_ASSERT(get_map_queries() != NULL);     // tests if map of queries is created

    TEST_ASSERT(DestroyIndex() == EC_SUCCESS);
}


void test_StartQuery(void) {
    
    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); // initialize all the neseccary structures

    struct query query;     // is inserted with different queryid every time

    query.words = strdup("This query looks good");
    query.length = 4;

    TEST_ASSERT(StartQuery(1, query.words, 0, 0) == EC_SUCCESS);    // matchtype = exact
    TEST_ASSERT(StartQuery(2, query.words, 0, 0) == EC_SUCCESS);    // matchtype = exact

    TEST_ASSERT(StartQuery(3, query.words, 1, 0) == EC_SUCCESS);    // matchtype = hamming
    TEST_ASSERT(StartQuery(4, query.words, 1, 0) == EC_SUCCESS);    // matchtype = hamming

    TEST_ASSERT(StartQuery(5, query.words, 2, 0) == EC_SUCCESS);    // matchtype = edit
    TEST_ASSERT(StartQuery(6, query.words, 2, 0) == EC_SUCCESS);    // matchtype = edit

    Map map_queries = get_map_queries();    // take the map with inserted queries

    TEST_ASSERT(map_capacity(map_queries) == 6);    // all the inserted queries are 6

    // tests if they are in the map of queries
    for (int i = 1; i < 7; i++) {
        struct query tmpquery;

        tmpquery.queryID = i;
        tmpquery.words = NULL;

        Query existquery = map_find(map_queries, &tmpquery);

        TEST_ASSERT(existquery != NULL);

        TEST_ASSERT(existquery->queryID == i);
    }

    
    String* wordsarr = Seperate_sentence(&query);   // seperate the query into words
    
    Map map_exact = index_index(get_index_exact());     // get the index for exact

    TEST_ASSERT(map_capacity(map_exact) == 4);      // the entries in the map_exact must be 4

    // check if queries are inserted in index-exact
    for (int i = 0; i < 4; i++) {
        struct entry entry;

        entry.word = wordsarr[i];
        entry.payload = NULL;

        Entry existentry = map_find(map_exact, &entry);

        TEST_ASSERT(existentry != NULL);
        TEST_ASSERT(list_size(existentry->payload) == 2);   // the queries in every entry must be 2

        // checks every payload to see if the 
        // right queries were inserted
        for (int j = 1; j < 3; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res != NULL);
        }
    }


    BKTree bktree_hamming = index_index(get_index_hamming());   // get the index of hamming
    
    TEST_ASSERT(bk_size(bktree_hamming) == 4);      // the entries in the bktree_hamming must be 4

    // check if queries are inserted in index-hamming
    for (int i = 0; i < 4; i++) {
        Entry existentry = bk_find_entry(bktree_hamming, wordsarr[i]);

        TEST_ASSERT(existentry != NULL);

        // checks every payload to see if the 
        // right queries were inserted
        for (int j = 3; j < 5; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res != NULL);
        }        
    }

    BKTree bktree_edit = index_index(get_index_edit());     // get the index of edit

    TEST_ASSERT(bk_size(bktree_edit) == 4);     // the entries in the bktree_edit must be 4

    // check if queries are inserted in index-edit
    for (int i = 0; i < 4; i++) {
        Entry existentry = bk_find_entry(bktree_edit, wordsarr[i]);

        TEST_ASSERT(existentry != NULL);

        // checks every payload to see if the 
        // right queries were inserted
        for (int j = 5; j < 7; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res != NULL);
        }        
    }

    // free the memory
    for (int i = 0; i < 4; i++) {
        free(wordsarr[i]);
    }

    free(wordsarr);
    free(query.words);

    TEST_ASSERT(DestroyIndex() == EC_SUCCESS);
}

// checks if the queries are removed from the payloads
void test_EndQuery(void) {
    struct query query;     // is inserted with different queryid every time

    query.words = strdup("This query looks good");
    query.length = 4;

    TEST_ASSERT(InitializeIndex() == EC_SUCCESS);   // initialize all the neseccary structures
    
    // inserts two queries in the index for exact
    TEST_ASSERT(StartQuery(1, query.words, 0, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, query.words, 0, 0) == EC_SUCCESS);

    // inserts two queries in the index for hamming
    TEST_ASSERT(StartQuery(3, query.words, 1, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(4, query.words, 1, 0) == EC_SUCCESS);

    // inserts two queries in the index for edit
    TEST_ASSERT(StartQuery(5, query.words, 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(6, query.words, 2, 0) == EC_SUCCESS);

    // removes queries from the index for exact
    TEST_ASSERT(EndQuery(1) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(1) == EC_NO_AVAIL_RES);

    TEST_ASSERT(EndQuery(2) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(2) == EC_NO_AVAIL_RES);

    // removes queries from the index for hamming
    TEST_ASSERT(EndQuery(3) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(3) == EC_NO_AVAIL_RES);

    TEST_ASSERT(EndQuery(4) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(4) == EC_NO_AVAIL_RES);

    // removes queries from the index for edit
    TEST_ASSERT(EndQuery(5) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(5) == EC_NO_AVAIL_RES);

    TEST_ASSERT(EndQuery(6) == EC_SUCCESS);
    TEST_ASSERT(EndQuery(6) == EC_NO_AVAIL_RES);

    String* wordsarr = Seperate_sentence(&query);   // seperate the query into words
    
    Map map_exact = index_index(get_index_exact());     // get the index for exact

    // check for every entry if the equivalent 
    // query is removed from the payload.
    for (int i = 0; i < 4; i++) {
        struct entry entry;

        entry.word = wordsarr[i];
        entry.payload = NULL;

        Entry existentry = map_find(map_exact, &entry);

        TEST_ASSERT(list_size(existentry->payload) == 0);

        // checks if the query keep exists in the payload
        for (int j = 1; j < 3; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res == NULL);
        }
    }


    BKTree bktree_hamming = index_index(get_index_hamming());   // get the index for hamming

    // check for every entry if the equivalent 
    // query is removed from the payload.
    for (int i = 0; i < 4; i++) {
        Entry existentry = bk_find_entry(bktree_hamming, wordsarr[i]);

        TEST_ASSERT(list_size(existentry->payload) == 0);

        // checks if the queries keep exists in the payload
        for (int j = 3; j < 5; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res == NULL);
        }        
    }



    BKTree bktree_edit = index_index(get_index_edit());     // get the index for edit

    TEST_ASSERT(bk_size(bktree_edit) == 4);

    // check for every entry if the equivalent 
    // query is removed from the payload.
    for (int i = 0; i < 4; i++) {
        Entry existentry = bk_find_entry(bktree_edit, wordsarr[i]);

        TEST_ASSERT(list_size(existentry->payload) == 0);

        // checks if the query keep exists in the payload
        for (int j = 5; j < 7; j++) {
            query.queryID = j;

            ListNode res = list_find(existentry->payload, &query);

            TEST_ASSERT(res == NULL);
        }        
    }

    // free the memory
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
    // check if all queries were inserted in the complete list  
    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); 
    
    // insert queries in the indices
    TEST_ASSERT(StartQuery(1, "This query looks good", 0, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, "This query looks good", 0, 0) == EC_SUCCESS);


    TEST_ASSERT(StartQuery(3, "This query looks good", 1, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(4, "This query looks good", 1, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(5, "This query looks good", 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(6, "This query looks good", 2, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(7, "Hey there! What's up?", 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(8, "Hey there! What's up?", 2, 0) == EC_SUCCESS);

    TEST_ASSERT(MatchDocument(1, "This were lovely query good looks") == EC_SUCCESS);

    unsigned int p_doc_id = 0;      // document id
    unsigned int p_num_res = 0;     // number of completed queries
    unsigned int* p_query_id = NULL;    // list of completed queries

    GetNextAvailRes(&p_doc_id, &p_num_res, &p_query_id);

    TEST_ASSERT(p_num_res == 6);    // the num of completed queries are 6

    // the expected completed queries  
    // are 1, 2, 3, 4, 5, 6
    for(int i = 0; i < p_num_res; i++) {
       TEST_ASSERT(p_query_id[i] == i+1);
    }

    free(p_query_id);

    // removes one query from the entries->payload 
    // and after that it checks if it was removed

    TEST_ASSERT(EndQuery(1) == EC_SUCCESS);

    TEST_ASSERT(MatchDocument(1, "This were lovely query good looks") == EC_SUCCESS);

    // initialize the values
    p_doc_id = 0;
    p_num_res = 0;
    p_query_id = NULL;

    GetNextAvailRes(&p_doc_id, &p_num_res, &p_query_id);

    TEST_ASSERT(p_num_res == 5);    // the num of completed queries are 5, because query was removed

    // the expected completed queries  
    // are 2, 3, 4, 5, 6
    for(int i = 0; i < p_num_res; i++){
       TEST_ASSERT(p_query_id[i] == i+2);
    }

    // free memory
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
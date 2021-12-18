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

    struct query tmpquery;

    tmpquery.queryID = 1;
    tmpquery.words = NULL;

    Query existquery = map_find(map_queries, &tmpquery);

    TEST_ASSERT(existquery != NULL);

    TEST_ASSERT(existquery->queryID == 1);

    tmpquery.queryID = 2;

    existquery = map_find(map_queries, &tmpquery);

    TEST_ASSERT(existquery != NULL);

    TEST_ASSERT(existquery->queryID == 2);

    tmpquery.queryID = 3;

    existquery = map_find(map_queries, &tmpquery);

    TEST_ASSERT(existquery != NULL);

    TEST_ASSERT(existquery->queryID == 3);


    tmpquery.queryID = 4;

    existquery = map_find(map_queries, &tmpquery);

    TEST_ASSERT(existquery != NULL);

    TEST_ASSERT(existquery->queryID == 4);


    tmpquery.queryID = 5;

    existquery = map_find(map_queries, &tmpquery);

    TEST_ASSERT(existquery != NULL);

    TEST_ASSERT(existquery->queryID == 5);


    tmpquery.queryID = 6;

    existquery = map_find(map_queries, &tmpquery);

    TEST_ASSERT(existquery != NULL);

    TEST_ASSERT(existquery->queryID == 6);

    

    Map map = index_index(get_index_edit());

    String* wordsarr = Seperate_sentence(&query);

    for (int i = 0; i < 4; i++) {
        struct entry entry;

        entry.word = wordsarr[i];
        entry.payload = NULL;

        Entry existentry = map_find(map, &entry);

        query.queryID = 1;

        ListNode res = list_find(existentry->payload, &query);

        TEST_ASSERT(res != NULL);

    }




    TEST_ASSERT(DestroyIndex() == EC_SUCCESS);

}

void test_EndQuery(void) {
    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); 
    
    TEST_ASSERT(StartQuery(1, "This query looks good", 0, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, "This query looks good", 0, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(3, "This query looks good", 1, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(4, "This query looks good", 1, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(5, "This query looks good", 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(6, "This query looks good", 2, 0) == EC_SUCCESS);

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

    TEST_ASSERT(MatchDocument(1, "This were lovely query good looks") == EC_SUCCESS);

    unsigned int p_doc_id = 0;
    unsigned int p_num_res = 0;
    unsigned int* p_query_id = NULL;

    GetNextAvailRes(&p_doc_id, &p_num_res, &p_query_id);

    for(int i = 0; i < p_num_res; i++){
       TEST_ASSERT(p_query_id[i] == i+1);
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
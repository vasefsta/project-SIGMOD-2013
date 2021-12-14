#include <stdlib.h>
#include <string.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "core_test.h"
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
    TEST_ASSERT(StartQuery(1, "This query looks good", 0, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, "This query looks good", 0, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(3, "This query looks good", 1, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(4, "This query looks good", 1, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(5, "This query looks good", 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(6, "This query looks good", 2, 0) == EC_SUCCESS);

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
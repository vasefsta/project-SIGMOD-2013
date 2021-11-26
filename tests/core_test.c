#include <stdlib.h>
#include <string.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "core_test.h"
#include "ADTMap.h"
#include "ADTIndex.h"
#include "ADTEntryList.h"
#include "ADTBKTree.h"
#include "ADTLinkedList.h"


void test_CreateIndex() {
    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); 
    
    TEST_ASSERT(get_index_edit() != NULL);
    TEST_ASSERT(get_index_exact() != NULL);
    TEST_ASSERT(get_index_hamming() != NULL);
    TEST_ASSERT(get_map_queries() != NULL);

    TEST_ASSERT(DestroyIndex() == EC_SUCCESS);
}


void test_StartQuery() {
    
    TEST_ASSERT(InitializeIndex() == EC_SUCCESS); 
    TEST_ASSERT(StartQuery(1, "This query looks good", 0, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, "This query looks good", 0, 0) == EC_SUCCESS);


    TEST_ASSERT(StartQuery(1, "This query looks good", 1, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, "This query looks good", 1, 0) == EC_SUCCESS);

    TEST_ASSERT(StartQuery(1, "This query looks good", 2, 0) == EC_SUCCESS);
    TEST_ASSERT(StartQuery(2, "This query looks good", 2, 0) == EC_SUCCESS);

    TEST_ASSERT(DestroyIndex() == EC_SUCCESS);

}


TEST_LIST = {

	{ "CreateIndex", test_CreateIndex },
    { "StartQuery", test_StartQuery },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
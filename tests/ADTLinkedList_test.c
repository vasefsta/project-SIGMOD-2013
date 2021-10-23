#include <stdlib.h>
#include <string.h>
#include "acutest.h"

#include "ADTLinkedList.h"





void test_create(){
    Listnode list = list_create();

    TEST_ASSERT(list != NULL);

    list_destroy(list, NULL);

}

void test_insert(){
    Listnode list = list_create();

    TEST_ASSERT(list != NULL);

    Query query1 = NULL;
    Query query2 = NULL;
    Query query3 = NULL;
    Query query4 = NULL;
   

    list_insert(list, query1);
    list_insert(list, query2);
    list_insert(list, query3);
    list_insert(list, query4);


    TEST_ASSERT(get_size(list) == 4);

    list_destroy(list, NULL);

}



TEST_LIST = {

	{ "linkedlist_create", test_create },
    { "linkedlist_insert", test_insert },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
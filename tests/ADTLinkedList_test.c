#include <stdlib.h>
#include <string.h>
#include "acutest.h"

#include "ADTLinkedList.h"

int compare_queries(Query a, Query b) {
	return strcmp(a->words,b->words);
}


void destroy(Query query){
    free(query->words);
    free(query);
}


void test_create(){
    List list = list_create();

    TEST_ASSERT(list != NULL);

    list_destroy(list, NULL);
}

void test_insert(){
    List list = list_create();

    TEST_ASSERT(list != NULL);

    Query query1 = malloc(sizeof(*query1));
    Query query2 = malloc(sizeof(*query1));
    Query query3 = malloc(sizeof(*query1));
    Query query4 = malloc(sizeof(*query1));
   

    query1->words = strdup("Kostis palamas giolo");
    query1->length = 3;

    query2->words = strdup("HAHAHA AHAHHAAH AHAHAHAH");
    query2->length = 3;

    query3->words = strdup("sadas dasasdad adsdasdas");
    query3->length = 3;

    query4->words = strdup("werew rewrqc dvs");
    query4->length = 3;


    list_insert(list, query1);
    list_insert(list, query2);
    list_insert(list, query3);
    list_insert(list, query4);


    TEST_ASSERT(list_size(list) == 4);

    ListNode result = list_find(list, (CompareFunc) compare_queries, query1);

    TEST_ASSERT(result != NULL);

    result = list_find(list, (CompareFunc) compare_queries, query2);

    TEST_ASSERT(result != NULL);

    result = list_find(list, (CompareFunc) compare_queries, query3);

    TEST_ASSERT(result != NULL);

    result = list_find(list, (CompareFunc) compare_queries, query4);

    TEST_ASSERT(result != NULL);
    
    
    list_destroy(list, (DestroyFunc) destroy);

}



TEST_LIST = {

	{ "linkedlist_create", test_create },
    { "linkedlist_insert", test_insert },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
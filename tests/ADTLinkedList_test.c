#include <stdlib.h>
#include <string.h>

#include "acutest.h"
#include "ADTEntryList.h"

#include "ADTLinkedList.h"

int compare_entries(Entry a, Entry b) {
	return strcmp(a->word,b->word);
}

void destroy_query(Query query) {
    free(query->words);
    free(query);
}

void destroy_entries(Entry entry){
    list_destroy(entry->payload, (DestroyFunc)destroy_query);
    free(entry->word);
    free(entry);
}


void test_create(){
    List list = list_create();

    TEST_ASSERT(list != NULL);

    list_destroy(list, NULL);
}

void test_insert(){
    List list = list_create();

    TEST_ASSERT(list != NULL);

    Entry entry1 = malloc(sizeof(*entry1));
    Entry entry2 = malloc(sizeof(*entry2));
   

    entry1->word = strdup("hello");
    entry1->payload = list_create();

    Query query1 = malloc(sizeof(*query1));
    Query query2 = malloc(sizeof(*query2));
    Query query3 = malloc(sizeof(*query3));
    Query query4 = malloc(sizeof(*query4));

    query1->words = strdup("Kostis palamas giolo");
    query1->length = 3;

    query2->words = strdup("HAHAHA AHAHHAAH AHAHAHAH");
    query2->length = 3;

    query3->words = strdup("sadas dasasdad adsdasdas");
    query3->length = 3;

    query4->words = strdup("werew rewrqc dvs");
    query4->length = 3;

    list_insert(entry1->payload, query1);
    list_insert(entry1->payload, query2);
    list_insert(entry1->payload, query3);
    list_insert(entry1->payload, query4);

    list_insert(list, entry1);


    entry2->word = strdup("bye");
    entry2->payload = list_create();

    Query query5 = malloc(sizeof(*query5));
    Query query6 = malloc(sizeof(*query6));
    Query query7 = malloc(sizeof(*query7));
    Query query8 = malloc(sizeof(*query8));

    query5->words = strdup("Kostis palamas giolo");
    query5->length = 3;

    query6->words = strdup("HAHAHA AHAHHAAH AHAHAHAH");
    query6->length = 3;

    query7->words = strdup("sadas dasasdad adsdasdas");
    query7->length = 3;

    query8->words = strdup("werew rewrqc dvs");
    query8->length = 3;

    list_insert(entry2->payload, query5);
    list_insert(entry2->payload, query6);
    list_insert(entry2->payload, query7);
    list_insert(entry2->payload, query8);

    list_insert(list, entry2);


    TEST_ASSERT(list_size(list) == 2);

    ListNode result = list_find(list, (CompareFunc) compare_entries, entry1);

    TEST_ASSERT(result != NULL);

    result = list_find(list, (CompareFunc) compare_entries, entry2);

    TEST_ASSERT(result != NULL);
    
    
    list_destroy(list, (DestroyFunc) destroy_entries);
}



TEST_LIST = {

	{ "linkedlist_create", test_create },
    { "linkedlist_insert", test_insert },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
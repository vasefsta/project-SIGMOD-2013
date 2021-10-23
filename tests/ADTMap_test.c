#include <stdlib.h>
#include <string.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTMap.h"

void destroy_query(Query query) {
	free(query->words);
	free(query);
}

unsigned int hash_function(Query query) {
	return hash_string(query->words);
}

int compare_queries(Query a, Query b) {
	return strcmp(a->words,b->words);
}


void test_create(void) {

	// Δημιουργούμε μια κενή λίστα (χωρίς αυτόματο free)
	int size = 100;
	Map map = map_create( (CompareFunc)compare_queries, size);
	map_set_hash_function(map, (HashFunc)hash_function);

	// Ελέγχουμε ότι δεν απέτυχε η malloc στην λίστα, και ότι
	// αρχικοποιείται με μέγεθος 0 (δηλαδή χωρίς κόμβους)
	TEST_ASSERT(map != NULL);
	
	map_destroy(map, (DestroyFunc)destroy_query);
}

void test_insert(void) {
	int size = 100;
	Map map = map_create( (CompareFunc)compare_queries, size);
	map_set_hash_function(map, (HashFunc)hash_function);

	TEST_ASSERT(map != NULL);

	Query q1 = malloc(sizeof(struct query));
	Query q2 = malloc(sizeof(struct query));
	Query q3 = malloc(sizeof(struct query));
	Query q4 = malloc(sizeof(struct query));


	q1->words = strdup("xartino to feggarai oeoeoeo");
	q1->length = 4;

	q3->words = strdup("xartino to feggarai oeoeoeo");
	q3->length = 4;

	q4->words = strdup("pezuna kalliopi");
	q4->length = 2;

	q2->words = strdup("Pezuna kalliopi");
	q2->length = 2;


	map_insert(map, q1);
	map_insert(map, q2);

	map_insert(map, q3);
	map_insert(map, q4);

	TEST_ASSERT(map_capacity(map) == 4);

	TEST_ASSERT(map_find(map, q1) == 1);
	TEST_ASSERT(map_find(map, q2) == 1);
	TEST_ASSERT(map_find(map, q3) == 1);
	TEST_ASSERT(map_find(map, q4) == 1);


	map_destroy(map, (DestroyFunc)destroy_query);	
}

TEST_LIST = {

	{ "map_create", test_create },
	{ "map_insert", test_insert },
	// { "map_find", 	test_find },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
#include <stdlib.h>
#include <string.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTMap.h"

void destroy_query(Query query) {
	free(query->words);
	free(query);
}


void test_create(void) {

	// Δημιουργούμε μια κενή λίστα (χωρίς αυτόματο free)
	int size = 100;
	MapNode map = map_create(size);

	// Ελέγχουμε ότι δεν απέτυχε η malloc στην λίστα, και ότι
	// αρχικοποιείται με μέγεθος 0 (δηλαδή χωρίς κόμβους)
	TEST_ASSERT(map != NULL);
	
	map_destroy(map, (DestroyFunc)destroy_query, size);
}

void test_insert(void) {
	MapNode map = map_create(100);

	TEST_ASSERT(map != NULL);

	Query q1 = malloc(sizeof(struct query));
	Query q2 = malloc(sizeof(struct query));
	Query q3 = malloc(sizeof(struct query));
	Query q4 = malloc(sizeof(struct query));


	q1->words = strdup("xartino to feggarai oeoeoeo");
	q1->length = 4;

	q3->words = strdup("xartino to feggarai oeoeoeo");
	q3->length = 4;

	q4->words = strdup("pezuna kalliopu");
	q4->length = 2;

	q2->words = strdup("Pezuna kalliopi");
	q2->length = 2;


	map_insert(map, q1, 100);
	map_insert(map, q2, 100);

	map_insert(map, q3, 100);
	map_insert(map, q4, 100);

	TEST_ASSERT(map_find(map, 100, q1) == 1);
	TEST_ASSERT(map_find(map, 100, q2) == 1);
	TEST_ASSERT(map_find(map, 100, q3) == 1);
	TEST_ASSERT(map_find(map, 100, q4) == 1);


	map_destroy(map, (DestroyFunc)destroy_query, 100);	
}

TEST_LIST = {

	{ "map_create", test_create },
	{ "map_insert", test_insert },
	// { "map_remove", test_remove },
	// { "map_find", 	test_find },
	// { "map_iterate",test_iterate },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
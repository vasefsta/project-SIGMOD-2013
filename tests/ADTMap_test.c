#include <stdlib.h>
#include <string.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTMap.h"

void destroy_query(Query query) {
	free(query->words);
	// free(query);
}

void test_create(void) {

	// Δημιουργούμε μια κενή λίστα (χωρίς αυτόματο free)
	int size = 100;
	MapNode map = map_create(size);

	// Ελέγχουμε ότι δεν απέτυχε η malloc στην λίστα, και ότι
	// αρχικοποιείται με μέγεθος 0 (δηλαδή χωρίς κόμβους)
	TEST_ASSERT(map != NULL);
	
	map_destroy(map, NULL, size);
}

void test_insert(void) {
	MapNode map = map_create(100);

	struct query q1[3];

	q1[0].words = strdup("hello world");
	q1[0].length = 2;

	q1[1].words = strdup("kwstis palamas");
	q1[1].length = 2;

	q1[2].words = strdup("xartino to feggarai oeoeoeo");
	q1[2].length = 4;


	for (int i = 0; i < 3; i++) {
		map_insert(map, &q1[i], 100);
	} 

	map_destroy(map, (DestroyFunc)destroy_query, 100);	
}

TEST_LIST = {
	// { "create", test_create },

	{ "map_create", test_create },
	{ "map_insert", test_insert },
	// { "map_remove", test_remove },
	// { "map_find", 	test_find },
	// { "map_iterate",test_iterate },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
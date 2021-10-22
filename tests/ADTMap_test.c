#include <stdlib.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTMap.h"


void test_create(void) {

	// Δημιουργούμε μια κενή λίστα (χωρίς αυτόματο free)
	MapNode map = map_create(100);

	// Ελέγχουμε ότι δεν απέτυχε η malloc στην λίστα, και ότι
	// αρχικοποιείται με μέγεθος 0 (δηλαδή χωρίς κόμβους)
	TEST_ASSERT(map != NULL);

	map_destroy(map, 100);
}

TEST_LIST = {
	// { "create", test_create },

	{ "map_create", test_create },
	// { "map_insert", test_insert },
	// { "map_remove", test_remove },
	// { "map_find", 	test_find },
	// { "map_iterate",test_iterate },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
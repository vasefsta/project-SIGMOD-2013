#include <stdlib.h>
#include <string.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTMap.h"


char Array[93][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
"sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
"rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip", "feta", "football", "basketball", "handball", "soft", "park", "walk",
"popular", "micro", "chips", "ketchup", "mayo", "clothes", "longsleeve", "jacket", "player", "barney", "dinosaurs", "phone", "socks", "manager", "pirates",
"carribean", "spray", "hookah", "chair", "sofa", "basket", "apple", "banana", "orange", "peach", "strawberry", "blue", "pink", "yellow", "brown", "black",
"white", "shadow", "jumbo", "public", "private", "glass", "plastic", "balcony", "floor", "plug", "piano", "electricity", "carbon", "bars", "portrait" }; 

void destroy_word(String word) {
	free(word);
}

unsigned int hash_function(String word) {
	return hash_string(word);
}

int compare_words(String stra, String strb) {
	return strcmp(stra,strb);
}


void test_create(void) {

	// Δημιουργούμε μια κενή λίστα (χωρίς αυτόματο free)
	int size = 100;
	Map map = map_create((CompareFunc)compare_words, size);
	map_set_hash_function(map, (HashFunc)hash_function);

	// Ελέγχουμε ότι δεν απέτυχε η malloc στην λίστα, και ότι
	// αρχικοποιείται με μέγεθος 0 (δηλαδή χωρίς κόμβους)
	TEST_ASSERT(map != NULL);
	
	map_destroy(map, NULL);
}

void test_insert(void) {
	int size = 100;
	Map map = map_create( (CompareFunc)compare_words, size);
	map_set_hash_function(map, (HashFunc)hash_function);

	TEST_ASSERT(map != NULL);

	int N = 50;
	String strArray[N];
	
	for (int i = 0; i < N; i++) {
		strArray[i] = strdup(Array[i]);
		map_insert(map, strArray[i]);
		TEST_ASSERT(map_find(map, strArray[i]));
	}

	TEST_ASSERT(map_capacity(map) == 50);

	map_destroy(map, (DestroyFunc) destroy_word);	
}

TEST_LIST = {

	{ "map_create", test_create },
	{ "map_insert", test_insert },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
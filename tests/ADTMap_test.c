/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTMap_test.c     
*/

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

void test_create(void) {

	// Create map with size 100.
	int size = 100;
	Map map = map_create((CompareFunc)strcmp, size);
	map_set_hash_function(map, (HashFunc)hash_function);

	// Test if map is created.
	TEST_ASSERT(map != NULL);	

	// Test if map capacity is 0.
	TEST_ASSERT(map_capacity(map) == 0);						
	
	// Free allocated memory of map.
	map_destroy(map, NULL);
}

void test_insert(void) {
	// Create map with size 100.
	int size = 100;
	Map map = map_create((CompareFunc)strcmp, size);
	map_set_hash_function(map, (HashFunc)hash_function);

	// Test if map is created
	TEST_ASSERT(map != NULL);									

	int N = 50;
	String strArray[N];
	
	// Create an array with 50 strings
	// and insert all strings in map.
	for (int i = 0; i < N; i++) {
		strArray[i] = strdup(Array[i]);
		map_insert(map, strArray[i]);		
	}

	// Test if all strings were iserted in Map
	for(int i = 0; i < N; i++)
		TEST_ASSERT(map_find(map, strArray[i]) != NULL);

	// Test map capacity
	TEST_ASSERT(map_capacity(map) == N);						

	String newstr = strdup("Athens");

	// Insert a new string.
	map_insert(map, newstr);

	// Test if new string was inserted
	TEST_ASSERT(map_find(map, newstr) != NULL);					
	// Test map capacity
	TEST_ASSERT(map_capacity(map) == N+1);					

    char tempstr[5] = "Goku"; 
	
	// Test if tempstr is not in map
    TEST_ASSERT((map_find(map, tempstr) == NULL));				
    
	// Test if map capacity remained same
	TEST_ASSERT(map_capacity(map) == N + 1);			

	// Free allocated memory.
	map_destroy(map, (DestroyFunc) destroy_word);	
}


void test_find(void) {
	// Create map with size 100.
	int size = 100;
	Map map = map_create((CompareFunc)strcmp, size);
	map_set_hash_function(map, (HashFunc)hash_function);
	
	// Test if map is created
	TEST_ASSERT(map != NULL);

	int N = 50;
	String strArray[N];
	
	// Create an array with 50 strings
	// and insert all strings in map.
	for (int i = 0; i < N; i++) {
		strArray[i] = strdup(Array[i]);
		map_insert(map, strArray[i]);
	}

	// Check if every word was inserted in map.
	// For every word in array check if
	// it exists in map.
	for (int i = 0; i < N; i++) {
		String checkstr = map_find(map, strArray[i]);
		TEST_ASSERT(checkstr != NULL);
		TEST_ASSERT(!strcmp(checkstr, strArray[i]));
	}

	char tempstr[5] = "Goku"; 
	// Checks if word exists in map.
	// This word is not inserted in map!!!
    String checkstr = map_find(map, tempstr);

	// Check if word was not found in map.
	TEST_ASSERT(checkstr == NULL);

	// Free allocated memory.
	map_destroy(map, (DestroyFunc) destroy_word);	
}

TEST_LIST = {
	// List of al tests.
	{ "map_create", test_create },
	{ "map_insert", test_insert },
	{ "map_find", test_find },
	{ NULL, NULL }
}; 
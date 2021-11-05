/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTLinkedList_test.c     
*/

#include <stdlib.h>
#include <string.h>

#include "acutest.h"
#include "ADTEntryList.h"

#include "ADTLinkedList.h"

char Array[93][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
"sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
"rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip", "feta", "football", "basketball", "handball", "soft", "park", "walk",
"popular", "micro", "chips", "ketchup", "mayo", "clothes", "longsleeve", "jacket", "player", "barney", "dinosaurs", "phone", "socks", "manager", "pirates",
"carribean", "spray", "hookah", "chair", "sofa", "basket", "apple", "banana", "orange", "peach", "strawberry", "blue", "pink", "yellow", "brown", "black",
"white", "shadow", "jumbo", "public", "private", "glass", "plastic", "balcony", "floor", "plug", "piano", "electricity", "carbon", "bars", "portrait" }; 



void test_create(){
    List list = list_create(NULL);

    TEST_ASSERT(list != NULL);
    TEST_ASSERT(list_size(list) == 0);

    list_destroy(list, NULL);
}


void test_insert(){ 
    List list = list_create((CompareFunc) strcmp);

    TEST_ASSERT(list != NULL);

    int N = 50;

	for (int i = 0; i < N; i++) {
		list_insert(list, Array[i]);
        TEST_ASSERT(list_find(list, Array[i]) != NULL);
	}

    TEST_ASSERT(list_size(list) == N);

    char newstr[7] = "Athens";

    list_insert(list, newstr);

    TEST_ASSERT((list_find(list, newstr) != NULL));
    TEST_ASSERT(list_size(list) == N + 1);

    char tempstr[5] = "Goku"; 

    TEST_ASSERT((list_find(list, tempstr) == NULL));
    TEST_ASSERT(list_size(list) == N + 1);

    list_destroy(list, NULL);
}


void test_find(void) {
    List list = list_create((CompareFunc)strcmp);

	TEST_ASSERT(list != NULL);

	int N = 50;
	
	// ελεγχουμε αν υπάρχουν στο map τα στοιχεία που εισάγαμε
	for (int i = 0; i < N; i++) {
		list_insert(list, Array[i]);
	}

	for (int i = 0; i < N; i++) {
		ListNode node = list_find(list, Array[i]);
        
        TEST_ASSERT(node != NULL);

        String checkstr = list_node_value(node);

		TEST_ASSERT(checkstr != NULL);
		TEST_ASSERT(!strcmp(checkstr, Array[i]));
	}

	char tempstr[5] = "Goku"; 

    ListNode node = list_find(list, tempstr);

    TEST_ASSERT(node == NULL);

    list_destroy(list, NULL);
}


TEST_LIST = {

	{ "linkedlist_create", test_create },
    { "linkedlist_insert", test_insert },
    { "linkedlist_find", test_find },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
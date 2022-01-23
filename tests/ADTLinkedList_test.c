/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : ADTLinkedList_test.c     
*/

#include <stdlib.h>
#include <string.h>

#include "acutest.h"
#include "ADTEntryList.h"

#include "ADTLinkedList.h"


// holds words that will be store in entries
char Array[93][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
"sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
"rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip", "feta", "football", "basketball", "handball", "soft", "park", "walk",
"popular", "micro", "chips", "ketchup", "mayo", "clothes", "longsleeve", "jacket", "player", "barney", "dinosaurs", "phone", "socks", "manager", "pirates",
"carribean", "spray", "hookah", "chair", "sofa", "basket", "apple", "banana", "orange", "peach", "strawberry", "blue", "pink", "yellow", "brown", "black",
"white", "shadow", "jumbo", "public", "private", "glass", "plastic", "balcony", "floor", "plug", "piano", "electricity", "carbon", "bars", "portrait" }; 



void test_create(void) {
    List list = list_create(NULL);          // create an empty list

    TEST_ASSERT(list != NULL);              // test if list is created
    TEST_ASSERT(list_size(list) == 0);      // test if list has 0 size

    list_destroy(list, NULL);               // destroy list
}


void test_insert(void){ 
    List list = list_create((CompareFunc) strcmp);      

    TEST_ASSERT(list != NULL);                          // test if list is created

    int N = 50;     // number of elements in the list


	for (int i = 0; i < N; i++) {
		list_insert(list, Array[i]);        // insert word = Array[i] in the list

        TEST_ASSERT(list_size(list) == (i + 1));    // checks if the size is updated
        
        // checks if word is inserted in the first node of list
        TEST_ASSERT(list_node_value(list_first(list)) == Array[i]);     
	}

    // checks if the first fifty words are inserted  
    // in the list with reverse order.
    int i = N - 1;
    for (ListNode node = list_first(list); node != NULL; node = list_find_next(node)) {
        String word = list_node_value(node);

        TEST_ASSERT(strcmp(word, Array[i]) == 0);

        i--;
    }
    
    list_destroy(list, NULL);       // free the list
}


void test_find(void) {
    List list = list_create((CompareFunc)strcmp);

	TEST_ASSERT(list != NULL);      // test if list is created

	int N = 50;     // number of elements in the list
	
	// Insert the first fifty words from Array in the list
	for (int i = 0; i < N; i++) 
		list_insert(list, Array[i]);

    // Checks if list_find can find all the inserted words in the list
	for (int i = 0; i < N; i++) {
		ListNode node = list_find(list, Array[i]);
        
        TEST_ASSERT(node != NULL);

        String checkstr = list_node_value(node);

		TEST_ASSERT(checkstr != NULL);
		TEST_ASSERT(strcmp(checkstr, Array[i]) == 0);
	}

    // Checks if word exists in the list.
    // This word is not inserted in the list.
	char tempstr[5] = "Goku"; 

    ListNode node = list_find(list, tempstr);

    TEST_ASSERT(node == NULL);

    // free the list
    list_destroy(list, NULL);
}


void test_remove(void) {
    List list = list_create((CompareFunc)strcmp);

	TEST_ASSERT(list != NULL);      // test if list is created

	int N = 50;     // number of elements in the list
	
	// Insert the first fifty words from Array in the list
	for (int i = 0; i < N; i++) 
		list_insert(list, Array[i]);

    // Deletes the first word from the list and then checks 
    // if the first word is equal to the word inserted before
    for (int i = N - 1; i >= 0; i--) {
        ErrorCode errcode = list_remove(list, NULL, Array[i]);

        TEST_ASSERT(errcode == EC_SUCCESS);

        // checks if size of list is updated
        TEST_ASSERT(list_size(list) == i);

        if (i > 0) {
            String word = list_node_value(list_first(list));
            TEST_ASSERT(strcmp(word, Array[i]) != 0);
        } else 
            TEST_ASSERT(list_first(list) == NULL);      // NULL beause there is no element in the list
    }

    // refills the list to tests if list_remove  
    // can delete a middle value.  
    for (int i = 0; i < N; i++) 
		list_insert(list, Array[i]);
    
    ErrorCode errcode = list_remove(list, NULL, Array[7]);  // Array[7] = "hello"

    TEST_ASSERT(errcode == EC_SUCCESS);

    TEST_ASSERT(list_size(list) == N - 1);

    // checks if word = "hello" is removed
    for (ListNode node = list_first(list); node != NULL; node = list_find_next(node)) {
        Pointer word = list_node_value(node);

        TEST_ASSERT(strcmp(word, Array[7]) != 0);
    }

    // checks if word = "metallica" is not removed,
    // because it wasn't inserted in the list.
    String tmpword = "metallica";

    errcode = list_remove(list, NULL, tmpword);  // is the word "hello"

    TEST_ASSERT(errcode == EC_NO_AVAIL_RES);

    TEST_ASSERT(list_size(list) == N - 1);

    // free the list
    list_destroy(list, NULL);
}

void test_remove_first(void) {
    List list = list_create((CompareFunc)strcmp);

	TEST_ASSERT(list != NULL);      // test if list is created

	int N = 50;     // number of elements in the list
	
	// Insert the first fifty words from Array in the list
	for (int i = 0; i < N; i++) 
		list_insert(list, Array[i]);

    // Deletes the first word from the list and then checks 
    // if the first word is equal to the word inserted before
    for (int i = N - 1; i >= 0; i--) {
        list_remove_first(list);

        // checks if size of list is updated
        TEST_ASSERT(list_size(list) == i);

        if (i > 0) {
            String word = list_node_value(list_first(list));
            TEST_ASSERT(strcmp(word, Array[i]) != 0);
        } else 
            TEST_ASSERT(list_first(list) == NULL);  // NULL beause there is no element in the list
    }

    // checks list_remove_first works right  
    // if a new word is inserted in the list.
    list_insert(list, Array[0]);

    list_remove_first(list);

    TEST_ASSERT(list_size(list) == 0);

    TEST_ASSERT(list_first(list) == NULL);      // NULL beause there is no element in the list

    // free the list
    list_destroy(list, NULL);
}


TEST_LIST = {

	{ "linkedlist_create", test_create },
    { "linkedlist_insert", test_insert },
    { "linkedlist_find", test_find },
    { "linkedlist_remove", test_remove },
    { "linkedlist_remove_first", test_remove_first },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
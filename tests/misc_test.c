/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : misc_test.c     
*/

#include "acutest.h"
#include "misc.h"
#include "ADTEntryList.h"
#include "ADTLinkedList.h"


int compare_query(QueryID* q1, QueryID* q2) {
    return *q1 - *q2;
}

int compare_queries(Query q1, Query q2) {
  return q1->queryID - q2->queryID;
}


int compare_entry(Entry e1, Entry e2) {
    return strcmp(e1->word, e2->word);
}


const void destroy_entry(Entry entry) {
    free(entry->word);                              
    list_destroy(entry->payload, NULL);
    free(entry);
}

void test_convert_to_query(void) {
  // Create a string and the query from this string.
  String string = strdup("looks watermelon winter");
  Query query = convert_to_query(string);

  // Test if query is created;
  TEST_ASSERT(query != NULL);

  // Test query's string.
  TEST_ASSERT(strcmp(query->words, "looks watermelon winter") == 0);
  
  // Test query's length.
  TEST_ASSERT(query->length == 3);

  // Free Query.
  free(query->words);
  free(query);
  free(string);
}

void test_Seperate_Sentence(void) {
  // Create a query with string as word.
  String string = strdup("looks watermelon winter");
  Query query = convert_to_query(string);

  // Test if query is created;
  TEST_ASSERT(query != NULL);

  // Test query's string.
  TEST_ASSERT(strcmp(query->words, "looks watermelon winter") == 0);
  
  // Test query's length.
  TEST_ASSERT(query->length == 3);

  String *Array = Seperate_sentence(query);

  // Test first element in array.
  TEST_ASSERT(strcmp(Array[0], "looks") == 0);  
  // Test second element in array.
  TEST_ASSERT(strcmp(Array[1], "watermelon") == 0);  
  // Test third element in array.
  TEST_ASSERT(strcmp(Array[2], "winter") == 0);  

  // Free allocated memory.
  for(int i = 0; i < 3; i++)
    free(Array[i]);

  free(Array);
  free(query->words);
  free(query);
  free(string);
}

void test_Deduplicate_words_map(void){
  // Create a string to deduplicate
  String string = strdup("first second third fourth fifth sixth second seventh first second eighth ninenth fourth first");
  
  // Get result in list.
  List list = deduplicated_words_map(string);

  ListNode node = list_first(list);
  // Get word from node
  String word = list_node_value(node);
  // Test word. 
  TEST_ASSERT(strcmp(word, "ninenth") == 0 );
  
  node = list_find_next(node);
  // Get word from node
  word = list_node_value(node);
  // Test word. 
  TEST_ASSERT(strcmp(word, "eighth") == 0 );

  node = list_find_next(node);
  // Get word from node
  word = list_node_value(node);
  // Test word. 
  TEST_ASSERT(strcmp(word, "seventh") == 0 );

   node = list_find_next(node);
  // Get word from node
  word = list_node_value(node);
  // Test word. 
  TEST_ASSERT(strcmp(word, "sixth") == 0 );

   node = list_find_next(node);
  // Get word from node
  word = list_node_value(node);
  // Test word. 
  TEST_ASSERT(strcmp(word, "fifth") == 0 );

   node = list_find_next(node);
  // Get word from node
  word = list_node_value(node);
  // Test word. 
  TEST_ASSERT(strcmp(word, "fourth") == 0 );

   node = list_find_next(node);
  // Get word from node
  word = list_node_value(node);
  // Test word. 
  TEST_ASSERT(strcmp(word, "third") == 0 );

   node = list_find_next(node);
  // Get word from node
  word = list_node_value(node);
  // Test word. 
  TEST_ASSERT(strcmp(word, "second") == 0 );

   node = list_find_next(node);
  // Get word from node
  word = list_node_value(node);
  // Test word. 
  TEST_ASSERT(strcmp(word, "first") == 0 );

  //NULL


  // Free allocated memory
  free(string);
  list_destroy(list, (DestroyFunc)free);
}



TEST_LIST = {
  { "convert_to_quet", test_convert_to_query },
  { "Seperate_Sentence", test_Seperate_Sentence },
  { "Deduplicate_words_map", test_Deduplicate_words_map},
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
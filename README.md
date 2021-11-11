![example workflow](https://github.com/vasiliki2000/project-SIGMOD-2013/actions/workflows/c.yml/badge.svg)
# project-SIGMOD-2013

 |Authors|sdi*******|
 |-------|----------|
 |Vasiliki Efstathiou|1800229|
 |Nikos Eftychiou|1800221|



### Compile And Execution

- With the command **"make"** in PROJECT-SIGMOD-2013 dir all source code needed by the program is compiled.
- With the command **"make run"** in PROJECT-SIGMOD-2013 dir all source code needed by the program is compiled and
    tests and main function are all executed.
- With the command **"make valgrind"** in PROJECT-SIGMOD-2013 dir all source code needed by the program is compiled
    tests and main functions are all executed with valgrind to show leak errors. (A test will pass only if valgrind has no errors or memory leaks).
- With the command **"make clean"** all files that "make" || "make run" || "make valgrind" produced are removed.

#### Example

```
    cd PROJECT-SIGMOD-2013
    make run
    make clean
```

```
    cd PROJECT-SIGMOD-2013
    make valgrind
    make clean
```

#### IMPORTANT

- To compile the program so index is EXACT_MATCH go to commmon.mk and set the last option to "-DEXACT"
- To compile the program so index is EXACT_MATCH go to commmon.mk and set the last option to "-DHAMMING"
- To compile the program so index is EXACT_MATCH go to commmon.mk and set the last option to "-DEDIT"


### Info to better understand our philosophy

- Every query read from **"queries.txt"** is stored in a hashtable (ADTMap).
- After a query is inserted in hashtable is also inserted in the payload of the corresponding entry.
- The payload of every entry is a list of Pointers pointing at the queries.

 
### General Information:

- We decided to organize the code in five directories. 
  - programs : Containing the main function of program.
  - modules &nbsp; : Containing all functions needed by modules to work properly.
  - include &nbsp; &nbsp; : Containing all header files needed by source code.
  - tests &nbsp; &nbsp; &nbsp; &nbsp; : Containing the test functions to prove that every module works flawless.
  - misc &nbsp; &nbsp; &nbsp; &nbsp; : Containing all "sub functions" needed by program.
    

### Notes: 

- With list_test both ADTLinkedList and ADTEntryList are being checked.
- lookup_entry_word can check only one word at a time so we call this function for every word in a Document.
- To generate queries.txt compile misc/Script.c and use the commant **"./a.out > queries.txt"**
- To generate Documents compile misc/Script_Documents.c and run it.


### ADTBKTREE

- struct bktree holds a bknode (the root of tree) and one compare function that will be either Hamming_distance or Edit_distance.

- struct bknode holds an entry (node) and a list of bknodes (children)

- ```ruby
    ErrorCode insert(BKNode bkparent, BKNode new, CompareFunc compare)
    ```    
    At first, this function will calculate the distance (d) betweeen the new node and the current node (parent node). After that if current node has no children, new node will be his newborn child. If parent has children, we proceed at finding a child with distance (d') equal to (d). If such child is found we recursively call insert with child as parent. If no child is found we insert new node with it's siblings.

- ```ruby
    int find(BKNode bkparent, CompareFunc compare, EntryList entrylist, String word, int threshold)
    ```
    At first this function will calculate the distance (d) betweeen the new node and the current node (parent node). Next it will calculate the lower bound (convert negative number to zero). If new node is within bounds bkparent will be added in entrylist. After that we calculate the distance of every child (if they exist) 
    and call find recurisively to check new node with all children

- ```ruby
    void destroy(BKNode bknode, DestroyFunc destroy_value)
    ```
    This function will recursively free bknode and its value with DestroyFunc
    and traverse through the children's list to do the same for them.

- ```ruby
    BKTree bk_create(MatchType type)
    ```
    This function creates a new bk tree.

- ```ruby
    ErrorCode bk_insert(BKTree bktree, Entry entry)
    ```
    This function will create a new node for the entry.
    After that it will check if a root is created. If not, the new node will become the root of th tree. Else we call insert to insert the new node in tree.

- ```ruby
    Entry bk_node_value(BKNode node)
    ```
    Returns the value of node.

- ```ruby
    int bk_find(BKTree bktree, EntryList entrylist, String word, int n)
    ```
    This function checks if a tree exists. If not returns negative message else calls find to check the nodes of the tree.

- ```ruby
    void bk_destroy(BKTree bktree, DestroyFunc destroy_value)
    ```
    This function calls destroy to free very node of tree and after that it frees the bktree

- ```ruby
    int hamming_distance(Pointer value1, Pointer value2){
    ```
    This function calculates the hamming distance between value1 and value2

- ```ruby
    int edit_distance(Pointer value1, Pointer value2) {
    ```
    This function calculates the edit distance between value1 and value2

    .
### ADTEntryList

- struct entry holds a string(word) and a list of the queries that own this word(payload)

- ```ruby
    Entry create_entry(String word, CompareFunc compare) 
    ```    
    Creates a new entry with empty list and word(word)

- ```ruby
    String get_entry_word(Entry entry)
    ```    
    Returns the word of the entry

- ```ruby
    List get_entry_payload(Entry entry)
    ```    
    Returns the payload of the entry

- ```ruby
    ErrorCode destroy_entry(Entry entry)
    ```    
    Destroys entry, it's word and it's payload list.

- ```ruby
    EntryList create_entry_list(CompareFunc compare)
    ```    
    Creates a new entrylist

- ```ruby
    unsigned int get_number_entries(EntryList entrylist)
    ```
    Returns the number of entries in entrylist

- ```ruby
    ErrorCode add_entry(EntryList entrylist, Entry new_entry)
    ```    
    Appends new_entry in entrylist

- ```ruby
    Entry get_first(EntryList entrylist)
    ```    
    Returns first entry from entrylist

- ```ruby
    Entry get_next(EntryList entrylist, Entry current_entry)
    ```    
    Returns the next entry of current_entry from entrylist

- ```ruby
    Entry find_entry(EntryList entrylist, Entry current_entry)
    ```    
    Returns an entry with same word as current_entry

- ```ruby
    ErrorCode destroy_entry_list(EntryList entrylist, DestroyFunc destroy_value)
    ```    
    Destroys entrylist.


### ADTIndex

- struct index holds a pointer to an index(Map or BKTree) and the MatchType of it(exact, hamming, edit).

- ```ruby
    Index create_index(MatchType matchtype, CompareFunc compare, int size)
    ```    
    This function creates an empty index.

 - ```ruby
    ErrorCode build_entry_index(Index index, const EntryList entrylist)
    ```    
    This function with the help of map_insert or bk_insert, inserts every entry from entrylist to index.

- ```ruby
    ErrorCode lookup_entry_index(Index index, String word, int threshold, EntryList result, CompareFunc compare_queries)
    ```    
    This function with the help of map_find or bk_find inserts in result all entries that match the word.


- ```ruby
    ErrorCode destroy_entry_index(Index index)
    ```    
    This function with the help of bk_destroy and map_destroy destroys the index.


### ADTLinkedList

- struct list holds a dummy ListNode (for the list) a compare function to compare the values in listnodes and the size that holds the number of items in list.

- struct listnode holds a pointer to the value of node and a pointer to the next listnode.

- ```ruby
    List list_create(CompareFunc compare)
    ```    
    This function creates an empty list with compare_function compare

- ```ruby
    int list_size(List list)
    ```    
    Returns the number of elements in list

- ```ruby
    ListNode list_first(List list)
    ```    
    Returns the first ListNode from list (next of dummy)

- ```ruby
    void list_insert(List list, Pointer value){
    ```    
    Creates a new ListNode and appends it at the end of list

- ```ruby
    ListNode list_find(List list, Pointer value)
    ```    
    Searches through list and returns a ListNode with value value. If no such ListNode exists, it returns NULL.

- ```ruby
    Pointer list_node_value(ListNode node)
    ```    
    Returns the value of node.

- ```ruby
    ListNode list_find_next(ListNode node)
    ```    
    Returns the next node of node.

- ```ruby
    void list_destroy(List list, DestroyFunc destroy)
    ```    
    Destroys list and it's nodes and with function destroy it destroyes node's value. 


### ADTMap

- struct map holds a MapNode to a hashtable, the size of hashtable, the number of elements in hashtable(capacity) a compare_function to compare MapNodes and a hashfunction to hash the values.

- struct mapnode holds a pointer to a value and a pointer to the next mapnode (If there are more than one mapnodes in the same bucket)

- ```ruby
    Map map_create( CompareFunc compare_function, int size)
    ```    
    This function creates a new map with size equal to 1.2 of the given size

- ```ruby
    void map_set_hash_function(Map map, HashFunc hash_function)
    ```    
    This function sets the hash_function of map

- ```ruby
    int map_capacity(Map map)
    ```    
    This function returns the number of mapnodes in map.

- ```ruby
    ErrorCode map_insert(Map map, Pointer value)
    ```    
    This function hashes the value, creates a new mapnode and inserts it in map. If the position from hash_function is empty new mapnode is inserted or else it's inserted at the end of the list of the bucket

- ```ruby
    int map_destroy(Map map, DestroyFunc destroy)
    ```    
    This function destroys every mapnode in map and the list in every bucket(if exists) and then frees map.

- ```ruby
    Pointer map_find(Map map, Pointer value){
    ```    
    This function hashes the value, finds the position and checks if the value matches exactly a mapnode. If a mapnode is found then it's returned. If not then NULL is returned.


### References


- Makefile is a modified version of "https://github.com/chatziko-k08/lecture-code" makefile.
- hash_string is the hash_string "https://github.com/chatziko-k08/lecture-code/blob/master/modules/UsingHashTable/ADTMap.c" starting at line 268.
- edit_distance function is the function "distance" used at "https://www.lemoda.net/c/levenshtein/" 



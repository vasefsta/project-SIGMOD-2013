![example workflow](https://github.com/vasiliki2000/project-SIGMOD-2013/actions/workflows/c.yml/badge.svg)
# project-SIGMOD-2013

 |Authors|sdi*******|
 |-------|----------|
 |Vasiliki Efstathiou|1800229|
 |Nikos Eftychiou|1800221|


 
### General Information:

- We decided to organize the code in five directories. 
  - programs : Containing the main function of program.
  - modules &nbsp; : Containing all functions needed by modules to work properly.
  - include &nbsp; &nbsp; : Containing all header files needed by source code.
  - tests &nbsp; &nbsp; &nbsp; &nbsp; : Containing the test functions to prove that every module works flawless.
  - misc &nbsp; &nbsp; &nbsp; &nbsp; : Containing all "sub functions" needed by program.
    

### ADTBKTREE

- struct bktree holds a bknode (the root of tree) and one compare function that will be either Hamming_distance or Edit_distance.

- struct bknode holds an entry (node) and a list of bknodes (children)

- #### insert(BKNode bkparent, BKNode new, CompareFunc compare)\
 At first, this function will calculate the distance (d) betweeen the new node and the current node (parent node). After that if current node has no children, new node will be his newborn child. If parent has children,
 we proceed at finding a child with distance (d') equal to (d). If such child is found we recursively call insert with child as parent. If no child is found we insert new node with it's siblings.


### Notes: 
- With list_test both ADTLinkedList and ADTEntryList are being checked.
- lookup_entry_word can check only one word at a time so we call this function for every word in a Document.



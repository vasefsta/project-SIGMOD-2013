![example branch parameter](https://github.com/vasiliki2000/project-SIGMOD-2013/actions/workflows/c.yml/badge.svg?branch=main)
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
- In programs/Makefile at line 3, you can pass the name of Documents to be read and the threshold.

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
 
### General Information:

- We decided to organize the code in five directories. 
  - programs : Containing the main function of program.
  - modules &nbsp; : Containing all functions needed by modules to work properly.
  - include &nbsp; &nbsp; : Containing all header files needed by source code.
  - tests &nbsp; &nbsp; &nbsp; &nbsp; : Containing the test functions to prove that every module works flawless.
  - misc &nbsp; &nbsp; &nbsp; &nbsp; : Containing all "sub functions" needed by program.
  - core &nbsp; &nbsp; &nbsp; &nbsp; : Containing all functions test_driver needs.

### Notes: 

- With list_test both ADTLinkedList and ADTEntryList are being checked.
- EndQuery removes thr query from all entries that this query exists. It does not remove the entry from index.

### References


- Makefile is a modified version of "https://github.com/chatziko-k08/lecture-code" makefile.
- hash_string is the hash_string "https://github.com/chatziko-k08/lecture-code/blob/master/modules/UsingHashTable/ADTMap.c" starting at line 268.
- edit_distance function is the function "distance" used at "https://www.lemoda.net/c/levenshtein/" 



#include "acutest.h"
#include "ADTBKTree.h"


void test_create(){
    BKTree bktree = create_bk_tree(MT_EDIT_DIST);

    TEST_ASSERT(bktree != NULL);

    bk_destroy(bktree, NULL);
}


void test_insert(){



}



TEST_LIST = {

	{ "bktree_create", test_create },
    // { "bktree_insert", test_insert },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 
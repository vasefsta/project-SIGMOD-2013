#include "ADTBKTree.h"
#include "ADTLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bknode {
    Pointer value;
    List children;
};

struct bktree {
    BKNode root;
    CompareFunc compare;
};

BKTree create_bk_tree(MatchType type){
    BKTree new_tree = malloc(sizeof(*new_tree));

    if (type == MT_EDIT_DIST)
        new_tree->compare = edit_distance;
    else if (type == MT_HAMMING_DIST)
        new_tree->compare = hamming_distance;
    else{
        free(new_tree);
        return NULL;
    }

    new_tree->root = NULL;
    
    return new_tree;

}   

ErrorCode insert_bknode(BKTree bktree, Pointer value){

}

void bk_destroy(BKTree bktree, DestroyFunc destroy){

}

int hamming_distance(Pointer value1, Pointer value2){
    String word1 = value1;

    String word2 = value2;

    if(strlen(word1) != strlen(word2))
        return -1;

    int hamming_dist = 0;


    for(int i = 0; i < strlen(word1); i++){
        if(word1[i] != word2[i])
            hamming_dist++;
    }

    return hamming_dist;

}

int edit_distance(Pointer value1, Pointer value2){
    

}
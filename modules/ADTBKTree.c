#include "ADTBKTree.h"
#include "ADTLinkedList.h"
#include "ADTEntryList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bknode {
    Pointer value;      //Pointer to struct Entry
    List children;      //List that value in nodes is a bknode
};

struct bktree {
    BKNode root;
    CompareFunc compare;    //hamming or edit
};


ErrorCode insert(BKNode bkparent, BKNode new, CompareFunc compare){
    int dist = compare(bkparent->value, new->value);
    if(!bkparent->children){
        bkparent->children = list_create();
        list_insert(bkparent->children, new);
        return EC_SUCCESS;
    }

    ListNode node;
    BKNode child;

    for(node = list_first(bkparent->children); node != NULL; node = list_find_next(node)){
        child = list_node_value(node);
        if(compare(child->value, bkparent->value) == dist)
            break;
    }

    if(!node){
        list_insert(bkparent->children, new);
        return EC_SUCCESS;
    } else {
        return insert(child, new, compare);

    }

}

Pointer bk_node_value(BKNode node){
    return node->value;
}

BKNode find(BKNode bkparent, CompareFunc compare, Pointer value) {
    int dist = compare(bkparent->value, value);
    if(dist == 0)
        return bkparent;

    if(bkparent->children == NULL){
        return NULL;
    }

    ListNode node;
    BKNode child; 

    for(node = list_first(bkparent->children); node != NULL; node = list_find_next(node)){
        child = list_node_value(node);
        if(compare(child->value, bkparent->value) == dist)
            break;
    }

    if(!node){
        return NULL;
    } else {
        return find(child, compare, value);
    }

}



BKTree bk_create(MatchType type){
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

ErrorCode bk_insert(BKTree bktree, Pointer value){

    BKNode new = malloc(sizeof(*new));
    new->children = NULL;
    new->value = value;
    if(bktree->root == NULL){
        bktree->root = new;
        return EC_SUCCESS;
    }

    int res = insert(bktree->root, new, bktree->compare);

    return res;
}

BKNode bk_find (BKTree bktree, Pointer value) {
    if(bktree->root == NULL){
        return NULL;
    }
    return find(bktree->root, bktree->compare, value);
}


void destroy(BKNode bknode, DestroyFunc destroy_value){
    if (!bknode)
        return;

    if ((bknode->children)) {
        for (ListNode node = list_first(bknode->children);
            node != NULL;
            node = list_find_next(node)) {
                BKNode bknode = list_node_value(node);
                destroy(bknode, (DestroyFunc)destroy_value); 
        }
                
        list_destroy(bknode->children, NULL);
    } 

    if (destroy_value)
        destroy_value(bknode->value);
    
    free(bknode);
}

void bk_destroy(BKTree bktree, DestroyFunc destroy_value){
    destroy(bktree->root, (DestroyFunc)destroy_value);
    free(bktree);
}


//compare functions 

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

int find_min(int one, int two, int three){
    int min = one;

    if(two < min)
        min = two;

    if (three < min)
        min = three;

    return min;
}

int edit_distance(Pointer value1, Pointer value2) {
    Entry e1 = value1;
    Entry e2 = value2;


    String word1 = e1->word;
    String word2 = e2->word;

    int len1 = strlen(word1);
    int len2 = strlen(word2);

    int array[len1 + 1][len2 + 1];  
    
    for(int i = 0; i < len1 + 1; i++){
        array[i][0] = i;
    } 
    for(int i = 0; i < len2 + 1; i++){
        array[0][i] = i;
    } 
    int value;

    for(int i = 1; i < len1 + 1; i++){
        for(int j = 1; j < len2 + 1; j++){
            if(word1[i] == word2[j])
                value = find_min(array[i-1][j], array[i][j-1], array[i-1][j-1]);
            else{
                value = find_min(array[i-1][j], array[i][j-1], array[i-1][j-1]) + 1;
            }
            array[i][j] = value;
        }
    }

    return array[len1][len2];


}

#include "ADTBKTree.h"
#include "ADTLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct bknode {
    Entry entry;      //Entry to struct Entry
    List children;      //List that value in nodes is a bknode
};

struct bktree {
    BKNode root;
    CompareFunc compare;    //hamming or edit
};


ErrorCode insert(BKNode bkparent, BKNode new, CompareFunc compare){
    int dist = compare(bkparent->entry->word, new->entry->word);

    if(!bkparent->children){
        bkparent->children = list_create();
        list_insert(bkparent->children, new);
        return EC_SUCCESS;
    }

    ListNode node;
    BKNode child;

    for (node = list_first(bkparent->children); node != NULL; node = list_find_next(node)){
        child = list_node_value(node);
        if(compare(child->entry->word, bkparent->entry->word) == dist)
            break;
    }

    if(!node){
        list_insert(bkparent->children, new);
        return EC_SUCCESS;
    } else {
        return insert(child, new, compare);
    }
}


int find(BKNode bkparent, CompareFunc compare, EntryList entrylist, String word, int threshold) {
    int dist_value_parent = compare(bkparent->entry->word, word);
    int low_range = dist_value_parent - threshold;

    if (low_range < 0)
        low_range = 0;

    if( (dist_value_parent <= threshold) && (dist_value_parent >= 0) )
        add_entry(entrylist, bkparent->entry);

    if(bkparent->children == NULL){
        return -1;
    }

    for(ListNode node = list_first(bkparent->children); node != NULL; node = list_find_next(node)){
        BKNode child = list_node_value(node);
        int dist_parent_child = compare(child->entry->word, bkparent->entry->word);

        if(dist_value_parent < 0){
            find(child, compare, entrylist, word, threshold);
        }
        else if ( (dist_parent_child <= dist_value_parent + threshold) && (dist_parent_child >= low_range)){
            find(child, compare, entrylist, word, threshold);            
        }
    }
    return 0;
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
        destroy_value(bknode->entry);
    
    free(bknode);
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


ErrorCode bk_insert(BKTree bktree, Entry entry){

    BKNode new = malloc(sizeof(*new));
    new->entry = entry;
    new->children = NULL;

    if(bktree->root == NULL){
        bktree->root = new;
        return EC_SUCCESS;
    }

    int res = insert(bktree->root, new, bktree->compare);

    return res;
}

Entry bk_node_value(BKNode node){
    return node->entry;
}

int bk_find(BKTree bktree, EntryList entrylist, String word, int n) {
    if(bktree->root == NULL){
        return -1;
    }

    return find(bktree->root, bktree->compare, entrylist, word, n);
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
    String word1 = value1;
    String word2 = value2;

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

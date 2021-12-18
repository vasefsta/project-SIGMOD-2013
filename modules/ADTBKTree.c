/*
 *  Authors: Vasiliki Efstathiou, Nikos Eftychiou
 *  File   : ADTBKTree.c     
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ADTBKTree.h"
#include "ADTLinkedList.h"
#include "misc.h"


struct bknode {
    Entry entry;      // Holds an entry.
    List children;      // A list that contains bknodes that are the children of a bk-parent node.
};

struct bktree {
    BKNode* root;           // It is the root of the bktree and it is NULL if bktree is empty.
    CompareFunc compare;    // Holds a pointer to the functions edit_distance or hamming_distance.
    MatchType type;         // The matchtype of the bktree.
    int size;
};


// Destroys a BKNode.
void destroy_bk_node(BKNode node){
    // Destroys the entry
    free(node->entry->word);
    list_destroy(node->entry->payload, NULL);

    free(node->entry);
    
    // Destroys the list of children.
    if(node->children)
        list_destroy(node->children, NULL);
    
    free(node);
}


// A helpful function for the bk_insert function.
// Returns the appropriate EC_SUCCESS.
ErrorCode insert(BKTree bktree,BKNode bkparent, BKNode new, CompareFunc compare){                           
    
    // If the new->entry->word and the bkparent->word are equal, then 
    // the query of the new->entry, is added in the bkparent->entry->payload.
    if(strcmp(bkparent->entry->word, new->entry->word) == 0){
        Query query = list_node_value(list_first(new->entry->payload));     // returns the query of the 
                                                                            // new->entry->payload
        
        list_insert(bkparent->entry->payload, query);       // insert the query of th new->entry in the 
                                                            // bkparent->entry->payload 
        destroy_bk_node(new);       // the new node is destroyed
       
        return EC_SUCCESS;
    }

    // If parent has no children, then it creates 
    // a new list, which will holds the children of the parent.
    // After that it inserts new as child in the list of children.
    if(!bkparent->children){                                                                  
        bkparent->children = list_create(NULL);
        list_insert(bkparent->children, new);
        bktree->size++;
        return EC_SUCCESS;
    }

    ListNode node;      // node that traverse the list of children
    BKNode child;       // holds the node value which is the bknode
    int dist = compare(bkparent->entry->word, new->entry->word);        // Get dist between new and bkparent

    // Traverse in list of children of bkparent
    for (node = list_first(bkparent->children); node != NULL; node = list_find_next(node)) {    
        child = list_node_value(node);

        // If new has same distance as child with parent get Child node
        if(compare(child->entry->word, bkparent->entry->word) == dist) {    
            break;
        }
    }

    // If no child was found with such distance 
    // insert new as child of parent.
    // Else:
    // -> If entry with same word exists append the query of new in the existing entry.
    // -> Else call recursively the insert function with chile. 
    if(!node){                                                                                
        list_insert(bkparent->children, new);
        bktree->size++;
        return EC_SUCCESS;
    } else {
        if(strcmp(child->entry->word, new->entry->word) == 0){                                
            Query query = list_node_value(list_first(new->entry->payload));     // returns the query of the 
                                                                                // new->entry->payload

            list_insert(child->entry->payload, query);      // insert the query of th new->entry in the 
                                                            // bkparent->entry->payload
            destroy_bk_node(new);       // the new node is destroyed
            return EC_SUCCESS;
        } else 
            return insert(bktree, child, new, compare);                                                    
    }
}


int find(BKNode bkparent, CompareFunc compare, CompareFunc compare_query, Map map_result, 
List complete_queries, String word, int threshold) {         // Find entries with threshold and word.
    // Calculate distance between word and bkparent's entry word
    int dist_value_parent = compare(bkparent->entry->word, word);                              
    // Calculate ( d - n)
    int low_range = dist_value_parent - threshold;                                                        
    
    if (low_range < 0)
        low_range = 0;

    // if d <= n and d > 0
    if( (dist_value_parent <= threshold) && (dist_value_parent >= 0) ) {                                   
        // Get payload of entry.
        List bkpayload = bkparent->entry->payload;

        // For every query in entry.
        for (ListNode node = list_first(bkpayload); node != NULL; node = list_find_next(node)) {
            //Get query.
            Query query = list_node_value(node);

            // If d is <= n.
            if (dist_value_parent <= query->match_dist) {
                // Create dummy Special.
                struct special tmpspecial;
                tmpspecial.query = query;
                tmpspecial.words = NULL;
                // Check if dummy exists in map_result.
                Special special = map_find(map_result, &tmpspecial);
                
                // If special was not found.
                if (!special) {
                    // Create a new Special.
                    special = malloc(sizeof(*special));
                    special->query = query;
                    special->words = list_create((CompareFunc) strcmp);
                    
                    // Insert it to map_result.
                    map_insert(map_result, special);

                    // Insert entry->word to special->words.
                    list_insert(special->words, bkparent->entry->word);

                     // If size of special words is equal to query's length 
                    // means that the query fully matches this document.
                    if (list_size(special->words) == special->query->length) {
                        // Create a QueryID.
                        QueryID* queryid = malloc(sizeof(*queryid));
                        *queryid = query->queryID;
                        // Insert queryID to complete_queries list.
                        list_insert(complete_queries, queryid);
                    }
                // If entry.word does not exist in special->words.
                // We check this so we do not store duplicate words
                // in special->words.
                } else if (!list_find(special->words, bkparent->entry->word)){
                    // Insert word to special->words/
                    list_insert(special->words, bkparent->entry->word);
                    // If size of special words is equal to query's length 
                    // means that the query fully matches this document.
                    if (list_size(special->words) == special->query->length) {
                        // Create a QueryID.
                        QueryID* queryid = malloc(sizeof(*queryid));
                        *queryid = query->queryID;
                        // Insert queryID to complete_queries list.
                        list_insert(complete_queries, queryid);
                    }
                }
            }

        }
    }

    // If bkparent has no children return -1.
    if (bkparent->children == NULL)
        return -1;

    // For every node in children
    for(ListNode node = list_first(bkparent->children); node != NULL; node = list_find_next(node)){
        // Get node value.
        BKNode child = list_node_value(node);
        // Calculate distance between child and parent.
        int dist_parent_child = compare(child->entry->word, bkparent->entry->word);                      
        // If distance between child and oarent is <= distance between new node and parent + threshold
        // and distance between child and parent is >= distance between new node and parent - threshold.
        // Call function recursively with child as parent.
        if ( (dist_parent_child <= dist_value_parent + threshold) && (dist_parent_child >= low_range))   
            find(child, compare, compare_query, map_result, complete_queries, word, threshold);
    }

    return 0;
}

Entry help_find_entry(BKNode bkparent, CompareFunc compare, String word) {
    // Calculate distance between word and bkparent's entry word
    int dist_value_parent = compare(bkparent->entry->word, word);
    // Calculate ( d - n)                              
    int low_range = dist_value_parent - 0;    

    if (low_range < 0)
        low_range = 0;

    // if d <= n and d > 0
    // Insert bkparent's entry in entrylist
    if( (dist_value_parent <= 0) && (dist_value_parent >= 0) ){                                  
        return bkparent->entry;                                                                   
    }
    // If parent has no children
    if(bkparent->children == NULL){                                                                      
        return NULL;
    }

    // For every child of parent.
    for(ListNode node = list_first(bkparent->children); node != NULL; node = list_find_next(node)){       
        // Get listNode value.
        BKNode child = list_node_value(node);
        // Calculate d for parent and child

        int dist_parent_child = compare(child->entry->word, bkparent->entry->word);
        // If distance of child and parent is in range ([d-n], [d+n])
        if ( (dist_parent_child <= dist_value_parent + 0 ) && (dist_parent_child >= low_range))    
            // Call recursice for child
            return help_find_entry(child, compare, word);                                             
    }
    // If no children were found return NULL.
    return NULL;
}

void destroy(BKNode bknode, DestroyFunc destroy_value){                        
    // Destroy bknode and its value
    if (!bknode)
        return;

    // If bknode has children
    if ((bknode->children)) {                             
        // For every child in list.                      
        for (ListNode node = list_first(bknode->children); node != NULL; node = list_find_next(node)) {
            // Get listNode value.
            BKNode bknode = list_node_value(node);
            // Destroy bknode.
            destroy(bknode, (DestroyFunc)destroy_value);
        }
        // Destroy list of children
        list_destroy(bknode->children, NULL);                                   
    } 
    // If destroy function and entry exists, destroy entry.
    if (destroy_value && bknode->entry)
        destroy_value(bknode->entry);
    
    // Free bknode.
    free(bknode);
}



BKTree bk_create(MatchType type) {
    // Create a new bktree.
    BKTree new_tree = malloc(sizeof(*new_tree));        
    
    // If type is MT_EDIT_DIST.
    if (type == MT_EDIT_DIST) {
        //Assign values.
        new_tree->compare = edit_distance;
        new_tree->root = malloc(sizeof(*new_tree->root));
        *(new_tree->root) = NULL;
    }                 
    // If type is MT_HAMMING_DIST.        
    else if (type == MT_HAMMING_DIST) {
        // Assign values.
        new_tree->compare = hamming_distance;
        // Create an array of 28 positions. (One for every available word length)
        // and initializes it.
        new_tree->root = malloc(sizeof(*new_tree->root) * 28);
        for (int i = 0; i < 28; i++) 
            new_tree->root[i] = NULL;
    // If any other match type is given
    // free memory and return NULL.
    } else {      
        free(new_tree);
        return NULL;
    }
    
    new_tree->type = type;
    new_tree->size = 0;
    
    return new_tree;
}   

ErrorCode bk_insert(BKTree bktree, Entry entry){
    // Important: There is only one query in the entry->payload.

    // If bktree exists, continue.
    assert(bktree);

    // Create a new bknode which contains the entry
    BKNode new = malloc(sizeof(*new));                          
    new->entry = entry;
    new->children = NULL;

    // If type is MT_EDIT_DIST
    if (bktree->type == MT_EDIT_DIST) {
        // If root is NULL assign new as root.
        if (*(bktree->root) == NULL) {
            *(bktree->root) = new;
            bktree->size++;
            return EC_SUCCESS;
        }
        // If root is occupied, call insert to insert new in bktree.
        else 
            return insert(bktree, *bktree->root, new, bktree->compare);
    }
    // If type is MT_HAMMING_DIST.
    else if (bktree->type == MT_HAMMING_DIST) {
        // Pos starts from word length -4
        // Because the lower length is 4.
        int pos = strlen(entry->word) - 4;

        // If root is NULL assign new as root.
        if ((bktree->root[pos]) == NULL) {
            bktree->root[pos] = new;
            bktree->size++;
            return EC_SUCCESS;
        }
        // If root is occupied, call insert to insert new in bktree.
        else
            return insert(bktree, bktree->root[pos], new, bktree->compare);
    }

    return EC_FAIL;
}

int bk_size(BKTree bktree) {
    return bktree->size;
}

int bk_find(BKTree bktree, Map map_result, List complete_queries, CompareFunc compare_query, String word, int n) {
    // If bktree exists.
    assert(bktree);
    
    // If type is MT_EDIT_DIST.
    if (bktree->type == MT_EDIT_DIST) {
        // If root is empty return -1.
        if (*bktree->root == NULL)
            return -1;
        // If root is not empty.
        // call find to fill complete queries.
        else 
            return find(*bktree->root, bktree->compare, compare_query, map_result, complete_queries, word, n);
    }
    // If type is MT_HAMMING_DIST.
    else if (bktree->type == MT_HAMMING_DIST) {
        // Pos starts from word length -4
        // Because the lower length is 4.
        int pos = strlen(word) - 4;
        // If root is NULL return -1.
        if (bktree->root[pos] == NULL)
            return -1;
        // If root exists call find to fill complete queries.
        else 
            return find(bktree->root[pos], bktree->compare, compare_query, map_result, complete_queries, word, n);
    }

    return -1;
}

Entry bk_find_entry(BKTree bktree, String word) {
    // If bktree exists, continue.
    assert(bktree);
    // If type is MT_EDIT_DIST.
    if (bktree->type == MT_EDIT_DIST) {
        // If root is empty return NULL.
        if (*bktree->root == NULL)
            return NULL;
        // If root is occupied, call insert to insert new in bktree.
        else 
            return help_find_entry(*bktree->root, bktree->compare, word);
    }
    // If type is MT_HAMMING_DIST.
    else if (bktree->type == MT_HAMMING_DIST) {
        // Pos starts from word length -4
        // Because the lower length is 4.
        int pos = strlen(word) - 4;
        // If root is empty return NULL.
        if (bktree->root[pos] == NULL)
            return NULL;
        // If root is occupied, call insert to insert new in bktree.
        else 
            return help_find_entry(bktree->root[pos], bktree->compare, word);     // Return result of find
    }
    // If any other match type was given, return NULL.
    return NULL;
}


void bk_destroy(BKTree bktree, DestroyFunc destroy_value){
    assert(bktree);     

    // Call destroy to free recursively bknodes of bktree
    if (bktree->type == MT_EDIT_DIST)
        destroy(*bktree->root, (DestroyFunc)destroy_value);                  
    else if (bktree->type == MT_HAMMING_DIST) {
        // Call destroy for all bktrees of the array
        for (int i = 0; i < 28; i++) 
        destroy(bktree->root[i], (DestroyFunc)destroy_value);                  
    }
    
    // Free root node and bktree.   
    free(bktree->root);
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

int edit_distance(Pointer value1, Pointer value2) {
    String word1 = value1;
    String word2 = value2;

    int len1 = strlen(word1);
    int len2 = strlen(word2);

    int matrix[len1 + 1][len2 + 1];
    int i;
    for (i = 0; i <= len1; i++) {
        matrix[i][0] = i;
    }
    for (i = 0; i <= len2; i++) {
        matrix[0][i] = i;
    }
    for (i = 1; i <= len1; i++) {
        int j;
        char c1;

        c1 = word1[i-1];
        for (j = 1; j <= len2; j++) {
            char c2;

            c2 = word2[j-1];
            if (c1 == c2) {
                matrix[i][j] = matrix[i-1][j-1];
            }
            else {
                int minimum = matrix[i-1][j] + 1;
                if (matrix[i][j-1] + 1 < minimum) {
                    minimum = matrix[i][j-1] + 1;
                }
                if (matrix[i-1][j-1] + 1 < minimum) {
                    minimum = matrix[i-1][j-1] + 1;
                }
                matrix[i][j] = minimum;
            }
        }
    }

    return matrix[len1][len2];
}

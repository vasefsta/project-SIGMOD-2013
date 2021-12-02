/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
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
    Entry entry;      //Entry to struct Entry
    List children;      //List that value in nodes is a bknode
};

struct bktree {
    BKNode* root;
    CompareFunc compare;    //hamming or edit
    MatchType type;
};


void destroy_bk_node(BKNode node){
    free(node->entry->word);
    list_destroy(node->entry->payload, NULL);
    free(node->entry);
    if(node->children)
        list_destroy(node->children, NULL);
    free(node);
}


ErrorCode insert(BKNode bkparent, BKNode new, CompareFunc compare){                           // Insert new in bktree.

    if(strcmp(bkparent->entry->word, new->entry->word) == 0){
        Query query = list_node_value(list_first(new->entry->payload));
        list_insert(bkparent->entry->payload, query);
        destroy_bk_node(new);
        return EC_SUCCESS;
    }

    int dist = compare(bkparent->entry->word, new->entry->word);          // Get dist between new and bkparent

    if(!bkparent->children){                                                                  // If parent has no children insert new as child
        bkparent->children = list_create(NULL);
        list_insert(bkparent->children, new);
        return EC_SUCCESS;
    }

    ListNode node;
    BKNode child;

    for (node = list_first(bkparent->children); node != NULL; node = list_find_next(node)){   // Traverse in list of children of bkparent 
        child = list_node_value(node);
        if(compare(child->entry->word, bkparent->entry->word) == dist){    // If new has same distance as child with parent get Child node
            break;
        }
    }

    if(!node){                                                                                // If no child was found with such distance insert new as child of parent
        list_insert(bkparent->children, new);
        return EC_SUCCESS;
    } else {
        if(strcmp(child->entry->word, new->entry->word) == 0){                                // If entry with same words exists append the ID of new in the existing entry.
            Query query = list_node_value(list_first(new->entry->payload));
            list_insert(child->entry->payload, query);
            destroy_bk_node(new);
            return EC_SUCCESS;
        } else {
            return insert(child, new, compare);                                                   // Call recursive for child and new
        }
    }
}


int find(BKNode bkparent, CompareFunc compare, CompareFunc compare_query, Map map_result, List complete_queries, String word, int threshold) {         // Find entries with threshold and word
int dist_value_parent = compare(bkparent->entry->word, word);                               // Calculate distance between word and bkparent's entry word
    int low_range = dist_value_parent - threshold;                                                        // Calculate ( d - n)

    if (low_range < 0)
        low_range = 0;

    if( (dist_value_parent <= threshold) && (dist_value_parent >= 0) ) {                                   // if d <= n and d > 0

        List bkpayload = bkparent->entry->payload;

        for (ListNode node = list_first(bkpayload); node != NULL; node = list_find_next(node)) {
            Query query = list_node_value(node);

            if (dist_value_parent <= query->match_dist) {
                struct special tmpspecial;

                tmpspecial.query = query;
                tmpspecial.times = 0;
                
                printf("START MAP FIND IN BKTREE = %d\n", query->queryID);

                Special special = map_find(map_result, &tmpspecial);

                printf("END MAP FIND IN BKTREE = %d\n", query->queryID);

                if (!special) {
                    special = malloc(sizeof(*special));

                    special->query = query;
                    special->times = 1;
                    
                    printf("START MAP INSERT IN BKTREE = %d\n", special->query->queryID);

                    map_insert(map_result, special);

                    printf("END MAP INSERT IN BKTREE = %d\n", special->query->queryID);

                } else if (special->times != special->query->length)
                    special->times++;

                if (special->times == special->query->length) {
                    // printf("QUERYYY IDDD BKTREE = %d\n", query->queryID);
                    ListNode node = list_find(complete_queries, &query->queryID);
                    // printf("QUERYYY IDDD BKTREE = %d\n", query->queryID);

                    if (!node) {
                        // printf("START LIST INSERT BKTREE = %d\n", query->queryID);
                        QueryID* queryid = malloc(sizeof(*queryid));

                        *queryid = query->queryID;
                        printf("START LIST INSERT BKTREE = %d\n", query->queryID);
                        list_insert(complete_queries, queryid);
                        printf("END LIST INSERT BKTREE = %d\n", query->queryID);

                    }
                }
            }

        }
    }

    if (bkparent->children == NULL)                                                                      // If parent has no children
        return -1;

    for(ListNode node = list_first(bkparent->children); node != NULL; node = list_find_next(node)){       // Traverse in list of children
        BKNode child = list_node_value(node);
        int dist_parent_child = compare(child->entry->word, bkparent->entry->word);                       // Calculate d for parent and child

        if ( (dist_parent_child <= dist_value_parent + threshold) && (dist_parent_child >= low_range))    // If distance of child and parent is in range ([d-n], [d+n])
            find(child, compare, compare_query, map_result, complete_queries, word, threshold);                                             // Call recursice for child
    }

    return 0;
}

Entry help_find_entry(BKNode bkparent, CompareFunc compare, String word) {
    int dist_value_parent = compare(bkparent->entry->word, word);                               // Calculate distance between word and bkparent's entry word
    int low_range = dist_value_parent;                                                        // Calculate ( d - n)

    if (low_range < 0)
        low_range = 0;

    if (dist_value_parent == 0){                                   // if d <= n and d > 0
        return bkparent->entry;
    }

    if (bkparent->children == NULL)                                                                      // If parent has no children
        return NULL;

    Entry entry;
    for(ListNode node = list_first(bkparent->children); node != NULL; node = list_find_next(node)){       // Traverse in list of children
        BKNode child = list_node_value(node);
        int dist_parent_child = compare(child->entry->word, bkparent->entry->word);                       // Calculate d for parent and child

        if ( (dist_parent_child <= dist_value_parent) && (dist_parent_child >= low_range))    // If distance of child and parent is in range ([d-n], [d+n])
            entry = help_find_entry(child, compare, word);                                             // Call recursice for child
    }

    return entry;
}

void destroy(BKNode bknode, DestroyFunc destroy_value){                         // Destroy bknode and its value
    if (!bknode)
        return;

    if ((bknode->children)) {                                                   // If bknode has children
        for (ListNode node = list_first(bknode->children);                      // Traverse children
            node != NULL;
            node = list_find_next(node)) {
                BKNode bknode = list_node_value(node);
                destroy(bknode, (DestroyFunc)destroy_value);                    // Call recursive function for child
        }
                
        list_destroy(bknode->children, NULL);                                   // Destroy list of children
    } 

    if (destroy_value && bknode->entry)
        destroy_value(bknode->entry);
    
    free(bknode);
}



BKTree bk_create(MatchType type) {
    BKTree new_tree = malloc(sizeof(*new_tree));                                // Create new bktree
    
    if (type == MT_EDIT_DIST) {
        new_tree->compare = edit_distance;
        new_tree->root = malloc(sizeof(*new_tree->root));

        *(new_tree->root) = NULL;
    }                         
    else if (type == MT_HAMMING_DIST) {
        new_tree->compare = hamming_distance;
        new_tree->root = malloc(sizeof(*new_tree->root) * 28);

        for (int i = 0; i < 28; i++) 
            new_tree->root[i] = NULL;
    }
    else{
        free(new_tree);
        return NULL;
    }

    new_tree->type = type;
    
    return new_tree;
}   


ErrorCode bk_insert(BKTree bktree, Entry entry){
    assert(bktree);

    BKNode new = malloc(sizeof(*new));                          // Create new bknode
    new->entry = entry;
    new->children = NULL;

    if (bktree->type == MT_EDIT_DIST) {
        if (*(bktree->root) == NULL) {
            *(bktree->root) = new;
            return EC_SUCCESS;
        }
        else 
            return insert(*bktree->root, new, bktree->compare);
    }
    else if (bktree->type == MT_HAMMING_DIST) {
        int pos = strlen(entry->word) - 4;

        if ((bktree->root[pos]) == NULL) {
            bktree->root[pos] = new;
            return EC_SUCCESS;
        }
        else
            return insert(bktree->root[pos], new, bktree->compare);
    }

    return EC_FAIL;
}

int bk_find(BKTree bktree, Map map_result, List complete_queries, CompareFunc compare_query, String word, int n) {
    assert(bktree);
    
    if (bktree->type == MT_EDIT_DIST) {
        if (*bktree->root == NULL)
            return -1;
        else 
            return find(*bktree->root, bktree->compare, compare_query, map_result, complete_queries, word, n);     // Return result of find
    }
    else if (bktree->type == MT_HAMMING_DIST) {
        int pos = strlen(word) - 4;

        if (bktree->root[pos] == NULL)
            return -1;
        else 
            return find(bktree->root[pos], bktree->compare, compare_query, map_result, complete_queries, word, n);     // Return result of find
    }

    return -1;
}

Entry bk_find_entry(BKTree bktree, String word) {
    assert(bktree);

    if (bktree->type == MT_EDIT_DIST) {
        if (*bktree->root == NULL)
            return NULL;
        else 
            return help_find_entry(*bktree->root, bktree->compare, word);     // Return result of find
    }
    else if (bktree->type == MT_HAMMING_DIST) {
        int pos = strlen(word) - 4;

        if (bktree->root[pos] == NULL)
            return NULL;
        else 
            return help_find_entry(bktree->root[pos], bktree->compare, word);     // Return result of find
    }

    return NULL;
}


void bk_destroy(BKTree bktree, DestroyFunc destroy_value){
    assert(bktree);

    if (bktree->type == MT_EDIT_DIST)
        destroy(*bktree->root, (DestroyFunc)destroy_value);                  // Call destroy to destroy bknodes of bktree
    else if (bktree->type == MT_HAMMING_DIST) {
        for (int i = 0; i < 28; i++) 
        destroy(bktree->root[i], (DestroyFunc)destroy_value);                  // Call destroy to destroy bknodes of bktree
    }
    
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

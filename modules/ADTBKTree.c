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

    BKNode new = malloc(sizeof(*new));
    new->children = NULL;
    new->value = value;

    if(bktree->root == NULL){
        bktree->root = new;
        return EC_SUCCESS;
    }

    return insert(bktree->root, new, bktree->compare);
}

ErrorCode insert(BKNode bkparent, BKNode new, CompareFunc compare){
    int dist = compare(bkparent->value, new->value);

    if(!bkparent->children){
        bkparent->children = list_create();
        list_insert(bkparent->children, new);
        return EC_SUCCESS;
    }

    ListNode node;
    BKNode child;

    for(node = list_first(bkparent); node != NULL; node = list_find_next(node)){
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

void bk_destroy(BKTree bktree, DestroyFunc destroy){
    destroy(bktree->root);
    free(bktree);

}

void destroy(BKNode bknode){

    if(!bknode)
        return;

    for(ListNode node = list_first(bknode->children); node != NULL; node = list_find_next(node)){
        BKNode temp = list_node_value(node);
        destroy(temp);
    }
    
    free(bknode->value);
    free(bknode);
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

int edit_distance(Pointer value1, Pointer value2){
    String word1 = value1;

    String word2 = value2;

    int len1 = strlen(word1);
    int len2 = strlen(word2);

    int matrix[len1 + 1][len2 + 1];         //Create array

    for (int i = 0; i <= len1; i++){        //Initialize for word1 and NULL ex. '' and 'word1' [1, 2, 3, 4, 5]
        matrix[i][0] = i;
    }
    for (int i = 0; i <= len2; i++){        //initialize for word2 and NULL ex. '' and 'word2  [1, 2, 3, 4, 5]
        matrix[0][i] = i;
    }

    for (int i = 1; i <= len1; i++){        //Lopp through word letter to letter.
        char c1;

        c1 = word1[i - 1];                  //get letter from word1.

        for (int j = 1; j <= len2; j++){    //compare with letters from word2.
            char c2;

            c2 = word2[j - 1];              //get letter from word2.
            if (c1 == c2){                  //if letters are same no further action needed
                matrix[i][j] = matrix[i - 1][j - 1];
            } else {                        
                int delete;
                int insert;
                int substitute;
                int minimum;

                delete = matrix[i - 1][j] + 1;  
                insert = matrix[i][j - 1] + 1;  
                substitute = matrix[i - 1][j - 1] + 1;  
                minimum = delete;                       //find min from those 3
                if (insert < minimum){
                    minimum = insert;
                }
                if (substitute < minimum){
                    minimum = substitute;
                }
                matrix[i][j] = minimum;     //matrix in this position is the min.
            }
        }
    }
    return matrix[len1][len2];
}
/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : misc.c     
*/

#include "common_types.h"
#include "core.h"
#include "ADTLinkedList.h"
#include "ADTEntryList.h"
#include "ADTMap.h"
#include "ADTIndex.h"
#include "misc.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int hash_query(Query q){
    return q->queryID;
}

int hash_func(Query query){
    return hash_string(query->words);
}

Query convert_to_query(String string){ 
    Query query = malloc(sizeof(*query));               // Create query
    query->words = strdup(string);
    int count = 0;

    String token = strtok(string, " \t\n");

    while (token != NULL ){                             // Count number of words
        count++;
        token = strtok(NULL, "  \t\n");   
    }
    
    query->length = count;

    return query;
}

String *Seperate_sentence(Query query){
    String *Array = malloc(sizeof(String)*query->length);           // Create array
    String dummy = strdup(query->words);                            // Create a dummy string

    int i  = 0;

    String token = strtok(dummy, " \t\n");

    Array[i] = strdup(token);


    while( (token = strtok(NULL, " \t\n")) != NULL){                // For every word in sentence
        i++;
        Array[i] = strdup(token);                                   // Copy to Array[i]
    }

    free(dummy);
    return Array;
}

List deduplicated_words_map(String doc_str){                          

    Map map = map_create((CompareFunc)strcmp, 450);           
    map_set_hash_function(map,(HashFunc)hash_string);

    List list_words = list_create((CompareFunc)strcmp);

    String dummy = strdup(doc_str);                            // Create a dummy string
    String token = strtok(dummy, " \t\n");
    String word =  strdup(token);

    if(!map_find(map, word)){
        map_insert(map, word);
        list_insert(list_words, word);
    } else 
        free(word);



    while( (token = strtok(NULL, " \t\n")) != NULL){                // For every word in sentence
        
        word = strdup(token);                                   // Copy to Array[i]
        
        if(!map_find(map, word)){
            map_insert(map, word);
            list_insert(list_words, word);
        } else  
            free(word);
    
    }
    
    free(dummy);
   
    map_destroy(map, NULL);
    return list_words;

}


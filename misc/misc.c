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

// int compare_query(QueryID* q1, QueryID* q2){
//     return *q1 - *q2;
// }


String path_of_doc(String namedoc) {
    String path = malloc(sizeof(*path)*(strlen(namedoc) + 19));

    strcpy(path, "../misc/documents/");
    strcat(path, namedoc);

    return path;
}


int times_in_list(EntryList entrylist, Query query) {                                           // Returns how many times the query is in entrylist

    int count = 0; 
    for(Entry entry = get_first(entrylist); entry != NULL; entry = get_next(entrylist, entry)){
        if(list_find(entry->payload, query) != NULL)
            count++;
    }
    return count;
}


int count_queries(String filename){
    FILE *FP = fopen(filename, "r");

    if(FP == NULL)
        return -1;

    size_t bytes;
    String buffer = NULL;
    size_t buffsize = MAX_QUERY_LENGTH;
    int count = 0;
    
    while((bytes = getline(&buffer, &buffsize, FP)) != -1 ){        // Read file line by line
        count++;
    }

    free(buffer);
    fclose(FP);
    return count;
}

int hash_query(Query q){
    return q->queryID;
}


// List unique_queries(EntryList entrylist, CompareFunc compare_query) {                  // Return a list of all queries in entrylist (queries are not duplicated)

//     Map map = map_create((CompareFunc) compare_query, 300);
//     map_set_hash_function(map, (HashFunc)hash_query);

//     List list_of_queries = list_create((CompareFunc) compare_query);

//     for(Entry entry = get_first(entrylist); entry != NULL; entry = get_next(entrylist, entry)){           //For every entry in entrylist
//         List list = entry->payload;
//         for(ListNode listnode = list_first(list); listnode != NULL; listnode = list_find_next(listnode)){ // For every query in payload
//             Query query = list_node_value(listnode);
//             if(map_find(map, query) == NULL){
//                 list_insert(list_of_queries, query);
//                 map_insert(map, query);
//             }
//         }
//     }

//     map_destroy(map, NULL);
//     return list_of_queries;
// }


// int comp_spec(Special s1, Special s2){
//     return s1->id - s2->id;
// }

// int hash_spec(Special s){
//     return s->id;
// }


// List find_complete_queries(EntryList entrylist, CompareFunc compare_query){

//     Map map = map_create((CompareFunc)comp_spec, 1000);
//     map_set_hash_function(map, (HashFunc)hash_spec);

//     List complete_list = list_create(compare_query);

//     for(ListNode listnode = list_first(entrylist); listnode != NULL; listnode = list_find_next(listnode)){
//         Entry entry = list_node_value(listnode);
        
//         for (ListNode node = list_first(entry->payload); node != NULL; node = list_find_next(node)){
//             Query query = list_node_value(node);
//             Special s1 = malloc(sizeof(*s1));
//             s1->id = query->queryID;
//             s1->times = 0;
//             Special S = map_find(map, s1);
//             if(S){
//                 S->times++;
//                 if(S->times == query->length){
//                     list_insert(complete_list, query);
//                 }
//                 free(s1);
//             } else{
//                 map_insert(map, s1);
//             }
//         }
//     }
//     map_destroy(map, free);

//     return complete_list;

// }


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


int hash_func(Query query){
    return hash_string(query->words);
}


// Map map_of_queries(String filename, EntryList entrylist, CompareFunc compare_query){
    
//     FILE *FP = fopen(filename, "r");                            // Open filename
//     if(FP == NULL)
//         return NULL;

//     size_t buffsize = MAX_QUERY_LENGTH;
//     String buffer = NULL;

//     size_t bytes;

//     int num_of_queries = count_queries(filename);
//     num_of_queries *= 1.2;

//     Map map = map_create( (CompareFunc) compare_query, num_of_queries);      // Create map for queries
//     map_set_hash_function(map, (HashFunc) hash_func);

//     int i = 0;
//     while((bytes = getline(&buffer, &buffsize, FP)) != -1 ){        // Read file line by line
//         i++;                                                        
//         buffer[strlen(buffer)-1] = '\0';
//         Query new_query = convert_to_query(buffer);                 // Create new query
//         new_query->queryID = i-1;
//         map_insert(map, new_query);                                 // Insert query in map

//         String *Array = Seperate_sentence(new_query);               // Get every word of query's sentence

//         for(int i = 0; i < new_query->length; i++){                 // For every word
//             Entry e1 = create_entry(Array[i], (CompareFunc) compare_query);   // Create a new entry          
//             Entry entry = find_entry(entrylist, e1);
//             if(entry != NULL){                                      // If entry with keyword was found
//                 list_insert(entry->payload, new_query);   // Inserty query in entry's payload
//                 free(Array[i]);
//             } else {
//                 entry = create_entry(Array[i], (CompareFunc) compare_query);  // Create new entry
//                 list_insert(entry->payload, new_query);               // Add query in entry's payload
//                 add_entry(entrylist, entry);                                    // Add entry in entrylist
//             }
//             List e1_payload = e1->payload;
//             list_destroy(e1_payload, NULL);                                     // Free dummy Pointers
//             free(e1);
//         }
    
//         free(Array);
//     }

    
//     free(buffer);
    
//     fclose(FP);

//     return map;

// }

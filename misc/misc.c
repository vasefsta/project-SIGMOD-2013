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

    ssize_t bytes;
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


List unique_queries(EntryList entrylist, CompareFunc compare_query) {                                                                // Return a list of all queries in entrylist (queries are not duplicated)

    List list_of_queries = list_create( (CompareFunc) compare_query);

    for(Entry entry = get_first(entrylist); entry != NULL; entry = get_next(entrylist, entry)){           //For every entry in entrylist
        List list = entry->payload;
        for(ListNode listnode = list_first(list); listnode != NULL; listnode = list_find_next(listnode)){ // For every query in payload
            Query query = list_node_value(listnode);
            if(list_find(list_of_queries, query) == NULL){
                list_insert(list_of_queries, query);
            }
        }
    }
    return list_of_queries;
}


List find_complete_queries(EntryList entrylist, CompareFunc compare_query){

    List complete_list = list_create((CompareFunc) compare_query);        

    List unique = unique_queries(entrylist, compare_query);        

    for(ListNode listnode = list_first(unique); listnode != NULL; listnode = list_find_next(listnode)){     // For every query
        Query query = list_node_value(listnode);
        int times = times_in_list(entrylist, query);            
        if(query->length == times)                      // If is as times as its length then it's full
            list_insert(complete_list, query);
    }

    list_destroy(unique, NULL);

    return complete_list;

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
    //Free string na ginete meta tin sinartisi
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

List deduplicated_words(String filename){                          
    FILE *FP = fopen(filename,"r");                                 //Open filename

    if(!FP){
        puts("Is null");
        return NULL;
    }

    char buffer[MAX_WORD_LENGTH+1];
    char letter[2];
    letter[1] = '\0';
    char a;


    List list = list_create((CompareFunc) strcmp);                  // Create list
            

    strcpy(buffer, "");

    while ((a = fgetc(FP)) != EOF){                                 // Read char by charfile
        if(a == ' ' || a == '\n'){                                  // If word is over
            if(!list_find(list, buffer)){                           // and not in list
                String value = strdup(buffer);
                list_insert(list, value);                           // Insert in list
            } 
            strcpy(buffer, "");
        } else {
            letter[0] = a;
            strcat(buffer, letter);                                 // Append char to String.
        }
    }

    fclose(FP);
    
    return list;

}

List deduplicated_words_map(String doc_str){                          

    char buffer[MAX_WORD_LENGTH+1];
    char letter[2];
    letter[1] = '\0';

    Map map = map_create((CompareFunc)strcmp, MAX_DOC_LENGTH);           
    map_set_hash_function(map,(HashFunc)hash_string);

    List list_words = list_create(strcmp);

    strcpy(buffer, "");

    int doc_size = strlen(doc_str);
    for (int i = 0; i < doc_size; i++){                                 // Read char by charfile
        char a = doc_str[i];
        if(a == ' ' || a == '\n'){                                  // If word is over
            if(!map_find(map, buffer)){                           // and not in list
                String value = strdup(buffer);
                map_insert(map, value);
                list_insert(list_words, value);
            } 
            strcpy(buffer, "");
        } else {
            letter[0] = a;
            strcat(buffer, letter);                                 // Append char to String.
        }
    }

    return map;

}


int hash_func(Query query){
    return hash_string(query->words);
}


Map map_of_queries(String filename, EntryList entrylist, CompareFunc compare_query){
    

    FILE *FP = fopen(filename, "r");                            // Open filename
    if(FP == NULL)
        return NULL;

    size_t buffsize = MAX_QUERY_LENGTH;
    String buffer = NULL;

    size_t bytes;

    int num_of_queries = count_queries(filename);
    num_of_queries *= 1.2;

    Map map = map_create( (CompareFunc) compare_query, num_of_queries);      // Create map for queries
    map_set_hash_function(map, (HashFunc) hash_func);

    int i = 0;
    while((bytes = getline(&buffer, &buffsize, FP)) != -1 ){        // Read file line by line
        i++;                                                        
        buffer[strlen(buffer)-1] = '\0';
        Query new_query = convert_to_query(buffer);                 // Create new query
        new_query->queryID = i-1;
        map_insert(map, new_query);                                 // Insert query in map

        String *Array = Seperate_sentence(new_query);               // Get every word of query's sentence

        for(int i = 0; i < new_query->length; i++){                 // For every word
            Entry e1 = create_entry(Array[i], (CompareFunc) compare_query);   // Create a new entry          
            Entry entry = find_entry(entrylist, e1);
            if(entry != NULL){                                      // If entry with keyword was found
                list_insert(entry->payload, new_query);   // Inserty query in entry's payload
                free(Array[i]);
            } else {
                entry = create_entry(Array[i], (CompareFunc) compare_query);  // Create new entry
                list_insert(entry->payload, new_query);               // Add query in entry's payload
                add_entry(entrylist, entry);                                    // Add entry in entrylist
            }
            List e1_payload = e1->payload;
            list_destroy(e1_payload, NULL);                                     // Free dummy Pointers
            free(e1);
        }
    
        free(Array);
    }

    
    free(buffer);
    
    fclose(FP);

    return map;

}

// const void destroy_query(Query q){
//     free(q->words);
//     free(q);
// }

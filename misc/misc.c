#include "common_types.h"
#include "core.h"
#include "ADTLinkedList.h"
#include "ADTEntryList.h"
#include "ADTMap.h"
#include "ADTIndex.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int compare_queries(Query q1, Query q2){
    return strcmp(q1->words, q2->words);
}

int times_in_list(EntryList entrylist, Query query) {

    int count = 0; 

    for(Entry entry = get_first(entrylist); entry != NULL; entry = get_next(entrylist, entry)){
        if(list_find(get_entry_payload(entry), query) != NULL)
            count++;
    }
    return count;
}


List unique_queries(EntryList entrylist) {

    List list_of_queries = list_create( (CompareFunc) compare_queries);

    for(Entry entry = get_first(entrylist); entry != NULL; entry = get_next(entrylist, entry)){
        List list = get_entry_payload(entry);

        for(ListNode listnode = list_first(list); listnode != NULL; listnode = list_find_next(listnode)){
            Query query = list_node_value(listnode);
            if(list_find(list_of_queries, query) == NULL){
                list_insert(list_of_queries, query);
            }
        }
    }

    return list_of_queries;

}


List find_complete_queries(EntryList entrylist){

    List complete_list = list_create((CompareFunc) compare_queries);

    List unique = unique_queries(entrylist);

    for(ListNode listnode = list_first(unique); listnode != NULL; listnode = list_find_next(listnode)){
        Query query = list_node_value(listnode);
        int times = times_in_list(entrylist, query);
        if(query->length == times)
            list_insert(complete_list, query);
    }

    list_destroy(unique, NULL);

    return complete_list;

}


Query convert_to_query(String string){
    Query query = malloc(sizeof(*query));
    query->words = strdup(string);
    int count = 0;

    String token = strtok(string, " \t\n");

    while (token != NULL ){
        count++;
        token = strtok(NULL, "  \t\n");   
    }
    
    query->length = count;

    return query;
    //Free string na ginete meta tin sinartisi
}

String *Seperate_sentence(Query query){
    String *Array = malloc(sizeof(String)*query->length);
    String dummy = strdup(query->words);

    int i  = 0;

    String token = strtok(dummy, " \t\n");

    Array[i] = strdup(token);


    while( (token = strtok(NULL, " \t\n")) != NULL){
        i++;
        Array[i] = strdup(token);
    }

    free(dummy);
    return Array;
}

List deduplicated_words(String filename){
    FILE *FP = fopen(filename,"r");

    if(!FP){
        puts("Is null");
        return NULL;
    }
    


    char buffer[MAX_WORD_LENGTH+1];
    char letter[2];
    letter[1] = '\0';
    char a;


    List list = list_create((CompareFunc) strcmp);
            

    strcpy(buffer, "");

    while ((a = fgetc(FP)) != EOF){
        if(a == ' ' || a == '\n'){
            if(!list_find(list, buffer)){
                String value = strdup(buffer);
                list_insert(list, value);
            } 
            strcpy(buffer, "");
        } else {
            letter[0] = a;
            strcat(buffer, letter);
        }
    }

    fclose(FP);
    
    return list;

}


int hash_func(Query query){
    return hash_string(query->words);
}

int compare_entries(Entry e1, Entry e2){
    return (strcmp(get_entry_word(e1), get_entry_word(e2)));
}

Map map_of_queries(String filename, EntryList entrylist){
    

    FILE *FP = fopen(filename, "r");
    if(FP == NULL)
        return NULL;

    size_t buffsize = MAX_QUERY_LENGTH;
    String buffer = NULL;

    size_t bytes;


    Map map = map_create( (CompareFunc) compare_queries, 120);
    map_set_hash_function(map, (HashFunc) hash_func);
    int i = 0;
    while((bytes = getline(&buffer, &buffsize, FP)) != -1 ){
        i++;
        buffer[strlen(buffer)-1] = '\0';
        Query new_query = convert_to_query(buffer);
        map_insert(map, new_query);

        String *Array = Seperate_sentence(new_query);

        for(int i = 0; i < new_query->length; i++){
            Entry e1 = create_entry(Array[i], (CompareFunc) compare_queries);            
            Entry entry = find_entry(entrylist, e1);
            if(entry != NULL){
                list_insert(get_entry_payload(entry), new_query);
                free(Array[i]);
            } else {
                entry = create_entry(Array[i], (CompareFunc) compare_queries);
                list_insert(get_entry_payload(entry), new_query);
                add_entry(entrylist, entry);
            }
            List e1_payload = get_entry_payload(e1);
            list_destroy(e1_payload, NULL);
            free(e1);
        }
    
        free(Array);
    }

    
    free(buffer);
    
    fclose(FP);

    return map;

}

const void destroy_query(Query q){
    free(q->words);
    free(q);
}

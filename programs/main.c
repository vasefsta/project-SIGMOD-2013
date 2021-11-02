#include "common_types.h"
#include "core.h"
#include "ADTLinkedList.h"
#include "ADTEntryList.h"
#include "ADTMap.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Query convert_to_query(String string){
    Query query = malloc(sizeof(*query));

    query->words = strdup(string);

    int count = 0;

    String token = strtok(string, " ");

    while (token != NULL){
        count++;
        token = strtok(NULL, " ");   
    }
    
    query->length = count;

    return query;
    //Free string na ginete meta tin sinartisi
}

String *Seperate_sentence(Query query){
    String *Array = malloc(sizeof(String)*query->length);
    String dummy = strdup(query->words);

    int i  = 0;

    String token = strtok(dummy, " ");

    Array[i] = strdup(token);


    while( (token = strtok(NULL, " ")) != NULL){
        i++;
        Array[i] = strdup(token);
    }

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


    List list = list_create();
            

    strcpy(buffer, "");

    while ((a = fgetc(FP)) != EOF){
        if(a == ' '){
            if(!list_find(list, (CompareFunc) strcmp, buffer)){
                printf("Inserting to list %s ", buffer);
                String value = strdup(buffer);
                list_insert(list, value);
                strcpy(buffer, "");
            }

        } else {
            letter[0] = a;
            strcat(buffer, letter);
        }
    }

    fclose(FP);
    
    return list;

}

const int compare(Query query1, Query query2){
    return strcmp(query1->words, query2->words);
}

int hash_func(Query query){
    return hash_string(query->words);
}

Map map_of_queries(String filename, EntryList entrylist){
    

    FILE *FP = fopen(filename, "r");
    if(FP == NULL)
        return NULL;

    size_t buffsize = MAX_QUERY_LENGTH;
    String buffer = NULL;

    size_t bytes;


    Map map = map_create( (CompareFunc) compare, 120);
    map_set_hash_function(map, (HashFunc) hash_func);
    int i = 0;
    while((bytes = getline(&buffer, &buffsize, FP)) != -1 ){
        i++;
        Query new_query = convert_to_query(buffer);
        map_insert(map, new_query);

        String *Array = Seperate_sentence(new_query);

        for(int i = 0; i < new_query->length; i++){
            ListNode node = list_find(entrylist, (CompareFunc) strcmp, Array[i]);
            Entry entry;
            if(node != NULL){
                entry = list_node_value(node);
                list_insert(entry->payload, new_query);
            } else {
                entry = create_entry(Array[i]);
                entry->payload = list_create();
                list_insert(entry->payload, new_query);
            }
        }

    }
    
    free(buffer);
    
    fclose(FP);

    return map;

}

void destroy_query(Query q){
    free(q->words);
    free(q);
}

int main(){

    // EntryList entrylist = create_entry_list();
    // Map map = map_of_queries("../misc/queries.txt", entrylist);


    // map_destroy(map,(DestroyFunc) destroy_query);

    // List list = deduplicated_words("../misc/documents/Document1");
    
    // list_destroy(list, (DestroyFunc) free);

    // destroy_entry_list(entrylist);

    return 0;
}
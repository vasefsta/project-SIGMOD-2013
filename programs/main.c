#include "common_types.h"
#include "core.h"
#include "ADTLinkedList.h"
#include "ADTEntryList.h"
#include "ADTMap.h"
#include "ADTIndex.h"


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


    List list = list_create();
            

    strcpy(buffer, "");

    while ((a = fgetc(FP)) != EOF){
        if(a == ' ' || a == '\n'){
            if(!list_find(list, (CompareFunc) strcmp, buffer)){
                puts(buffer);
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

const int compare(Query query1, Query query2){
    return strcmp(query1->words, query2->words);
}

int hash_func(Query query){
    return hash_string(query->words);
}


const int compare_entry_string(String word, Entry e1){
    return (strcmp(e1->word, word));
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
            ListNode node = list_find(entrylist, (CompareFunc) compare_entry_string, Array[i]);
            Entry entry;
            if(node != NULL){
                entry = list_node_value(node);
                list_insert(entry->payload, new_query);
                free(Array[i]);
            } else {
                entry = create_entry(Array[i]);
                list_insert(entry->payload, new_query);
                add_entry(entrylist, entry);
            }
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


int main(){
    int err;

    EntryList entrylist = create_entry_list();
    EntryList result = create_entry_list();
    
    Map map = map_of_queries("../misc/queries.txt", entrylist);

    List list = deduplicated_words("../misc/documents/Document1");

    Index index_exact = create_index(MT_EXACT_MATCH, 100);



    err = build_entry_index(index_exact, entrylist);

    if(err != EC_SUCCESS){
        perror("Exiting...");
        return -1;
    }

    
    // printf("Number of entries in result = %d\n", get_number_entries(entrylist));


    // err = lookup_entry_index(index_exact, "sotira", 0, result);    


//###################################################################

    list_destroy(list, (DestroyFunc) free);

    destroy_entry_index(index_exact);

    destroy_entry_list(entrylist, (DestroyFunc) destroy_entry);

    map_destroy(map,(DestroyFunc) destroy_query);

    return 0;
}
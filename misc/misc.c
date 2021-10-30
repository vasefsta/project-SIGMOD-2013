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

List deduplicated_words(String filename){
    FILE *FP = fopen(filename,"r");

    if(!FP)
        return NULL;
    


    char buffer[MAX_WORD_LENGTH+1];
    char letter[2];
    letter[1] = '\0';
    char a;


    List list = list_create();

    strcpy(buffer, "");

    while ((a = fgetc(FP)) != EOF){
        if(a == ' '){
            
            if(!list_find(list, (CompareFunc) strcmp, buffer)){
                printf("Inserting to list%s ", buffer);
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

const int compare(Entry entry, String value){
    return strcmp(entry->word, value);
}

 map_of_queries(String filename){

    FILE *FP = fopen(filename, "r");
    if(FP == NULL)
        return NULL;

    long int buffsize = MAX_QUERY_LENGTH;
    String buffer = NULL;

    size_t bytes;

    EntryList entrylist = create_entry_list();

    while((bytes = getline(&buffer, &buffsize, FP)) != -1 ){
        Query new_query = convert_to_query(buffer);



    }

    return NULL;

}



int main(){
   

    map_of_queries("./queries.txt");

    // List list = deduplicated_words("./documents/Document1");
    

    // list_destroy(list, (DestroyFunc) free);


}
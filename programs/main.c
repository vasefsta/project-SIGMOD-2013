#include "common_types.h"
#include "core.h"
#include "ADTLinkedList.h"
#include "ADTEntryList.h"
#include "ADTMap.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// int countWords(char *string){

// 	int count = 0;

	
// 	for(int i = 0; i < strlen(string); i++){
// 		char a = string[i];

// 		if (a == ' '){
// 			count++;
// 		}
// 	}

// 	return count;
// }

// int readQueries(char *path){
// 	FILE *FP = fopen(path, "r");
	
// 	char line[MAX_QUERY_LENGTH+1];
	
// 	Query q;

// 	while (fgets(line, sizeof(line), FP)) {
//         //  q = malloc(sizeof(*q));
// 		//  q->words = strdup(line);
// 		printf("%s", line);
// 		char *temp = strdup(line);
// 		printf("%d\n", countWords(temp));	

//     }


// 	fclose(FP);

// }


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

 Map map_of_queries(String filename){

    FILE *FP = fopen(filename, "r");
    if(FP == NULL)
        return NULL;

    size_t buffsize = MAX_QUERY_LENGTH;
    String buffer = NULL;

    size_t bytes;

    //EntryList entrylist = create_entry_list();

    while((bytes = getline(&buffer, &buffsize, FP)) != -1 ){
      //  Query new_query = convert_to_query(buffer);
    }

    return NULL;

}

int main(){
   
    Map map = map_of_queries("../misc/queries.txt");

    List list = deduplicated_words("../misc/documents/Document1");
    
    list_destroy(list, (DestroyFunc) free);

    map_destroy(map, NULL);
}
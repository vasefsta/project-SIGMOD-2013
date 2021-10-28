#include "common_types.h"
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
    //Free string na ginete meta tin sinartisi
}


int main(){
    char temp[8] = "Ena Dio";
    char *string = strdup(temp);

    Query new = convert_to_query(string);
    free(string);

    printf("%d\n", new->length);
    puts(new->words);

    free(new->words);

    free(new);
}
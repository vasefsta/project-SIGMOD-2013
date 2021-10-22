#include <stdio.h>
#include <string.h>
#include "../include/core.h"
#include "../include/common_types.h"

int countWords(char *string){

	int count = 0;

	
	for(int i = 0; i < strlen(string); i++){
		char a = string[i];

		if (a == ' '){
			count++;
		}
	}

	return count;
}

int readQueries(char *path){
	FILE *FP = fopen(path, "r");
	
	char line[MAX_QUERY_LENGTH+1];
	
	Query q;

	while (fgets(line, sizeof(line), FP)) {
        //  q = malloc(sizeof(*q));
		//  q->words = strdup(line);
		printf("%s", line);
		char *temp = strdup(line);
		printf("%d\n", countWords(temp));	

    }


	fclose(FP);

}


int main(int argc, char *argv[]){
	readQueries("../misc/queries.txt");
}

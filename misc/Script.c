#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char Array[40][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
    "sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
    "rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip" }; 


    for(int i = 0; i < 100; i++){

        int times = rand() % 5;
        times++;

        char String[100];
        strcpy(String, "");


        for (int k = 0; k < times; k++){
            int num = rand()%40;
            
            strncat(String, Array[num], strlen(Array[num])+1);
            strcat(String, " ");
        }
        puts(String);

    }




}
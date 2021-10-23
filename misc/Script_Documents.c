#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char Array[93][31] = {"where", "flower", "done", "wonderful", "coffee", "shop", "mall", "hospital", "hello", "world", "beautiful", "water", "music", "chocolate",
    "sunny", "beach", "summer", "winter", "autumn", "spring", "diving", "cream", "sotira", "book", "potatoes", "poetry", "athletes", "salad" , "lobster", "forest",
    "rock", "metal", "jazz", "guitar", "simple", "love", "hate", "kiss","friends", "trip", "feta", "football", "basketball", "handball", "soft", "park", "walk",
    "popular", "micro", "chips", "ketchup", "mayo", "clothes", "longsleeve", "jacket", "player", "barney", "dinosaurs", "phone", "socks", "manager", "pirates",
    "carribean", "spray", "hookah", "chair", "sofa", "basket", "apple", "banana", "orange", "peach", "strawberry", "blue", "pink", "yellow", "brown", "black",
    "white", "shadow", "jumbo", "public", "private", "glass", "plastic", "balcony", "floor", "plug", "piano", "electricity", "carbon", "bars", "portrait" }; 



    for(int i = 0; i < 1000; i++){

        int times = rand() %19;
        times+=3;

        char String[500];
        strcpy(String, "");


        for (int k = 0; k < times; k++){
            int num = rand()%93;
            
            strncat(String, Array[num], strlen(Array[num])+1);
            strcat(String, " ");
        }
        char f[15];
        strcpy(f, "Document");
        char buffer[5];
        sprintf(buffer, "%d", i);

        strcat(f, buffer);

        FILE *fp = fopen(f, "w+");

        fprintf(fp, String);
        fclose(fp);

    }




}
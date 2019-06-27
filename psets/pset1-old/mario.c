/*
Title: Mario
Description: For CS50x pset1
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/
#include <stdio.h>
#include <cs50.h>
int main(void){
    printf("Height: ");
    int height;
    height = GetInt(); // Height?
    while(height < 0 || height > 23) {
        // Check conditions
        printf("Retry:");
        height = GetInt(); // Ask Again
    }
    int l;
    for(l = 1; height + 1 != l; l++) {
        // Each line
        int s = 0;
        for(s = 0; s+1 != ((height + 1)-l); s++){
            // Spaces
            printf(" ");
        }
        int h = 0;
        for(h = 0;l+1 != h; h++){
            // Hashes
            printf("#");
        }
        printf("\n");
    }
}
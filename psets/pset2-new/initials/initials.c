/*
Title: Initials
Description: 2017 version
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    // Vars
    int first = 0;
    // Input
    string name = get_string();
    // Processing
    for (int i = 0; i < strlen(name); i++) { // Loop through it!
        if (first == 0 && isalpha(name[i])) { // If the first character hasn't been processed yet, and it is a char...
            first = 1; // Yep, already processed
            printf("%c", toupper(name[i]));
        }
        else {
            if (isspace(name[i]) && isalpha(name[i + 1]) && first == 1) { // If it's a space followed by a character.
                printf("%c", toupper(name[i+1])); // Then print the character next to it.
                i++; // Skip next char
            }
        }
    }
    printf("\n"); // new line
}
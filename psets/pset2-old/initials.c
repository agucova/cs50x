/*
Title: Initials
Description: Gives you the initials for a written name, cs50 pset2
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/
#include <cs50.h> // Include cs50, for GetString()
#include <stdio.h> // Incluse stdio, for printf
#include <string.h> // Include string, for strlen and toupper
#include <ctype.h> // Include ctype, for isspace

int main(void) {
    string fullname = GetString(); // Get the string
    int l = strlen(fullname); // Get length of the string
    printf("%c", toupper(fullname[0])); // Print the initial letter.
    for(int i = 0;l!=i;i++) { // Loop the entire string
        if(isspace(fullname[i])) { // If there is space on i
            printf("%c", toupper(fullname[i+1]));  // Then print the next letter with mayus.
        }
    }
    printf("\n"); // End the final line
}
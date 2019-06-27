/*
Title: Vegenere Cipher
Description: Version 2017
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[]) {
    // Vars
    int out; // Use an int to prevent ascii overflow in a char.
    // Input
    if (argc < 2 || argc > 2) { // Number of arguments check
        printf("Usage: ./vegenere key\n");
        return(1);
    }
    string key = argv[1]; // Define key
    for (int ki = 0; ki < strlen(key); ki++) { // Check if key alpha.
        if (!isalpha(key[ki])) {
            printf("Invalid key.\n");
            return(1);
        }
    }
    string plaintext = get_string("plaintext: "); // Prompt for plaintext.

    // Output
    printf("ciphertext: ");
    for (int character = 0, length = strlen(plaintext), ki = 0; character < length; character++) // Loop through the text to cipher
    {
        if (isalpha(plaintext[character])) { // It is an alpha character?
            if (isupper(plaintext[character])) { /// Only if it's upper
                int diff = tolower(key[ki]) - 97; // Calculate key displacement
                out = plaintext[character] + diff; // Add displacement over the character
                if (out > 90) { // Wrap around int
                    diff = out - 90;
                    out = 64 + diff;
                    }
            }
            else { /// Only if it's lower
                int diff = tolower(key[ki]) - 97;
                out = plaintext[character] + diff;
                if (out > 122) { // Wrap around int
                    diff = out - 122;
                    out = 96 + diff;
                    }
                }
            char cast = out; // Cast to use an int as a character.
            printf("%c", cast);
            ki++; // Add to counter of key position.
            if (ki == strlen(key)) { // Prevent key overflow.
                ki = 0;
            }
        }
        else {
            printf("%c", plaintext[character]); // If it's not alpha, don't bother.
        }
    }
    printf("\n"); // Final line!
}
/*
Title: ¡Heil Caesar!
Description: A highly complicated (inefficient) caesar shifting algorithm for cs50x pset2. 2017 Version.
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
    char out;
    // Input
    if (argc < 2) {
        printf("You must include an argument.");
        return(1);
    }
    int key = atoi(argv[1]);
    while (key > 26) {
        key = key - 26;
    }
    string plaintext = get_string("plaintext: ");
    // Output
    printf("ciphertext: ");
    for (int character = 0, length = strlen(plaintext); character < length; character++)
    {
        if (isalpha(plaintext[character])) { // It is a alpha character?
            if (isupper(plaintext[character])) { /// Only if it's upper
                out = plaintext[character] + key; // Sum one
                if (!isalpha(out)) { // Wrap around
                    int diff = out - 91;
                    out = 65 + diff;
                    }
                printf("%c", out);
            }
            else { /// Only if it's lower
                out = plaintext[character] + key; // Sum one
                if (!isalpha(out)) { // Wrap around
                    int diff = out - 123;
                    out = 97 + diff;
                    }
                printf("%c", out);
                }
        }
        else {
            printf("%c", plaintext[character]);
        }
    }
    printf("\n");
}
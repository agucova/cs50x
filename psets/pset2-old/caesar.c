/*
Title: ¡Heil Caesar!
Description: A highly complicated (inefficient) caesar shifting algorithm for cs50x pset2.
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/
#include <cs50.h> // Include cs50, for GetString()
#include <stdio.h> // Incluse stdio, for printf
#include <string.h> // Include string, for strlen
#include <ctype.h> // Include ctype, for isalpha, islower & isupper

int main(int argc, string argv[]) {
    int intReceived = 0; // Boolean (or number) for the do-while. 
    // Input validation (Check at the end)
     do
    {
        if(argv[1]==NULL) // If there's no provided argument (argv == null doesn't work)
        {
            printf("Input a integer, please :3\n");
            return 1; // :(
        }
        else
        {
            intReceived = 1; // Break the loop
        }
    } while(intReceived == 0);
    
    // Variables
    int shift = atoi(argv[1]); // Get the first argument. (0 is the command itself)
    string plain = GetString(); // Store the plain text!
    int len = strlen(plain); // Store the strlen of the plain text
    int mayus_start = 65; // ASCII starting point of mayus chars
    int minus_start = 97; // ASCII starting point of minus chars
    int alpha_length = 26; // Length of the alphabet
    // Actual shifting
    for (int i = 0; i < len; i++) { // Loop through the chars
        if(isalpha(plain[i])) { 
            if(islower(plain[i])) {
                int lower  = (((plain[i] - minus_start + shift) % alpha_length) + minus_start); // Wrap around
                printf("%c", lower); // Print the encrypted minus character
            }
            else if (isupper(plain[i])) {
                int higher = (((plain[i] - mayus_start + shift) % alpha_length) + mayus_start); // Wrap around
                printf("%c", higher); // Print the encrypted mayus character
            }
        }
        else {
            printf("%c", plain[i]); // If it's not alpha, don't do anything
        }
    }
    printf("\n"); // New line.
    return 0; // It worked :D
}
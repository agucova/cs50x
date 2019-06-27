/*
Title: Water
Description: Water Program of pset1 (CS50)
Author: Agustín Covarrubias
NOTE: This source has a lot of unnecesary comments.
*/
#include <stdio.h>
#include <cs50.h>
int main(void) {
    // prints "minutes:"
    printf("minutes: ");
    // Gets the minutes amount and stores it on a variable.
    int minutes = GetInt();
    // Converts minutes to ounces (according to the pset1 docs)
    int ounces = minutes * 192;
    // Calculate bottles by dividing the ounces by 16 (one bottle)
    int bottles = ounces/16;
    // Prints the result.
    printf("bottles: %i\n", bottles);
}

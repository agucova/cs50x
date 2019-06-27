/*
Title: Greedy
Description: For CS50x pset 1
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/
// Include the math library to use roundf()
#include <math.h>
#include <stdio.h>
#include <cs50.h>
int main(void){
    float change;
        do { // Keep asking if the answer is <= 0
            printf("How much change is owed \n"); 
            change = GetFloat(); 
    }
    while (change <= 0); 
    // Pass from dollars to cents (And from float to int), and round!
    int cents = roundf(change * 100); 
    int quarters = 0;
    int dimes = 0;
    int nickels = 0;
    // If it can be divided perfectly, do so.
    while ((cents/25) >= 1) {
        quarters++;
        cents = cents - 25;
    }
    while ((cents/10) >= 1) {
        dimes++;
        cents = cents - 10;
    }
    while ((cents/5) >= 1) {
        nickels++;
        cents = cents - 5;
    }
    // Sum the coins, (Pennies are 1, so no division needed)
    int coins = quarters + dimes + nickels + cents;
    printf("%i\n", coins);
}
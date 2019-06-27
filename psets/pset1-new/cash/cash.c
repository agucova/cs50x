/*
Title: Cash
Description: Version 2017 of greedy.
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/
#include <stdio.h>
#include <cs50.h>
#include <math.h>
int main(void) {
    // Vars
    float cash;
    int coins = 0;
    // Input
    do {
    cash = get_float("Change owed: ");
    }
    while(cash < 0);
    // Processing
    cash = round(cash * 100);
    while (cash >= 25) {
        cash = cash - 25;
        coins++;
    }
    while (cash >= 10) {
        cash = cash - 10;
        coins ++;
    }
    while (cash >= 5) {
        cash = cash - 5;
        coins ++;
    }
    while (cash >= 1) {
        cash = cash - 1;
        coins ++;
    }
    // Output
    printf("%i\n", coins);
}
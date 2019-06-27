/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

bool searchRec(int value, int values[], int middle, int upper, int lower);
/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    bool found = false;
    int upper = n-1;
    int lower = 0;
    int middle = lower + ((upper - lower) / 2);

    if(n < 0) { // Check for non-negative
        return false;
    }
    if (value == values[middle]) { // Did we find it already?
        return true;
    }
    else if (value < values[middle]) { // If it seems to farther down:
        upper = middle - 1; // The new upper limit is the old middle (exclusive)
        found = searchRec(value, values, middle, upper, lower); // Recursive Inception
    }
    else if (value > values[middle]) {
        lower = middle + 1; // The new lower limit is the old middle (exclusive)
        found = searchRec(value, values, middle, upper, lower); // Recursive Inception
    }
    return found;
}

bool searchRec(int value, int values[], int middle, int upper, int lower) {
    bool found = false;
    middle = lower + ((upper - lower) / 2);
    if (value == values[middle]) {
        return true;
    }
    else if (lower == upper) {
        return false;
    }
    else if (value < values[middle]) {
        upper = middle - 1;
        found = searchRec(value, values, middle, upper, lower);
    }
    else if (value > values[middle]) {
        lower = middle + 1;
        found = searchRec(value, values, middle, upper, lower);
    }
    return found;
}
/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int count[65536] = {0};
    int h = 0;
    int max;

    for(int r = 0; r < n; r++) { // For every item in the array
        count[values[r]]++; // Increment the number corresponding to values[r]
    }
    for(int i = 0; i < n; i++) { // Loop through array size
        if (values[i] > max) { // If it is bigger than the current max:
            max = values[i]; // Set it as the new max
        }
    }
    for(int i = 0; i <= max; i++) { // Loop through count size
        while (0 < count[i]) { // If there is a number to be added
            values[h] = i; // Append to array
            h++; // Increase array key
            count[i]--; // Check from to-do
        }
    }
    return;
}

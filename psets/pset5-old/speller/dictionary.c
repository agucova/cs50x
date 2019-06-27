/*
Title: Speller
Description: Functions for implementing a trie in a speller.
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/
#include "dictionary.h"

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Functions
int hashChar(char c);
// unsigned int count(node *traverse); // Recursive function deprecated
int freeThePointers(node *traverse);
unsigned int oCount;
bool load(const char *dictionary)
{
    root = malloc(sizeof(node)); // Initialize root node of trie.
    memset(root, 0, sizeof(node)); // Initialize values inside (see line 32)
    node *traverse = root; // Initialize traverse to root.
    FILE *dictf = fopen(dictionary, "r");
    for (int c = fgetc(dictf); c != EOF; c = fgetc(dictf)) // For each character in the dictionary til end of file.
    {
        if (c != '\n')
        {
            int ci = hashChar(c); // Use the mapping function
            if (ci <= 26)
            {
                if (traverse->children[ci] == NULL) // If the node for the character does not exist and it's not a newline
                {
                    traverse->children[ci] = malloc(sizeof(node)); // Create a node for it
                    memset(traverse->children[ci], 0, sizeof(node)); // Thanks Vipin Kumar from StackOverflow on this. (Initializes values inside)
                }
                traverse = traverse->children[ci]; // Set the traverse for the current char/node.
            }
        }
        else
        {
            traverse->isWord = true; // Change the tag at the end
            traverse = root; // Get back to the root of the trie.
            oCount += 1;
        }
    }
    fclose(dictf);
    return true;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *traverse = root;
    for (int i = 0; i < strlen(word); ++i) // Prefix opt.
    {
        int ci = hashChar(word[i]);
        if (ci != 28)
        {
            if (traverse->children[ci] != NULL)
            {
                traverse = traverse->children[ci];
            }
            else
            {
                return false;
            }
            if (i + 1 == strlen(word) && traverse->isWord == true)
            {
                return true;
            }
        }
    }
    return false;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return oCount; // Used to be a recursive function (see comments), but using a variable during load is much faster.
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    return freeThePointers(root); // Use another recursive function.
}

int hashChar(char c)
{
    c = tolower(c); // Get letter to lowercase
    int ci = c; // Get a variable representing the letter (or apostrophe) numerically.
    if (122 >= ci && ci >= 97) // If alphanumeric
    {
        return (ci - 97); // Map the variable to 0 to 25.
    }
    else if (c == '\'') // Else if it's the apostrophe
    {
        return 26; // Map it to 26
    }
    else
    {
        return 28; // Signal invalid character. Note, error catching was disabled for optimization.
    }
}

// unsigned int count(node *traverse) {
//     int number = 0;
//     for(int i = 0; i < 27; i++) { // For every children
//         if(traverse->children[i] != NULL) { // If the children is NULL
//             number += count(traverse->children[i]); // Add the values inside through recursion.
//         }
//     }
//     if(traverse -> isWord) { // If we have touched the end, add one!
//         number += 1;
//     }
//     return number;
// }

int freeThePointers(node *traverse) // Same recursion as the old count().
{
    for (int i = 0; i < 27; ++i) // Prefix opt.
    {
        if (traverse->children[i] != NULL)
        {
            freeThePointers(traverse->children[i]);
        }
    }
    free(traverse);
    return true;
}
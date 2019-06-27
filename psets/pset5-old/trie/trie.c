/*
Title: Trie
Description: Example of a trie implementation
Author: Agustín Covarrubias
WARNING: This source may have a lot of unnecesary comments.
*/
#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
typedef struct node {
    struct node *children[27];
    bool isEnd;
} node;

int size;

int main(void) {
    node *root = malloc(sizeof(node));
    node *traverse = root;
    // Get size
    do {
        size = get_int("How many words are you going to enter? ");
    } while (size < 1 && size != INT_MAX);

    // Get the words
    for(int i = 0; i < size; i++) {
        char *word = get_string("Enter a word. ");
        for(int j = 0; j < sizeof(*word); j++) {
            char character = tolower(word[j]);
            traverse->children[character - 98] = malloc(sizeof(node));
            traverse = traverse->children[character - 98];
        }
        traverse = root;
    }
    // for(int i = 0; i < 27; i++) {
    //     if(root->children[i]) {

    //     }
    // }

}
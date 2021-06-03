// Implements a dictionary's functionality

//I got some tips from Anthony Catantan's article on this web link https://medium.com/anthony-catantan/i-saw-the-light-my-experience-with-cs50-weeks-5-and-6-72c29a6230fb

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>

#include "dictionary.h"

// initialize hashtable size
#define HASHTABLE_SIZE 65536

int word_count;

//node is defined in dictionary header file
// define a hashtable
node* hashtable[HASHTABLE_SIZE] = {NULL};

/**
 * Hash function via reddit user delipity:
 * https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn
 */
// hashing function
int hashing_function(char* word)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
        hash = (hash << 2) ^ word[i];
    return hash % HASHTABLE_SIZE;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{

   //make a copy of word since word is constant and cannot be altered
    int n = strlen(word);
    int length = 0;
    for (int i = 0; i < n;  i++)
    {
        length++;
    }

    char word_copy[length +1];

    //convert words to lowercase
    for (int i = 0; i < length; i++)
    {
        word_copy[i] = tolower(word[i]);
    }

    //add the end of string character
    word_copy[n] = '\0';

    //hash the copy
    int index = hashing_function(word_copy);

    //check for error
    if (hashtable[index] == NULL)
    {
        return false;
    }

    else if (hashtable[index] != NULL)
    {
        node *cursor = hashtable[index];

        while (cursor != NULL)
        {
            //compare strings, return true if 0(which means theyre the same)
            if(strcasecmp(cursor->word, word)==0)
            {
                return true;
            }
            else
            {
                cursor = cursor->next;
            }
        }
    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    //open a file called dictioanry and check for if the file exists
    FILE* dict = fopen(dictionary, "r");

    //check if dictionary opens correctly
    if (dict == NULL)
    {
        printf("could not open dictionary file\n");
        return false;
    }


    //variable to store words from dictionary in
    char word[LENGTH +1];


    //scan the words in the dictionary and copy into the variable word and execute until the end of file
    while (fscanf(dict, "%s", word) != EOF)
    {
        //malloc memory space for new node and check for error
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        //copy word into node
        strcpy(new_node -> word, word);

        //variable that stores the return of the hased word
        int hashed = hashing_function(word);

        //checks if the node at the location of the hashed word is pointing to NULL.
        //If it is, then we can safely have the head of the hashtable point directly to the new node
        if (hashtable[hashed] == NULL)
        {
            hashtable[hashed] = new_node;
            new_node -> next = NULL;
        }

        //If it isn't, we would have to first, have the new node point to the next, and then have the head of the hashtable point to the new node in order to not lose the chain of links
        else
        {
            new_node -> next = hashtable[hashed];
            hashtable[hashed] = new_node;
        }
        word_count++;
    }
    //close dictionary and return true
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
     // for nodes in the hashtable
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        // check the table for a node at that index
        node* cursor = hashtable[i];
        if (cursor)
        {
            // free the linked list
            node* temp = cursor -> next;
            free (cursor);
            cursor = temp;
        }
    }
    return true;
}

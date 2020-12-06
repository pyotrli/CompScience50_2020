// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536; //65536; // max for unsigned int

// Hash table
node *table[N];

// Dictionary size
unsigned int dict_size = 0;

// Hash function prototype
unsigned int hash_word(const char *word);

// Returns true if word is in dictionary else false N4
bool check(const char *word)
{
    unsigned int bucket = hash_word(word);
    node *tmp = table[bucket];
    while (tmp != NULL)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
        else
        {
            tmp = tmp->next;
        }
    }
    return false;
}

// Hashes word to a number - N2
// djb2 hash function prototype from http://www.cse.yorku.ca/~oz/hash.html
// below code from Neel Mehta https://github.com/hathix/cs50-section/blob/master/code/7/sample-hash-functions/good-hash-function.c
unsigned int hash_word(const char *word)
{
    unsigned long hash = 5381;
    
    for (const char *ptr = word; *ptr != '\0'; ptr++)
    {
        hash = ((hash << 5) + hash) + tolower(*ptr);
    }
    
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false N1
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    
    // If file unreadable, stop.
    if (file == NULL)
    {
        printf("Cannot open dictionary\n");
        return -1;
    }
    
    // Read strings one by one
    char *word = malloc(sizeof(char) * LENGTH + 1); // TODO Free this
    if (word == NULL)
    {
        printf("Memory allocation problem\n");
        return -1;
    }
    
    while (fscanf(file, "%s", word) != EOF)
    {
        // Update global var
        dict_size++;
        // Create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Memory allocation problem\n");
            return -1;
        }
        
        // Copy dict word into node
        strcpy(n->word, word);
        
        // Hash word
        unsigned int bucket = hash_word(word);

        // Insert node into hash table - mind the pointers
        n->next = table[bucket];
        table[bucket] = n;
    }

    // Cycle through buckets and print contents
//    for (int i = 0; i < N; i++)
//    {
//        printf("Bucket N%i -->", i);
//        for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
//        {
//            printf("%s -->", tmp->word);
//        }
//        printf("\n");
//    }
    
    free(word);
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded N3
unsigned int size(void)
{
    return dict_size != 0 ? dict_size : 0;
}

// Destroys all nodes and frees memory recursively
void destroy(node *head)
{
    node *tmp = head;
    if (tmp == NULL)
    {
        return;
    }
    else
    {
        destroy(tmp->next);
    }
    free(tmp);
}

// Unloads dictionary from memory, returning true if successful else false N5
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        destroy(table[i]);
    }
    return true;
}


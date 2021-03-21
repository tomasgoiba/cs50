// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
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
const unsigned int N = 9941;

// Constant for polynomial hash function
const unsigned int G = 31;

// Initialize variables
bool dload;  // Record if dictionary was loaded successfully
int dwords;  // Number of words in dictionary

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Traverse the linked list to check if 'word' is already in the table
    for (node *tmp = table[hash(word)]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }
    return false;

}

/**
 * HASH
 *
 * This function computes a hash code for the given string.
 *
 * In order to minimize collisions, a polynomial hash code is implemented along with compression by division.
 *
 * To calculate the hash code, we multiply the ASCII value for each char in the string
 * by the power of a nonzero constant G and add up the result, ignoring overflow. We then compress
 * the total using modulus division over N to ensure the code fits into the table.
 *
 * For best results, N and G are coprime, which greatly reduces the number of common factors and thus
 * minimizes collisions, giving a better distribution of hashes
 */
unsigned int hash(const char *word)
{
    // Implement polynomial hash code using Horner's Method
    unsigned int h = 0;
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        h = G * h + tolower(word[i]);
    }

    // Hash compression by modulus division
    return h % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return dload = false;
    }

    // Initialize variables
    char dword[LENGTH + 1];
    dwords = 0;

    // Loop over the words in the file
    while (fscanf(inptr, "%s", dword) != EOF)
    {
        // Create node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Could not allocate memory.\n");
            return dload = false;
        }

        // Copy the word into the node
        strcpy(n->word, dword);

        // Find hash for the word
        int index = hash(n->word);

        // Insert node at the beginning of the list
        n->next = table[index];
        table[index] = n;

        // Update word count
        dwords++;
    }

    // Close file and return true
    fclose(inptr);
    return dload = true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (!dload)
    {
        return 0;
    }
    else
    {
        return dwords;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Free allocated memory one node at a time using helper node 'tmp'
    for (int i = 0; i < N; i++)
    {
        for (node *tmp = table[i]; tmp != NULL; tmp = table[i])
        {
            table[i] = table[i]->next;
            free(tmp);
        }
    }
    return true;
}
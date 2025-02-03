// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

int count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
// 2 ^ 18
const unsigned int N = 524288;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    node *p;
    char *lower_word = malloc(sizeof(char) * (strlen(word) + 1));
    for (int i = 0; i < strlen(word); i++)
    {
        lower_word[i] = tolower(word[i]);
    }
    lower_word[strlen(word)] = '\0';

    p = table[hash(lower_word)];
    if (p == NULL)
    {
        free(lower_word);
        return false;
    }

    while (true)
    {
        if (strcmp(p->word, lower_word) == 0)
        {
            free(lower_word);
            return true;
        }

        p = p->next;

        if (p == NULL)
        {
            break;
        }
    }

    free(lower_word);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    int sum = 0;
    int word_length = strlen(word);

    for (int i = 0; i < word_length; i++)
    {
        sum += tolower(word[i]) * (i + 1);
    }

    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    FILE *d;
    d = fopen(dictionary, "r");
    if (d == NULL)
    {
        return false;
    }

    char *word = malloc(sizeof(char) * LENGTH + 1);
    if (word == NULL)
    {
        fclose(d);
        return false;
    }

    node *n;
    int n_hash;
    int fscan;

    while (true)
    {
        fscan = fscanf(d, "%s", word);
        if (feof(d))
        {
            break;
        }

        if (fscan != 1)
        {
            free(word);
            fclose(d);
            return false;
        }

        n = malloc(sizeof(node));
        if (n == NULL)
        {
            free(word);
            fclose(d);
            return false;
        }

        n_hash = hash(word);
        n->next = table[n_hash];
        strcpy(n->word, word);
        table[n_hash] = n;

        count++;
    }

    free(word);
    fclose(d);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    node *p;
    node *temp;

    for (int i = 0; i < N; i++)
    {
        p = table[i];
        if (p == NULL)
        {
            continue;
        }

        while (true)
        {
            temp = p;
            p = p->next;
            free(temp);
            temp = NULL;

            if (p == NULL)
            {
                break;
            }
        }
    }

    return true;
}

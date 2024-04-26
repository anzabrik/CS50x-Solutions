// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 * 26;
#define ALPHABET 26

// Hash table
node *table[N];
node *n = NULL;
unsigned int word_count = 0;

// 4 Returns true if word is in dictionary, else false -- takes a word and "IS this word in the dictionary"
bool check(const char *word)
{
    int hash_index = hash(word);
    node *cursor = table[hash_index]; //now cursor points at the head of the linked list at the given index in the table
    while (cursor != NULL)
    {
        if (!strcasecmp(cursor->word, word))
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// 2 Hashes word to a number -- takes a word and runs a function on it returning a number
unsigned int hash(const char *word)
{
    // TODO:
    int hash_counter = 0;
    int word_hash_index = 0;
    int alph[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

    int first_letter_ind = tolower(word[0]) - 'a';
    int second_letter_ind = (tolower(word[1]) - 'a'); // % 26;

    for (int i = 0; i < ALPHABET; i++)
    {
        if (first_letter_ind == alph[i])// The counter will be incremented by 26
        {
            for (int j = 0; j < ALPHABET; j++)
            {
                if (second_letter_ind == alph[j])
                {
                    word_hash_index = hash_counter;
                    return word_hash_index;
                }
                hash_counter += 1;//after each letter in j, increment the counter. By the end of aX, the counter is 25
            }
        }
        else
        {
            hash_counter += 26;//after each letter in i, increment the counter
        }
    }
    return word_hash_index;
}


bool load(const char *dictionary)// Takes a dictionary and loads it into a hashtable
{
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open dict file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Couldn't open dictionary/n");
        fclose(dict); //???1st dif
        return false;
    }

    // Read strings from the file one at a time
    char w_d[LENGTH];
    while (fscanf(dict, "%s", w_d) != EOF) //Maybe the mistake was because i didn't allocate space for that word
    {
        // Create a new node for each word
        n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, w_d);
        n->next = NULL;

        // Hash word to obtain hash value 4TH DIFFERENCE
        int words_hash = hash(n->word);

        // Insert node into hash table at that location
        for (int i = 0; i < N; i++)
        {
            if (words_hash == i)
            {
                if (table[i] != NULL)
                {
                    n->next = table[i];
                    table[i] = n;
                }
                else
                {
                    table[i] = n;
                }
                word_count++; //???for SIZE function
                break;
            }
        }
    }
    fclose(dict);
    return true;
}

// 3 Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)// KEEP TRACK OF WODS AS YOU load them in your dictionary
{
    // TODO
    return word_count;
}

// 5 Unloads dictionary from memory, returning true if successful, else false -
// set cursor to the previusbackwards, freeing the nodes one by one
bool unload(void)
{
    // TODO
    node *curs = NULL;
    node *temp = NULL;
    for (int i = 0; i < N; i++) // Go into each bucket
    {
        curs = table[i]; // Curs points at the first node
        while (curs != NULL) // While there're any nodes in this bucket
        {
            temp = curs; // Temp points at the first node
            curs = curs->next; // Curs points at the second node (so that we don't lose it)
            free(temp);
        }
    }
    return true;
}

/*
    maybe later upgrade the hash function - for instance, by adding the sum of ascii values somehow
    unsigned int hash_ind = 0;
    int word_length = strlen(word);
    for (int i = 0; i < word_length; i++)
    {
        hash_ind += tolower(word[i]) - 'a';
    }
    return hash_ind;
        //printf("%i\n", word_hash_index);
    //printf("%i\n", hash_counter);
    //printf("%i", first_letter_ind);
    */
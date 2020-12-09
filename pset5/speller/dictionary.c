
// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000;
int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
  char word_copy[LENGTH + 1];

  for (int i = 0; i < strlen (word); i++) word_copy[i] = tolower (word[i]);
  word_copy[strlen (word)] = '\0';


  for (node *cursor = table[hash (word_copy)]; cursor != NULL; )  {
    if (strcasecmp(cursor->word, word_copy) == 0) return true;
    else cursor = cursor->next;
  }

  return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
  int i, hash = 0;
  for (i = 0; i < strlen (word); i++) hash = (hash << 2) ^ word[i];

  return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
  int i;
  node *new_node, *head;
  FILE *file;

  file = fopen (dictionary, "r");
  if (file == NULL) return false;

  for (char word[LENGTH + 1]; fscanf (file, "%s", word) != EOF; ) {
    new_node = malloc (sizeof (node));
    if (new_node == NULL) {
      unload();
      return false;
    }

    strcpy (new_node->word, word);

    i = hash (new_node->word);
    head = table[i];

    if (head == NULL) table[i] = new_node;
    else {
      new_node->next = table[i];
      table[i] = new_node;
    }

    word_count++;

  }

  fclose (file);
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
  for (int i = 0; i < N ; i++) free (table[i]);

  return true;
}


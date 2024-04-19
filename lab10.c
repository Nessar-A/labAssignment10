#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
};

// Initializes a trie node
struct TrieNode *createNode()
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node)
    {
        node->isEndOfWord = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts the word to the trie structure
void insert(struct TrieNode *root, char *word)
{
    struct TrieNode *current = root;
    while (*word)
    {
        int index = *word - 'a';
        if (!current->children[index])
        {
            current->children[index] = createNode();
        }
        current = current->children[index];
        word++;
    }
    // Mark the end of the word
    current->isEndOfWord++;
}

// Searches for the word in the trie and returns the number of occurrences
int numberOfOccurrences(struct TrieNode *root, char *word)
{
    struct TrieNode *current = root;
    while (*word)
    {
        int index = *word - 'a';
        if (!current->children[index])
        {
            return 0; // Word does not exist
        }
        current = current->children[index];
        word++;
    }
    return current->isEndOfWord; // Return the count of occurrences
}

// Deallocates the trie structure
void deallocateTrie(struct TrieNode *root)
{
    if (!root)
        return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i])
        {
            deallocateTrie(root->children[i]);
        }
    }
    free(root);
}

// Reads the dictionary from a file
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    //Starts because it will increase on first line even though first line just tells us how many lines there truly is
    int count = -1;
    char word[100];

    while (fscanf(file, "%s", word) != EOF)
    {
        pInWords[count] = strdup(word);
        count++;
    }

    fclose(file);
    return count;
}

int main(void)
{
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary-1.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode *root = createNode();
    for (int i = 1; i < numWords; i++)
    {
        insert(root, inWords[i]);
    }

    // Test with sample words
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    deallocateTrie(root);

    return 0;
}

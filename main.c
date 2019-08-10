#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
//Fix me: Do the necessary change to make the implementation //case-insensitive
   while (1)
    {
        char c = fgetc(file);
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            // .............
            if(c >= 'A' && c <= 'Z'){
                c += 32;
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF || !isalpha(c))
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    const char* fileName = "input1.txt";
    if (argc > 1)
    {
        fileName = argv[1];
    }
    printf("Opening file: %s\n", fileName);

    FILE* file = fopen(fileName, "r");

    char* word;

    clock_t timer = clock();

    struct HashMap* map = hashMapNew(10);
    struct HashLink* ptr;

    // --- Concordance code begins here ---
    word = nextWord(file);
    while(word != NULL){
        int idx = abs(HASH_FUNCTION(word)) % map->capacity;
        ptr = map->table[idx];
        printf("Adding word: %s\n", word);
        printf("Initial idx: %d\n", idx);
        printf("Initial pointer: %p\n", ptr);
            while(ptr != NULL){
                if(strcmp(word, ptr->key) == 0){
                    ptr->value++;
                }
                else{
                    ptr = ptr->next;
                }
            }
            if(ptr == NULL){
                hashMapPut(map, word, 1);
            }
        word = nextWord(file);
    }
    free(word);
    hashMapPrint(map);
    // Be sure to free the word after you are done with it here.
    // --- Concordance code ends here ---

    fclose(file);

    timer = clock() - timer;
    printf("\nRan in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    printf("Empty buckets: %d\n", hashMapEmptyBuckets(map));
    printf("Number of links: %d\n", hashMapSize(map));
    printf("Number of buckets: %d\n", hashMapCapacity(map));
    printf("Table load: %f\n", hashMapTableLoad(map));

    hashMapDelete(map);
    return 0;
}

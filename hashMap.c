/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Cooper Smith, Anthony Minniti, Gabe Schafman
 * Date: 05/08/19
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
 HashLink* hashLinkNew(const char* key, int value, HashLink* next)
 {
     HashLink* link = malloc(sizeof(HashLink));
     link->key = malloc(sizeof(char) * (strlen(key) + 1));
     strcpy(link->key, key);
     link->value = value;
     link->next = next;
     return link;
 }

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(struct HashLink* link)
{
    free(link->key); //Free dynamic allocation key variable
    free(link); //Free dynamic allocation link variable
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
 void hashMapInit(HashMap* map, int capacity)
 {
     map->capacity = capacity;
     map->size = 0;
     map->table = malloc(sizeof(HashLink*) * capacity);
     for (int i = 0; i < capacity; i++)
     {
         map->table[i] = NULL;
     }
 }

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(struct HashMap* map)
{
    struct HashLink* cur;
    struct HashLink* temp; //Create link pointers current and temporary.
    for(int i = 0; i < map->capacity; i++){ //For all map slots 0 -> capacity
        cur = map->table[i]; //Skip the sentinel and go into bucket.
        while(cur != NULL){  //While the current link is not NULL>
            temp = cur->next;  //See where the next link is.
            hashLinkDelete(cur); //Free the memory from this link.
            cur = temp;  //Assign the next link as current for iteration.
        }
    }
    // FIXME: implement  CHECKED BY: TONY
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
 HashMap* hashMapNew(int capacity)
 {
     HashMap* map = malloc(sizeof(HashMap));
     hashMapInit(map, capacity);
     return map;
 }

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(struct HashMap* map)
{
    hashMapCleanUp(map); //Clean up all the map, except the pointers directly in the table.
    free(map);  //Free the memory in the
}

/**
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(struct HashMap* map, const char* key)
{
    int* temp;  //Create a temporary integer variable.
    struct HashLink* ptr;  //Create a HashLink pointer.

    for(int i = 0; i < map->capacity; i++){                                     //iterates through every bucket
        ptr = map->table[i];                                              //starts iterating at the first link after the bucket sentinel.
        while(ptr != 1 && ptr != NULL){                                                     //if not at the end of each bucket linked list
            if(strcmp(key, ptr->key) != 0){                      //converts both keys to ints and compares the values, if they're not equal moves pointer to next link
                ptr = ptr->next;
            }
            else if(strcmp(key, ptr->key) == 0){                                                              //if the key values are equal, copies link value to temp pointer  //THIS COULD LEAD TO AN ERROR. The else if () is a safer bet.
                temp = &(ptr->value);                                               //Assign the temp int as the value found.
                return temp;                                                        //returns temp pointer to value in desired link
            }
        }
    }
    // FIXME: implement
    return NULL;                //if key doesn't match any in table, returns NULL
}

/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity (double of the old capacity). After allocating the new table,
 * all of the links need to rehashed into it because the capacity has changed.
 *
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 *
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
    int i;
    struct HashLink* ptr = NULL;
    HashMap* newMap = hashMapNew(capacity);
    printf("\nMap resize:\nOld Address: %p\nNew: %p\n\n", map, newMap);
    for(i = 0; i < map->capacity; i++){
        ptr = map->table[i];
        while(ptr != NULL){
                assert(sizeof(ptr) == sizeof(struct HashLink*));
                printf("The pointers being added during resize %p |\n", ptr);
                printf("The next pointer: %p |\n", ptr->next);
                hashMapPut(newMap, ptr->key, ptr->value);
                ptr = ptr->next;

        }
    }
    struct HashMap* temp = map;
    map = newMap;
    hashMapDelete(temp);
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 *
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)    //THIS FUNCTION MAY NEED A REDISIGN.
{
    printf("The current put is %s\n", key);
    struct HashLink* link;
    int contained = hashMapContainsKey(map, key);
    if(contained == 1){
        link = hashMapGet(map, key);
        link->value++;
    }
    else if(contained == 0){
        int idx = HASH_FUNCTION(key) % map->capacity;
        printf("The idx during put %d\n", idx);
        if(map->table[idx] != NULL){
            if(idx < 0){
                idx += map->capacity;
            }
            link = hashLinkNew(key, value, map->table[idx]);
            map->table[idx] = link;

            printf("added address %p in front of %p with the thrid being %p\n", link, link->next, link->next->next);
        }
        else{
            map->table[idx] = hashLinkNew(key, value, NULL);
            printf("New table entry address %p\n", map->table[idx]);
        }
    }
    float temp = hashMapTableLoad(map);                                         //Set that float equal to the table load.
    if(temp >= MAX_TABLE_LOAD){
        printf("Map being resized address %p\n", map);                                               //If table load is too high, like the rent,...
        resizeTable(map, (2 * map->capacity));                                        //Double the table size.
    }
    map->size++;
    // FIXME: implement


}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(struct HashMap* map, const char* key)
{
    struct HashLink* cur;
    struct HashLink* temp;
    struct HashLink* prev;                                                      //Create current, temporary, and previous HashLink pointers.
    int idx = abs((HASH_FUNCTION(key)) % (map->capacity));                      //Determine the starting index of the search.
    cur = map->table[idx];                                                      //Jump into bucket where the key points.
    prev = map->table[idx];                                                     //Track your previous pointer.
    while(cur != 1 && cur != NULL){                                                         //While the bucket is not empty. Because if it's empty, we do nothing.
        if(strcmp(key, cur->key) == 0){                          //If the value is found.
            temp = cur;                                                                 //Assign the temp to the current value.
            prev->next = cur->next;                                                     //Assign the previous next, to the current next.
            hashLinkDelete(temp);                                                       //Delete the container that had the value.
            map->size--;
        }
        else{
            prev = cur;                                                         //Assign the previous pointer to the current spot.
            cur = prev->next;                                                    //Iterate to the next container.
        }
    }
    // FIXME: implement
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(struct HashMap* map, const char* key)
{
    struct HashLink* ptr;                                                              //Create pointer for link iteration.
    int idx = abs((HASH_FUNCTION(key)) % (map->capacity));                      //Determine the starting index of the search.
    ptr = map->table[idx];                                                      //Start looking for the value in the bucket it would be in.
    while(ptr != 1 && ptr != NULL){                                                         //While not at the end of each bucket linked list
        if(strcmp(key, ptr->key) != 0){                      //Converts both keys to ints and compares the values, if they're not equal moves pointer to next link
            ptr = ptr->next;
        }
        else {                                                                  //If the key values are equal
            return 1;
        }
    }
    // FIXME: implement
    return 0;                                                                   //Link is not in the table
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(struct HashMap* map)
{
    // FIXME: implement
    return (map->size);                                                         //Return map size variable.
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(struct HashMap* map)
{
    // FIXME: implement
    return (map->capacity);                                                     //Returns max number of buckets in the table.
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(struct HashMap* map)
{
    int i, j;
    j = 0;
    for(i = 0; i < map->capacity; i++){
        if(map->table[i] == NULL)
            j++;
    }
    // FIXME: implement
    return j;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(struct HashMap* map)
{
    float load = ((float)map->size) / ((float)map->capacity);
    // FIXME: implement
    return load;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(struct HashMap* map)
{
    struct HashLink* temp;                                                             //Create a temporary pointer for links
    for(int i = 0; i < map->capacity; i++){                                     //Start iterating through buckets
        temp = map->table[i];
        while(temp != NULL && temp != 1){
            printf("%d \n", temp->value);
            temp = temp->next;
        }
        printf("\n");
    }
  // FIXME: implement
}

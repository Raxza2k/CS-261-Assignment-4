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
    HashLink* link = malloc(sizeof(HashLink));   // Dynamic memory allocation for new link.
    link->key = malloc(sizeof(char) * (strlen(key) + 1)); //Dynamic memory allocation for the key.
    strcpy(link->key, key); //Copy the key into the dynamic slot.
    link->value = value; // Add the value to the link.
    link->next = next; // Put the next link provided pointer into new link.
    return link; //Return the link to call function.
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
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
    map->capacity = capacity;  //Set this (provided) hashmap to desired capacity.
    map->size = 0;  //Set the size to 0.
    map->table = malloc(sizeof(HashLink*) * capacity); //Dynamically allocate space for sentinels or links IAW capacity.
    for (int i = 0; i < capacity; i++)  //Set all table pointers to NULL.
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    HashLink* cur, temp; //Create link pointers current and temporary.
    for(int i = 0; i < map->capacity; i++){ //For all map slots 0 -> capacity
        cur = map->table[i]->next; //Skip the sentinel and go into bucket.
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
    HashMap* map = malloc(sizeof(HashMap)); //Allocate memory for the hashMap.
    hashMapInit(map, capacity); //Initialize hashMap, which also builds it. NO SENTINELS!
    return map;  //Return map to call function.
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
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
int* hashMapGet(HashMap* map, const char* key)
{
    int* temp;
    HashLink* ptr;

    for(int i = 0; i < map->capacity; i++){     //iterates through every bucket
        ptr = map->table[i]->next;              //starts iterating at the first link after the bucket
        while(ptr != NULL){                     //if not at the end of each bucket linked list
            if(strcmp(HASH_FUNCTION(key), HASH_FUNCTION(ptr->key)) != 0){           //converts both keys to ints and compares the values, if they're not equal moves pointer to next link
                ptr = ptr->next;
            }
            else {                                  //if the key values are equal, copies link value to temp pointer
                temp = &(ptr->value);
                return temp;                        //returns temp pointer to value in desired link
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
    HashMap* newMap = hashMapNew(capacity);     //create new map with new capacity

    HashLink* ptr;

    for(int i = 0; i < map->capacity; i++){             //iterates through old map buckets
        ptr = map->table[i]->next;
        while(ptr != NULL){                             //iterates through the linked list in each bucket
            hashMapPut(newMap, ptr->key, ptr->value);   //puts every link node in the new map
        }
    }
    hashMapDelete(map);    //delete old map
    // FIXME: implement
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
void hashMapPut(HashMap* map, const char* key, int value)
{
    HashLink* ptr;
    int idx = abs((HASH_FUNCTION(key)) % (map->capacity));          //determines index for new value to be placed

    for(int i = 0; i < map->capacity; i++){     //iterates through every bucket
        if(i == idx && map->table[i]->next != NULL){
            ptr = map->table[i]->next;              //starts iterating at the first link after the bucket
            while(ptr != NULL){                     //if not at the end of each bucket linked list
                if(strcmp(HASH_FUNCTION(key), HASH_FUNCTION(ptr->key)) != 0){           //converts both keys to ints and compares the values, if they're not equal moves pointer to next link
                    ptr = ptr->next;
                }
                else if(strcmp(HASH_FUNCTION(key), HASH_FUNCTION(ptr->key)) == 0){                                  //if the key values are equal, copies value to link value variable
                    ptr->value = value;
                }
            }
            if(ptr == NULL){
                ptr = map->table[i]->next;
                hashLinkNew(key, value, ptr->next);
            }
        }
    }
    //Resize Table if needed.
    float temp;
    temp = hashMapTableLoad(map);
    if(temp >= MAX_TABLE_LOAD){
        resizeTable(map, 2 * map->capacity);
    }
    // FIXME: implement
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
    HashLink* cur, tmp;
    for(int i = 0; i < map-> capacity; i ++){
        cur = map->table[i]->next;
        if(HASH_FUNCTION(cur->key) == HASH_FUNCTION(key)){
            map->table[i]->next = cur->next;
            hashLinkDelete(cur);
        }
        while(cur != NULL){
            if(HASH_FUNCTION(cur->next->key) == HASH_FUNCTION(key)){
                temp = cur->next->next;
                hashLinkDelete(cur->next);
                cur->next = temp;
            }
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
int hashMapContainsKey(HashMap* map, const char* key)
{
    HashLink* ptr;

    for(int i = 0; i < map->capacity; i++){     //iterates through every bucket
        ptr = map->table[i]->next;              //starts iterating at the first link after the bucket
        while(ptr != NULL){                     //if not at the end of each bucket linked list
            if(strcmp(HASH_FUNCTION(key), HASH_FUNCTION(ptr->key)) != 0){           //converts both keys to ints and compares the values, if they're not equal moves pointer to next link
                ptr = ptr->next;
            }
            else {                                  //if the key values are equal
                return 1;
            }
        }
    }
    // FIXME: implement
    return 0;           //link is not in the table
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    // FIXME: implement
    return (map->size);
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    // FIXME: implement
    return (map->capacity);
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
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
float hashMapTableLoad(HashMap* map)
{
    float load = ((float)map->size) / ((float)map->capacity);
    // FIXME: implement
    return load;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
    HashLink* temp;
    for(int i = 0; i < map->capacity; i++){
        temp = map->table[i]->next;
        while(temp != NULL){
            printf("%d \t \n", temp->value);
            temp = temp->next;
        }
    }
  // FIXME: implement


}

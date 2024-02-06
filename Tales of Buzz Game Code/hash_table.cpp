// ============================================
// The HashTable class file
// Author: Huy Le   GTID: 903845849
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

/****************************************************************************
 * Include the Public Interface
 *
 * By including the public interface at the top of the file, the compiler can
 * enforce that the function declarations in the the header are not in
 * conflict with the definitions in the file. This is not a guarantee of
 * correctness, but it is better than nothing!
 ***************************************************************************/
#include "hash_table.h"

/****************************************************************************
 * Include other private dependencies
 *
 * These other modules are used in the implementation of the hash table module,
 * but are not required by users of the hash table.
 ***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
 * Hidden Definitions
 *
 * These definitions are not available outside of this file. However, because
 * the are forward declared in hash_table.h, the type names are
 * available everywhere and user code can hold pointers to these structs.
 ***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
    /** The array of pointers to the head of a singly linked list, whose nodes
        are HashTableEntry objects */
    HashTableEntry **buckets;

    /** The hash function pointer */
    HashFunction hash;

    /** The number of buckets in the hash table */
    unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry
{
    /** The key for the hash table entry */
    unsigned int key;

    /** The value associated with this hash table entry */
    void *value;

    /**
     * A pointer pointing to the next hash table entry
     * NULL means there is no next entry (i.e. this is the tail)
     */
    HashTableEntry *next;
};

/****************************************************************************
 * Private Functions
 *
 * These functions are not available outside of this file, since they are not
 * declared in hash_table.h.
 ***************************************************************************/
/**
 * createHashTableEntry
 *
 * Helper function that creates a hash table entry by allocating memory for it on
 * the heap. It initializes the entry with key and value, initialize pointer to
 * the next entry as NULL, and return the pointer to this hash table entry.
 *
 * @param key The key corresponds to the hash table entry
 * @param value The value stored in the hash table entry
 * @return The pointer to the hash table entry
 */
static HashTableEntry *createHashTableEntry(unsigned int key, void *value)
{
    // TODO: Implement
    // 1. Create an initialize a new hash table entry given an input key and value
    //     Hint: how do we allocate memory in the heap?
    //     Note: Make sure to initialize the next pointer to null
    //make new node
    HashTableEntry* newNode = (HashTableEntry*) malloc(sizeof(HashTableEntry));
    //give it key, value, & next pointer to null
    newNode -> key = key;
    newNode -> value = value;
    newNode -> next = NULL; 
    // 2. Return the new hash table entry
    return  newNode;
}

/**
 * findItem
 *
 * Helper function that checks whether there exists the hash table entry that
 * contains a specific key.
 *
 * @param hashTable The pointer to the hash table.
 * @param key The key corresponds to the hash table entry
 * @return The pointer to the hash table entry, or NULL if key does not exist
 */
static HashTableEntry *findItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    
    // 3. If there is nothing in the bucket, return NULL
    if (hashTable == NULL) {
        return NULL;
    }
    // 1. Get the bucket number
    unsigned int index = hashTable->hash(key); //get bucket num
    // 2. Get the head entry
    HashTableEntry* currNode = (HashTableEntry*) hashTable -> buckets[index]; //currNode is head entry
    // 4. Loop through the hash table to find if the key matches
    //      4a. While you are not at end node of the hash table 
    //      4b. If the key is found, return the hash table entry
    //      4c. Otherwise, move to the next node
    while (currNode) { //if current node is not null
        if (currNode -> key == key) { //and matches key
            return currNode;  //return current node
        }
        currNode = currNode -> next; //else move to next node
    }
    // 5. Return NULL if the key is not present
    return NULL;   //return null if no key
}

/****************************************************************************
 * Public Interface Functions
 *
 * These functions implement the public interface as specified in the header
 * file, and make use of the private functions and hidden definitions in the
 * above sections.
 ****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
    // The hash table has to contain at least one bucket. Exit gracefully if
    // this condition is not met.
    if (numBuckets == 0)
    {
        printf("Hash table has to contain at least 1 bucket...\n");
        exit(1);
    }

    // Allocate memory for the new HashTable struct on heap.
    HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

    // Initialize the components of the new HashTable struct.
    newTable->hash = hashFunction;
    newTable->num_buckets = numBuckets;
    newTable->buckets = (HashTableEntry **)malloc(numBuckets * sizeof(HashTableEntry *));

    // As the new buckets are empty, init each bucket as NULL.
    unsigned int i;
    for (i = 0; i < numBuckets; ++i)
    {
        newTable->buckets[i] = NULL;
    }

    // Return the new HashTable struct.
    return newTable;
}

void destroyHashTable(HashTable *hashTable)
{
    // TODO: Implement
    // 1. Loop through each bucket of the hash table to remove all items.
    //      1a. set temp to be the first entry of the ith bucket
    //      1b. delete all entries
    if (hashTable == NULL) {
        return;
    }
    for (int i = 0; i < hashTable -> num_buckets; i++) { //Go thru i =0 -> HT.num_buckets
        HashTableEntry* currNode = hashTable->buckets[i]; //curr node is 1st  entry of ith bucket
        HashTableEntry* nextNode; //currNode.next
        while(currNode) { //current node is not null
            nextNode = currNode->next; //pointer to currNode.next
            //destroy curr node + all value
            free(currNode->value);  //free currnode.value
            free(currNode);         //free currnode
            //go to next entry
            currNode = nextNode;    //set currNode is nextNode
        }
        hashTable->buckets[i] = NULL; // 2. Free buckets
    }   
    //Free address containing HT.buckets once all buckets are empty
    free(hashTable->buckets);
    // 3. Free hash table
    free(hashTable);
}

void *insertItem(HashTable *hashTable, unsigned int key, void *value)
{
    if (hashTable == NULL) {
        return NULL;
    }
    // TODO: Implement
    //1. First, we want to check if the key is present in the hash table.
    int index = hashTable -> hash(key);    //make bucket num
    HashTableEntry* currEntry = findItem(hashTable,key);   //currEntry is the head
    //2. If the key is present in the hash table, store new value and return old value
    if (currEntry) { //entry not null -> key in HT
        void* old = currEntry->value; //value of existing key is updated with new value
        currEntry->value = value; //update value of currEntry
        return old; //return old value
        //3. If not, create entry for new value and return NULL
    } else { //entry is null -> key not in HT
        // unsigned int index = hashTable->hash(key);
        HashTableEntry* newEntry = createHashTableEntry(key,value); //make new entry with key,value + pointer points to new entry
        newEntry->next = hashTable->buckets[index]; //next pointer of new entry -> current 1st entry (if entry existed at this index -> new entry is linked to existing entry)
        hashTable->buckets[index] = newEntry; //'buckets' has pointers to 1st entry of each LL. newEntry is now first entry in LL -> newEntry is new head
        return NULL; //insertion is successful
    }
}

void *getItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    
    // **NOTE: DIfferences between Find and Get**
    // This function simply calls another function to FIND the item
    // based on the key, and check if the key exist and then return the item's value
    // to GET the value that corresponds to the key in the hash table.
    HashTableEntry* item = findItem(hashTable, key); //call method findItem to find item based on key
 
    //1. First, we want to check if the key is present in the hash table.
    if (hashTable == NULL) {
        return NULL;
    }
    //2. If the key exist, return the value
    if (item) {    //if item is not null
        return item->value;  //return item.value
    } else {       //if no item
        return NULL;   //return null
    }
    //3. If not. just return NULL
}


void *removeItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    //Remove the item in hash table based on the key and return the old value stored in it.
    //In other words, free the hash table entry from heap and return its old value
    if (hashTable == NULL) {
        return NULL;
    }
     //1. Get the bucket number and the head entry
    int index = hashTable -> hash(key); //get index of bucket
    HashTableEntry* currNode = hashTable -> buckets[index]; //pointer to 1st entry of bucket (get head of bucket)
    HashTableEntry* prev = NULL; //initialize pointer to Null (keep track of previous entry in LL)
    while (currNode) {  //currNode not null
        //2. If the head holds the key, change the head to the next value, and return the old value
        if (currNode -> key == key) { //curr key == key
            void *old = currNode -> value; //store value associated with entry in 'old'
            //3. If not the head, search for the key to be removed 
            if (prev) {  //prev is not null (current entry is not 1st in list)
                prev->next = currNode->next; //updates pointers in LL to remove current entry
            } else { //prev is null (current entry is head)
                hashTable -> buckets[index] = currNode->next; //bucket[index] pointer moves to next entry
            }
            //5. Unlink node from the list, free, and return old value
            free(currNode); //deallocate memory of current entry (no memory leaks)
            return old; //return stored value of removed entry
        }
        prev = currNode; //update prev pointer to current entry b4 next iteration
        currNode = currNode -> next; //moves entry pointer to next entry
    }
    //4. If the key is not present in the list, return NULL
    return NULL; //if no key found -> return NULL
    
}

void deleteItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    //Delete the item in the hash table based on the key. 
    
    // **NOTE: DIfferences between Remove and Delete**
    // This function simply calls another function that REMOVES the item
    // based on the key, and then free its return value to DELETE it from the hash table
    // You're basically clearing the memory
 
    //1. Remove the entry and free the returned data
    free(removeItem(hashTable, key));   //call removeItem method and free its return value
}
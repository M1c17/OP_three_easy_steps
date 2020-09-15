#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include "mapreduce.h"

#define Pthread_create(thread, attr, start_routine, arg) assert(pthread_create(thread, attr, start_routine, arg) == 0);
#define Pthread_join(thread, value_ptr) assert(pthread_join(thread, value_ptr) == 0);
#define Pthread_mutex_lock(mutex) assert(pthread_mutex_lock(mutex) == 0);
#define Pthread_mutex_unlock(mutex) assert(pthread_mutex_unlock(mutex) == 0);

//// https://www.quora.com/How-is-the-size-of-a-hash-table-determined-How-should-optimization-be-done-for-it-to-be-fast
#define HASH_SIZE 2000

/* Build The MapReduce infrastructure supports the execution of user-defined Map() and Reduce() functions.
    As from the original paper: "Map(), written by the user, takes an input pair
    and produces a set of intermediate key/value pairs. The MapReduce library groups
    together all intermediate values associated with the same intermediate key K
    and passes them to the Reduce() function."
    create internal data structure to pass keys and values from mappers to reducers
*/

typedef struct key_valueNode {
    char *key;
    char **values;
    int num_valueArray;
    struct key_valueNode *next;
} key_valueNode;

// hashtable entries bucket
typedef struct hashTable_Bucket {
    key_valueNode **entries;
    unsigned long numKeys;
    pthread_mutex_t keyLock;
    pthread_mutex_t *locks;
} hashTable_Bucket;

//-----------------------------------------
// Hash preparation
// In each partition, there's a hash table that holds
// the key value pairs, eg: <"hello": 1,1,2,1>.
// The hash table implements its own hash function to decide which
// hash table bucket the key value pair goes to in the hashtable.
// However, since this hash table is not perfect hashing, collision might
// occurs. So the key value pairs are essentially a node in the linkedlist
// in the hashtable bucket.
//-----------------------------------------

//-----------------------------------------
// GLOBAL VARIABLES
//-----------------------------------------
// Variables for partition
struct hashTable_Bucket **partitionHashT;
int partition_number;
// Variables for mapper and reducer
Partitioner partitioner;
Mapper mapper;
Reducer reducer;
int numArray;
char** files;
pthread_mutex_t fileLock = PTHREAD_MUTEX_INITIALIZER;
int num_files;
int current_file;
int current_partition;
pthread_mutex_t *partitionLock;
struct key_valueNode **reduceList;


unsigned long hash_function(const char *key);
hashTable_Bucket *ht_create(void);
key_valueNode *ht_pair(const char *key, const char *value);
void ht_insert(hashTable_Bucket *hashTable, const char *key, const char *value);
void display_ht(hashTable_Bucket *ht);

char *get_next(char *key, int partition_number);
void* GetFileAndMap(void *files);
void MR_Emit(char *key, char *value);
int compare(const void *keyValue1, const void *keyValue2);
void *SortPartitionAndReduce();
unsigned long MR_DefaultHashPartition(char *key, int num_partitions);
void MR_Run(int argc, char *argv[], Mapper map, int num_mappers, Reducer reduce, int num_reducers, Partitioner partition);

//------------------------------------------------------
/* This function is from this source: http://www.cse.yorku.ca/~oz/hash.html
 * the hash function is called sdbm.
*/ 
//------------------------------------------------------
unsigned long hash_function(const char *key)
{
    unsigned long hash = 0;
    int c;

    while((c = *key++) != '\0') {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return (hash % HASH_SIZE);
}

// Create hastable bucket 
hashTable_Bucket *ht_create(void) {
    // allocate table
    hashTable_Bucket *hashTable = malloc(sizeof(hashTable_Bucket) * 1);
    // allocate table entries
    hashTable->entries = malloc(sizeof(key_valueNode *) * HASH_SIZE);
    hashTable->locks = malloc(sizeof(pthread_mutex_t) * HASH_SIZE);
    pthread_mutex_init(&hashTable->keyLock, NULL);
    hashTable->numKeys = 0;

    for(int i = 0; i < HASH_SIZE; ++i) {
        hashTable->entries[i] = NULL;
        if(pthread_mutex_init(&(hashTable->locks[i]), NULL) != 0) {
            printf("Error: Lock init failed\n");
        }
    }
    return hashTable;
}

key_valueNode *ht_pair(const char *key, const char *value)
{
    key_valueNode *key_value = malloc(sizeof(key_valueNode) * 1);
    key_value->key = strdup(key);
    //printf("this is key: %s\n", key_value->key);
    key_value->values = malloc(2000);
    key_value->values[0] = strdup(value);
    //printf("this is value: %s\n:", key_value->values[0]);
    key_value->num_valueArray = 1;
    //printf("this is numArray: %d\n", key_value->num_valueArray);
    key_value->next = NULL;

    return key_value;
}

void ht_insert(hashTable_Bucket *hashTable, const char *key, const char *value) 
{
    unsigned long slot = hash_function(key);
    //printf("slot is: %lu\n", slot);
   
    // look up an entry 
    key_valueNode *curr_keyValue = hashTable->entries[slot];

    if(curr_keyValue == NULL){
        Pthread_mutex_lock(&hashTable->keyLock);
        hashTable->numKeys++;
        //printf("in ht_insert %lu\n", hashTable->numKeys);
        Pthread_mutex_unlock(&hashTable->keyLock);
        hashTable->entries[slot] = ht_pair(key, value);
        return;
    }
    key_valueNode *prev = NULL;
    printf("in ht_insert %lu\n", hashTable->numKeys);
    // printf("this is numArray: %d\n", numArray);

    // walk through each node comparing key to insert correct value
    while(curr_keyValue != NULL) {
        if(strcmp(curr_keyValue->key, key) == 0){
            curr_keyValue->values[curr_keyValue->num_valueArray] = malloc(strlen(value) + 1);
            curr_keyValue->values[curr_keyValue->num_valueArray] = strdup(value);
            //printf("this is key: %s\n", curr_keyValue->key);
            //printf("this is value: %s\n", curr_keyValue->values[curr_keyValue->num_valueArray]);
            curr_keyValue->num_valueArray = curr_keyValue->num_valueArray + 1;
            //printf("this is num_valueArray %d\n", curr_keyValue->num_valueArray);
            return;
        }
        // walk to next node 
        prev = curr_keyValue;
        curr_keyValue = curr_keyValue->next;
    }
    printf("in ht_insert %lu\n", hashTable->numKeys);
    numArray = curr_keyValue->num_valueArray;
    // end of chain reached without a match
    prev->next = ht_pair(key, value);
}

void display_ht(hashTable_Bucket *ht) 
{
    for(int i = 0; i < HASH_SIZE; ++i) {
        key_valueNode *key_value = ht->entries[i];

        if(key_value == NULL){
            continue;
        }
        printf("slot #%d: ", i);

        for(;;) {
            printf("key: %s ", key_value->key);
            for(int j = 0; j < key_value->num_valueArray; ++j) {
                printf("value: %s ", key_value->values[j]);
            }
            printf("\n");
            
            if(key_value->next == NULL) {
                break;
            }

            key_value = key_value->next;
        }
        printf("\n");
    }
}

// Map() is called with a file name.
// a call to MR_Run() in the main() routine of the user program.
// This function is passed the argv array, and assumes that 
// argv[1] ... argv[n-1] (with argc equal to n) all contain file names that will be passed to the mappers.
void* GetFileAndMap(void *files){
    char **arguments = (char**) files;
    while(1) {
        Pthread_mutex_lock(&fileLock);
        int temp;
        if(current_file > num_files) {
            Pthread_mutex_unlock(&fileLock);
            return NULL;
        }
        if(current_file <= num_files) {
            temp = current_file;
            current_file++;
            //printf("In FILE\n");
        }
        Pthread_mutex_unlock(&fileLock);
        //printf("filename %s\n", arguments[temp]);
        mapper(arguments[temp]);
    }
    return NULL;
}

// function used for qsort to sort our list of keys in the hashtable
int compare(const void *keyValue1, const void *keyValue2)
{
    struct key_valueNode **k1 = (struct key_valueNode **)keyValue1;
    struct key_valueNode **k2 = (struct key_valueNode **)keyValue2;

    if(*k1 == NULL && *k2 == NULL) {
        return 0;
    } else if(*k1 == NULL) {
        return -1;
    } else if(*k2 == NULL) {
        return -1;
    } else {
        return strcmp((*k1)->key, (*k2)->key);
    }
}

// For each partition, keys (and the value list associated with said keys)
// should be sorted in ascending key order and call the reducer function on it 
// with our get_next function passed in
void *SortPartitionAndReduce()
{
    int curr_partition = partition_number;
    printf("here in sortreduce\n");
    if(partitionHashT[curr_partition] == NULL) {
        return NULL;
    }
    //printf("curr_partition %d\n", curr_partition);
    struct hashTable_Bucket *temp_table = partitionHashT[0];
    // create a list of nodes to keep all my keys
    struct key_valueNode *list[partitionHashT[curr_partition]->numKeys];
    printf("numkeys %lu\n", partitionHashT[0]->numKeys);
    unsigned long counter = 0;
    printf("hey hello\n");
    // walk through hashtable
    for(int j = 0; j < HASH_SIZE; ++j) {
        if(temp_table->entries[j] == NULL) {
            continue;
        }
        //printf("here is\n"); 
        struct key_valueNode *temp_key_valueNode = temp_table->entries[j];
        while(temp_key_valueNode) {
            printf("here %lu\n", counter);
            list[counter] = temp_key_valueNode;
            counter++;
            temp_key_valueNode = temp_key_valueNode->next;
        }
    }
    qsort(list, partitionHashT[current_partition]->numKeys, sizeof(struct key_valueNode *), compare);
    for(int x = 0; x < counter; ++x) {
        printf("this is keys list: %p\n", list[x]);
    }
    //iterate over my counter keys calling reducer for each one 
    for (int k = 0; k < counter; ++k) {
        reduceList[curr_partition] = list[k];
        reducer(list[k]->key, get_next, curr_partition);
    }
    --curr_partition;
    
    return NULL; 
}

// Enables iteration over all of the values that produced that same key.
// returns a pointer to the value passed in by the MR_Emit()
char *get_next(char *key, int partition_number)
{   
    key_valueNode *curr_keyValue = reduceList[partition_number];

    if(curr_keyValue == NULL) {
        printf("is here\n");
        return NULL;
    }
    while(curr_keyValue != NULL) {
        if(strcmp(curr_keyValue->key, key) == 0) {
            --numArray;
            printf("num Array: %d\n", numArray);
            return curr_keyValue->values[numArray+1];
        }
    }
    return NULL;
}


// this function decide which partition (and hence, which reducer thread)
// gets a particular key/list of values to process
// For some apps, which reducer thread processes a particular key is not important
// (and thus the default is this function passed in to MR_Run())
unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    //printf("this is hash %lu\n", hash % num_partitions);
    return hash % num_partitions;
}

// Store <key, value> into partition
//     * If key already exists, append the value to the of existing value list
//     * Else, find the partition this <key, value> should go to and hash it into the 
//     * partition's hash table 
void MR_Emit(char *key, char *value) 
{
    //printf("Inserting: %s, %s\n", key, value);
    //printf("In MR_EMIT\n");

    if(key == NULL || value == NULL) {
        printf("invalid args to MR_Emit\n");
        exit(1);
    }
    // Determine the partition number for this key
    unsigned long partition_Number;
    
    if(partitioner != NULL) {
        partition_Number = partitioner(key, partition_number);
    } else {
        //printf("this is MR Emit partition_number %d\n", partition_number);
        partition_Number = MR_DefaultHashPartition(key, partition_number);
    }
    //printf("this is key %s\n", key);
    //printf("this is value %s\n", value);
    ht_insert(partitionHashT[partition_Number], key, value);
    
}

void ht_free(struct hashTable_Bucket *hashTable)
{
    for(int i = 0; i < HASH_SIZE; ++i) {
        key_valueNode *keyValue = hashTable->entries[i];
        pthread_mutex_t *lock_threads = &(hashTable->locks[i]);
        while(keyValue) {
            free(keyValue->key);
	        free(keyValue->values[0]);
	        free(keyValue->values);
	        free(keyValue);
        }
        pthread_mutex_destroy(lock_threads);
    }
    free(hashTable->entries);
    free(hashTable->locks);
    free(hashTable);  
}

//--------------------------------------------------------------------------
// Memory management
// When the entire mapping and reduction is complete, it is the responsibility 
// of the MR library to free everything
// Free memory
//--------------------------------------------------------------------------
void MR_Run(int argc, char *argv[], Mapper map, int num_mappers, Reducer reduce, int num_reducers, Partitioner partition) 
{
    printf("In MR_RUN\n");
    num_files = argc - 1;
    current_file = 1;
    //printf("numOfFilesLeft %d\n", numOfFilesLeft);
    // Set partitioner and mapper - reducer
    mapper = map;
    reducer = reduce;
    partitioner = partition;
    partition_number = num_reducers;
    partitionHashT = malloc(sizeof(struct hashTable_Bucket *)*num_reducers);

    // Check for valid arguments
    if(argc < 1) {
        printf("Bad argument. Usage: ./Mapreducer <filename_0> <filename_1> <filename_2> .....\n");
    }
    // Create Partitions
    for(int i = 0; i <= partition_number; i++) {
        struct hashTable_Bucket *ht_partition = ht_create();
        partitionHashT[i] = ht_partition;
    }
    printf("In MrRUN numkeys %lu\n", partitionHashT[0]->numKeys);
    
    // create num_mappers threads that perform the map tasks
    pthread_t mappers[num_mappers];
    for(int i = 0; i < num_mappers; i++) {
        Pthread_create(&mappers[i], NULL, GetFileAndMap, (void *)argv);
    }

    // for(int i = 0; num_mappers; i++) {
    //     Pthread_join(mappers[i], NULL);
    // }

    pthread_t reducers[num_reducers];
    reduceList = malloc(sizeof(key_valueNode*) * num_reducers);
    // create nnum_reducers threads to perform the reduction tasks
    for(int i = 0; i < num_reducers; i++) {
        Pthread_create(&reducers[i], NULL, SortPartitionAndReduce, NULL);
    }

    for(int i = 0; i < num_reducers; i++) {
        Pthread_join(reducers[i], NULL);
    }

    // Memory clean-up
    for(int i = 0; i < partition_number; i++) {
        ht_free(partitionHashT[i]);
    }
    free(partitionLock);
    free(reduceList);
    free(partitionHashT);
}

void Map(char* file_name)
{
    printf("In Map\n");
	FILE *fp=fopen(file_name,"r");
	assert(fp!=NULL);

	char* line=NULL;
	size_t size=0;
	while(getline(&line,&size,fp)!=-1)
	{
		//token, dummy equals to a line in the file
		char *token, *dummy=line;
		//while curr pointer doesn't reach the end of current line
		while((token=strsep(&dummy," \t\n\r"))!=NULL)
		{
			MR_Emit(token,"1");
		}
	}
	free(line); 
	fclose(fp);
}

void Reduce(char *key, Getter get_next, int partition_number)
{
	int count=0;
	char* value;
	//iterate all the values that produced the same key
	while((value=get_next(key,partition_number))!=NULL)
		count++;
	printf("%s %d\n",key,count);
}

int main(int argc, char* argv[])
{
	//MR_Run(argc, argv, Map, 1, Reduce, 1, NULL);
    hashTable_Bucket *ht = ht_create();

    ht_insert(ht, "key1", "1");
    ht_insert(ht, "key2", "1");
    ht_insert(ht, "key1", "2");

    display_ht(ht);
	return 0;
}

// int main(int argc, char* argv[]) 
// {
//     hashTable_Bucket *ht = ht_create();

//     ht_insert(ht, "key1", "1");
//     ht_insert(ht, "key2", "1");
//     ht_insert(ht, "key3", "1");
//     ht_insert(ht, "key4", "1");
//     ht_insert(ht, "key5", "1");
//     ht_insert(ht, "key6", "1");
//     ht_insert(ht, "key7", "1");

//     ht_insert(ht, "key1", "2");
//     ht_insert(ht, "key1", "3");
//     ht_insert(ht, "key1", "4");

//     display_ht(ht);

//     printf("get_next: %s\n", get_next(ht, "key1", 1));
//     printf("get_next: %s\n", get_next(ht, "key1", 1));
//     printf("get_next: %s\n", get_next(ht, "key1", 1));
//     printf("get_next: %s\n", get_next(ht, "key1", 1));
//     printf("get_next: %s\n", get_next(ht, "key1", 1));

//     return 0;
// }
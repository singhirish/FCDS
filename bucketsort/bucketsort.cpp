#include <stdlib.h>
#include <string.h>
#include "bucketsort.h"
#include <stdio.h>
#include <thread>

using namespace std;

// 94 buckets, because we allow a range of 94 different characters
#define N_BUCKETS 94

typedef struct {
	long int *data;
	int length;
	long int total;
} bucket;

void sort_with_index(char* a, bucket *bucket, int index) {
	int i, length;
	long int key;
	key = bucket->data[index];
	length = bucket->length;
	i = index - 1;
	while (i >= 0
			&& strcmp(a + bucket->data[i] * length, a + key * length) > 0) {
		bucket->data[i + 1] = bucket->data[i];
		i--;
	}
	bucket->data[i + 1] = key;
}

void sort(char* a, bucket *buckets, int threadNumber, int threadSize) {
	int j;

	for (int k = threadNumber; k < N_BUCKETS; k += threadSize) {
		bucket *_bucket = &buckets[k];
		for (j = 0; j < _bucket->total; j++) {
			sort_with_index(a, _bucket, j);
		}	
	}
}

// returns the sorted indices
long int* bucket_sort(char *a, int length, long int size, int numberOfThreads) {

	long int i;
	bucket buckets[N_BUCKETS], *b;
	long int *returns;

	// allocate memory
	returns = (long int *) malloc(sizeof(long int) * size);
	for (i = 0; i < N_BUCKETS; i++) {
		// write starting-address of bucket's section to data
		buckets[i].data = returns + i * size / N_BUCKETS;
		buckets[i].length = length;
		buckets[i].total = 0;
	}

	// copy the keys to "buckets"
	for (i = 0; i < size; i++) {
		// Select the next element (starting at position a (the address of the array of characters), plus the element's index and its length, and to get 0-based indices subtract the minimum input 			// character 0x21), to get the bucket that needs to receive the currenlty processed element

		// The calculation a + i * length retrieves the first character of the next element that is to be placed in a bucket (e.g. if the number 95 would have to be sorted, the number 9 would be
		// retrieved and sorted into the bucket for the range 90-100.
		b = &buckets[*(a + i * length) - 0x21];
		// Write to position total++ (the latest element's index) the currently processed index i
		b->data[b->total++] = i;
	}
	thread threads[numberOfThreads];

	// sort each "bucket"
	for (i = 0; i < numberOfThreads; i++) {
		threads[i] = thread(sort, a, buckets, i, numberOfThreads);
	}

	for (i = 0; i < numberOfThreads; i++) {
		threads[i].join();
	}	

	//for (auto& th : threads) th.join();

	return returns;
}

/* @author St\'efano Drimon Kurz M\'or */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ThreadPool.h"
#include <thread>
#include <iostream>

using namespace std;

typedef struct _item_t {
	int value;   // v_i
	int weight;  // w_i
	float density; // v_i/w_i
} item_t;

FILE *fin, *fout;
int *results;

void openfile(char* in) {
	fin = fopen(in, "r+");
	if (fin == NULL) {
		perror("fopen fin");
		exit(EXIT_FAILURE);
	}
}

int greater_f(const int x, const int y);
int compare_f(const void *x, const void *y);

int _knapsack_f(const int n, const int M, const item_t * const items, bool initialCall, int index) {
	int v = 0, w = 0;
	int r = 0;

	if (initialCall) {
		v = items[0].value;
		w = items[0].weight;
	}

	if (n < 1)
		return 0;

	while (M - w >= 0) {
		r = greater_f(r, v + _knapsack_f(n - 1, M - w, &items[1], false, index));
		v += items[0].value;
		w += items[0].weight;
	}

	results[index] = r;

	return r;
}

void knapsack_f(const int n, const int M, const item_t * const items, int numberOfThreads) {
	//thread threads[n];
	{
		ThreadPool pool(numberOfThreads);
		results = new int[n];

		for (int i = 0; i < n; i++) {
			//threads[i] = thread(_knapsack_f, n - i, M, &(items[i]), true, i);
			pool.enqueue(_knapsack_f, n - i, M, &(items[i]), true, i);
		}
	}

	int max = 0;
	for (int i = 0; i < n; i++) {
		max = greater_f(max, results[i]);
	}

	cout << max << endl;
}

int main(int argc, char *argv[]) {

	int n;
	int M;
	item_t *items;

	int i;

	int numberOfThreads = 1;

	if (argc == 2) {
		numberOfThreads = atoi(argv[1]);
	}
	 
	scanf("%d %d", &n, &M);
	items = (item_t*) calloc(n, sizeof(item_t));

	for (i = 0; i < n; ++i) {
		scanf("%d %d", &(items[i].value), &(items[i].weight));
		items[i].density = (float) (items[i].value) / items[i].weight;
	}

	qsort(items, (size_t) n, sizeof(item_t), compare_f);
	

	knapsack_f(n, M, items, numberOfThreads);

	free(items);

	return 0;

}

int greater_f(const int x, const int y) {
	return (x > y) ? x : y;
}

int compare_f(const void *x, const void *y) {
	return ((item_t*) x)->density - ((item_t*) y)->density;
}


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <thread>
#include "string.h"
#include "ThreadPool.h"

using namespace std;

int gcd(int u, int v) {
	if (v == 0)
		return u;
	return gcd(v, u % v);
}

void _friendly_numbers(long int i, long int start, long int end, long int **_the_num, long int **_num, long int **_den, int threadOffset, int indexOffset) {
	for (i = start + threadOffset; i <= end; i += indexOffset) {
		long int * the_num = *_the_num;
		long int * num = *_num;
		long int * den = *_den;

		long int factor, ii, sum, done, n;

		ii = i - start;
		sum = 1 + i;
		the_num[ii] = i;
		done = i;
		factor = 2;
		while (factor < done) {
			if ((i % factor) == 0) {
				sum += (factor + (i / factor));
				if ((done = i / factor) == factor)
					sum -= factor;
			}
			factor++;
		}
		num[ii] = sum;
		den[ii] = i;
		n = gcd(num[ii], den[ii]);
		num[ii] /= n;
		den[ii] /= n;
	}
}

void friendly_numbers(long int start, long int end) {
	long int last = end - start + 1;

	long int *the_num;
	the_num = (long int*) malloc(sizeof(long int) * last);
	long int *num;
	num = (long int*) malloc(sizeof(long int) * last);
	long int *den;
	den = (long int*) malloc(sizeof(long int) * last);

	long int i, j;

	{
		ThreadPool pool(4);
		for (int threadIndex = 0; threadIndex < 4; threadIndex++) {
			pool.enqueue(_friendly_numbers, i, start, end, &the_num, &num, &den, threadIndex, 4);
		}
	}	

	for (i = 0; i < last; i++) {
		for (j = i + 1; j < last; j++) {
			if ((num[i] == num[j]) && (den[i] == den[j]))
				printf("%ld and %ld are FRIENDLY\n", the_num[i], the_num[j]);
		}
	}

	free(the_num);
	free(num);
	free(den);
}

int main(int argc, char **argv) {
	long int start;
	long int end;

	while (1) {
		scanf("%ld %ld", &start, &end);
		if (start == 0 && end == 0)
			break;
		printf("Number %ld to %ld\n", start, end);
		friendly_numbers(start, end);
	}

	return EXIT_SUCCESS;
}

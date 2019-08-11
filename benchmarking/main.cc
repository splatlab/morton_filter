/*
 * ============================================================================
 *
 *       Filename:  main.cc
 *
 *         Author:  Prashant Pandey (), ppandey2@cs.cmu.edu
 *   Organization:  Carnegie Mellon University
 *
 * ============================================================================
 */

#include <iostream>
#include <stdint.h>
#include <immintrin.h>  // portable to all x86 compilers
#include <tmmintrin.h>
#include <openssl/rand.h>
#include <sys/time.h>

#include "../morton_filter.h"

using namespace CompressedCuckoo;

/* Print elapsed time using the start and end timeval */
void print_time_elapsed(const char* desc, struct timeval* start, struct
												timeval* end)
{
	struct timeval elapsed;
	if (start->tv_usec > end->tv_usec) {
		end->tv_usec += 1000000;
		end->tv_sec--;
	}
	elapsed.tv_usec = end->tv_usec - start->tv_usec;
	elapsed.tv_sec = end->tv_sec - start->tv_sec;
	float time_elapsed = (elapsed.tv_sec * 1000000 + elapsed.tv_usec)/1000000.f;
	printf("%s Total Time Elapsed: %f seconds", desc, time_elapsed);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Please specify the log of the number of slots in the CQF.\n");
		exit(1);
	}
	uint64_t qbits = atoi(argv[1]);
	uint64_t nslots = (1ULL << qbits);
	uint64_t nvals = 85*nslots/100;
	uint64_t *vals;

	/* initialize ququ filter */
	Morton3_8 filter(nslots);

	/* Generate random values */
	vals = (uint64_t*)malloc(nvals*sizeof(vals[0]));
	RAND_bytes((unsigned char *)vals, sizeof(*vals) * nvals);
	srand(0);
	for (uint64_t i = 0; i < nvals; i++) {
		//vals[i] = rand() % filter.metadata->range;
		vals[i] = (1 * vals[i]) % ((1ULL << (qbits + 8)) - 1);
	}

	struct timeval start, end;
	struct timezone tzp;

	gettimeofday(&start, &tzp);
	/* Insert hashes in the ququ filter */
	for (uint64_t i = 0; i < nvals; i++) {
		if (!filter.insert(vals[i])) {
			fprintf(stderr, "Insertion failed");
			exit(EXIT_FAILURE);
		}
	}
	gettimeofday(&end, &tzp);
	print_time_elapsed("Insertion time", &start, &end);
	puts("");
	gettimeofday(&start, &tzp);
	for (uint64_t i = 0; i < nvals; i++) {
		if (!filter.likely_contains(vals[i])) {
			fprintf(stderr, "Lookup failed for %ld", vals[i]);
			exit(EXIT_FAILURE);
		}
	}
	gettimeofday(&end, &tzp);
	print_time_elapsed("Lookup time", &start, &end);

	return 0;
}

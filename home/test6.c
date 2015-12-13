#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define MAX_ALLOCS 120
#define MAX_TOTAL_ALLOCS 1200
#define RAND_SEED 12345
#define MAPPING_SIZE 8
#define DEALLOC_FALLDOWN 0.95

struct alloc {
	void* address;
	int size;
};

struct benchmark {
	long mapping_size;
	long free_space;

	struct alloc allocs[MAX_TOTAL_ALLOCS];
	int allocs_size;

	int max_deallocs;
	bool deallocate_after_fail;

	long allocs_number;
	long deallocs_number;
	long runs;

	clock_t start_time;
	clock_t end_time;
};

struct benchmark benchmark;

void run_benchmark(int mean_size, int jitter);

int run_strategies_benchmarks(int mean_size, int jitter) {
  printf("WORST FIT STRATEGY\n");
  tso_switch_strategy(WORST_FIT);
  srand(RAND_SEED);
  run_benchmark(mean_size,jitter);
  printf("\n\nBEST FIT STRATEGY\n");
  tso_switch_strategy(BEST_FIT);
  srand(RAND_SEED);
  run_benchmark(mean_size,jitter);
  printf("\n\nFIRST FIT STRATEGY\n");
  tso_switch_strategy(FIRST_FIT);
  srand(RAND_SEED);
  run_benchmark(mean_size,jitter);
}

int run_all_benchmarks(int mean_size) {

	int low_jitter = mean_size / 10;
	int medium_jitter = mean_size / 5;
	int high_jitter = mean_size / 2;
	int very_high_jitter = mean_size;

	printf("NO FRAGMENTATION\n\n");
	run_benchmark(mean_size, 0);
	printf("\n\n*****************\n\n");

	printf("SMALL FRAGMENTATION\n\n");
	run_benchmark(mean_size, low_jitter);
	printf("\n\n*****************\n\n");

	printf("MEDIUM FRAGMENTATION\n\n");
	run_benchmark(mean_size, medium_jitter);
	printf("\n\n*****************\n\n");

	printf("HIGH FRAGMENTATION\n\n");
	run_benchmark(mean_size, high_jitter);
	printf("\n\n*****************\n\n");

	printf("VERY HIGH FRAGMENTATION\n\n");
	run_benchmark(mean_size, very_high_jitter);
	printf("\n\n*****************\n\n");

}

int main(int argc, char const *argv[])
{

  printf("RANDOM ALLOCATION USAGE TEST\n\n");
  printf("Parameters:\n");
  printf("Mean size | Size jitter | Deallocate after fail | MAX_ALLOCS | DEALLOCS_FALLDOWN | STRATEGY\n\n");

  printf("MAX_ALLOCS: %ld\n", MAX_ALLOCS);
  printf("RAND_MAX: %ld\n", RAND_MAX);

  benchmark.deallocate_after_fail = true;

  printf("SMALL CHUNKS\n");
  run_all_benchmarks(4);
  printf("MEDIUM CHUNKS\n");
  run_all_benchmarks(128);
  printf("BIG CHUNKS\n");
  run_all_benchmarks(512);

  return 0;

}

bool run_allocations(int mean_size, int jitter) {

	// Alloc de una cantidad al azar [0-cantidad_disponible_de_allocs]
	// de un tamano mean_size +- jitter y ordenados al azar en un array
	// utilizando una variacion del algoritmo Fisher-Yates inside-out

	int max_allocs = MAX_ALLOCS < MAX_TOTAL_ALLOCS - benchmark.allocs_size ? MAX_ALLOCS : MAX_TOTAL_ALLOCS - benchmark.allocs_size;
	int up_to = rand() % max_allocs;
	if(up_to == 0)
		return false;

	for(int i = 0; i < up_to; i++) {
		int place = place > 0 ? rand() % benchmark.allocs_size : 0;

		if(place != benchmark.allocs_size){
			benchmark.allocs[benchmark.allocs_size] = benchmark.allocs[place]; 
		}
	
		struct alloc new_alloc;

		int expected_size = jitter > 0 ? rand() % (jitter * 2) - jitter + mean_size : mean_size;
		new_alloc.size = expected_size > benchmark.free_space ? benchmark.free_space : expected_size;

		//printf("New size:%ld\n", new_alloc.size);
		new_alloc.address = tso_alloc(new_alloc.size);
		//printf("New address:%p\n", new_alloc.address);
		if(new_alloc.address == NULL) {
			printf("Failed to alloc %d bytes\n", new_alloc.size);
			return false;
		}

		benchmark.allocs[place] = new_alloc;
		benchmark.allocs_size++;

		benchmark.free_space -= new_alloc.size;
		benchmark.allocs_number++;

		if(benchmark.free_space == 0) {
			printf("No more free space \n");
			return false;
		}
		else if(benchmark.free_space < mean_size - jitter) {
			printf("Not enough free space for minimum allocation \n");
			return false;
		}
	}

	return true;

}

bool run_deallocations_down_to(int down_to) {
	

	for(; benchmark.allocs_size > down_to; benchmark.allocs_size--) {
		struct alloc old_alloc = benchmark.allocs[benchmark.allocs_size - 1];
		tso_free(old_alloc.address);
		benchmark.deallocs_number++;
		benchmark.free_space += old_alloc.size;
	}


	return true;
}

bool run_full_deallocation() {
	run_deallocations_down_to(0);
	printf("Free: %ld | Allocs_size: %d",benchmark.free_space, benchmark.allocs_size);
}

bool run_deallocations() {
	// Dealloc de una cantidad al azar [0-cantidad_de_allocs]
	// del array
	int expected_deallocs = rand() % benchmark.allocs_size;
	int down_to = benchmark.allocs_size - (expected_deallocs > benchmark.max_deallocs ? benchmark.max_deallocs : expected_deallocs);
	run_deallocations_down_to(down_to);
	benchmark.max_deallocs = (int)(benchmark.max_deallocs * DEALLOC_FALLDOWN);
}

void run_benchmark(int mean_size, int jitter) {
	
	benchmark.mapping_size = getpagesize() * MAPPING_SIZE;
	benchmark.free_space = benchmark.mapping_size;
	benchmark.max_deallocs = MAX_ALLOCS;

	benchmark.allocs_size = 0;

	benchmark.allocs_number = 0;
	benchmark.deallocs_number = 0;
	benchmark.runs = 0;

	benchmark.start_time = clock();

	printf("RUNNING ALLOC TEST\n");
	printf("Mapping size: %ld\n", benchmark.mapping_size);
	printf("benchmark.deallocate_after_fail: %s\n", benchmark.deallocate_after_fail ? "true" : "false");

	int previous_allocs = 0;
	int previous_deallocs = 0;

	bool alloc_result;
	while(true) {
		benchmark.runs++;
		alloc_result = run_allocations(mean_size, jitter);
		if(!alloc_result && (benchmark.max_deallocs == 0 || !benchmark.deallocate_after_fail))
			break;
		int old_max_deallocs = benchmark.max_deallocs;
		run_deallocations(benchmark);
		printf("Allocs: %d | Deallocs: %d | ",
			benchmark.allocs_number - previous_allocs,
			benchmark.deallocs_number - previous_deallocs);
		printf("Running diference: %d | Max deallocs: %d\n",
			benchmark.allocs_number - benchmark.deallocs_number, old_max_deallocs);
		previous_allocs = benchmark.allocs_number;
		previous_deallocs = benchmark.deallocs_number;
	}

	benchmark.end_time = clock();

	double total_run_time = (double)(benchmark.end_time - benchmark.start_time) / CLOCKS_PER_SEC;
	double avg_alloc_time =  total_run_time / benchmark.allocs_number * 1000;
	
	printf("\nALLOC TEST STOPPEP\n");
	printf(" * Number of alloc-dealloc runs: %ld\n", benchmark.runs);
	printf(" * Number of allocs: %ld\n", benchmark.allocs_number);
	printf(" * Total run time: %f s\n", total_run_time);
	printf(" * Average time per alloc: %f ms\n", avg_alloc_time);
	printf(" * Initial size: %ld\n", benchmark.mapping_size);
	printf(" * Space remainging: %ld\n", benchmark.free_space);
	printf(" * Wasted space: %f\%\n", (float)benchmark.free_space / benchmark.mapping_size * 100);
	printf(" * Free/(Mean size) ratio: %f\n", (float)benchmark.free_space / mean_size);
	printf(" * (Mean size)/Jitter ratio: %f\n", (float)jitter / mean_size);

	run_full_deallocation();

}

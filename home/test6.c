#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define REGION_SIZE 8
#define DO_PRINT false
#define MAX_TOTAL_ALLOCS 12000
#define RAND_SEED 12345
#define MAPPING_SIZE 8

struct alloc {
	void* address;
	int size;
};

struct parameters {

	char* description;

	bool deallocate_after_fail;
	int jitter;
	int mean;
	double dealloc_falldown;
	int max_allocs;
	ALLOCATION_STRATEGY strategy;
};

struct benchmark {
	long mapping_size;
	long free_space;

	struct alloc allocs[MAX_TOTAL_ALLOCS];
	int allocs_size;

	int max_deallocs;

	long allocs_number;
	long deallocs_number;
	long runs;

	clock_t start_time;
	clock_t end_time;
};

struct result {

	char description[500];
	ALLOCATION_STRATEGY strategy;

	int max_allocs;
	int mean;
	int jitter;
	float dealloc_falldown;
	int total_bytes;
	bool deallocate_after_fail;

	long allocs_number;
	long deallocs_number;
	double avg_alloc_time;

	double mean_jitter_ratio;
	double free_mean_ratio;
	double total_mean_ratio;

	int free_bytes;
	double wasted_space;
	double wasted_space_overhead_accounted;
	double overhead_percentage;
	
};

struct result results[50];
int current_result = -1;

struct benchmark benchmark;

void run_benchmark(struct parameters p);

void init_benchmark(struct parameters p) {
	current_result++;
  srand(RAND_SEED);
	tso_switch_strategy(p.strategy);

  printf("STARTING TEST: %s\n",p.description + 1);
  fflush(stdout);
}

void add_to_description(char* str, struct parameters* p) {
	char* d = malloc(255);
	sprintf(d,"%s %s", p->description, str);
	p->description = d;
}

int run_strategies_benchmarks(int mean, int jitter, struct parameters p) {

	p.mean = mean;
	p.jitter = jitter;

	struct parameters p1, p2, p3;
	p1 = p2 = p3 = p;

	add_to_description("WORST FIT STRATEGY", &p1);
	p1.strategy = WORST_FIT;
  run_benchmark(p1);

 	add_to_description("BEST FIT STRATEGY", &p2);
	p2.strategy = BEST_FIT;
  run_benchmark(p2);

  add_to_description("FIRST FIT STRATEGY", &p3);
	p3.strategy = FIRST_FIT;
  run_benchmark(p3);

}

int run_all_benchmarks(int mean, struct parameters p) {

	int low_jitter = mean / 10;
	int medium_jitter = mean / 5;
	int high_jitter = mean / 2;
	int very_high_jitter = mean;

	struct parameters p1, p2, p3, p4, p5;
	p1 = p2 = p3 = p4 = p5 = p;

	/*add_to_description("NO FRAGMENTATION", &p1);
	run_strategies_benchmarks(mean, 0, p1);*/

	add_to_description("SMALL FRAGMENTATION", &p2);
	run_strategies_benchmarks(mean, low_jitter, p2);

	add_to_description("MEDIUM FRAGMENTATION", &p3);
	run_strategies_benchmarks(mean, medium_jitter, p3);

	add_to_description("HIGH FRAGMENTATION", &p4);
	run_strategies_benchmarks(mean, high_jitter, p4);

	add_to_description("VERY HIGH FRAGMENTATION", &p5);
	run_strategies_benchmarks(mean, very_high_jitter, p5);

}

bool print_results() {
	
	printf("strategy|description|max_allocs|mean|jitter|dealloc_falldown|total_bytes"  
		"|deallocate_after_fail|allocs_number|deallocs_number|avg_alloc_time|mean_jitter_ratio|" 
		"free_mean_ratio|total_mean_ratio|free_bytes|wasted_space|wasted_space_overhead_accounted|overhead_percentage\n");

	for(int i = 0; i <= current_result; i++) {

		struct result r = results[i];

		printf("%d|%s|%d|%d|%d|%f|%d|%s|%ld|%ld|%f|%f|%f|%f|%d|%f|%f|%f\n",
			r.strategy,r.description,r.max_allocs,r.mean,r.jitter,r.dealloc_falldown,
			r.total_bytes,r.deallocate_after_fail ? "true" : "false",r.allocs_number,r.deallocs_number,r.avg_alloc_time,
			r.mean_jitter_ratio,r.free_mean_ratio,r.total_mean_ratio,r.free_bytes,r.wasted_space,
			r.wasted_space_overhead_accounted,r.overhead_percentage);	

	}

}

int main(int argc, char const *argv[])
{

  if(DO_PRINT)
    printf("RANDOM ALLOCATION USAGE TEST\n\n");
	if(DO_PRINT)
    printf("STARTING...\n");

	struct parameters p;

	p.description = malloc(500);
	sprintf(p.description,"");

  p.deallocate_after_fail = true;
	p.dealloc_falldown = 0.8;
	p.max_allocs = 120;

	struct parameters p1, p2, p3;
	p1 = p2 = p3 = p;

  /*add_to_description("SMALL CHUNKS", &p1);
  run_all_benchmarks(32, p1);*/
  add_to_description("MEDIUM CHUNKS", &p2);
  run_all_benchmarks(128, p2);
  add_to_description("BIG CHUNKS", &p3);
  run_all_benchmarks(512, p3);

	print_results();

  return 0;

}

bool run_allocations(struct parameters p) {

	// Alloc de una cantidad al azar [0-cantidad_disponible_de_allocs]
	// de un tamano mean +- jitter y ordenados al azar en un array
	// utilizando una variacion del algoritmo Fisher-Yates inside-out

	int max_allocs = p.max_allocs < MAX_TOTAL_ALLOCS - benchmark.allocs_size ? p.max_allocs : MAX_TOTAL_ALLOCS - benchmark.allocs_size;
	int up_to = rand() % max_allocs;

	if(up_to == 0) {
		if(DO_PRINT)
      printf("No more space in array to allocate!");
		return false;
	}

	for(int i = 0; i < up_to; i++) {
		int place = place > 0 ? rand() % benchmark.allocs_size : 0;

		if(place != benchmark.allocs_size){
			benchmark.allocs[benchmark.allocs_size] = benchmark.allocs[place]; 
		}
	
		struct alloc new_alloc;

		int expected_size = p.jitter > 0 ? rand() % (p.jitter * 2) - p.jitter + p.mean : p.mean;
		new_alloc.size = expected_size > benchmark.free_space ? benchmark.free_space : expected_size;

		new_alloc.address = tso_alloc(new_alloc.size);

		if(new_alloc.address == NULL) {
			if(DO_PRINT)
        printf("Failed to alloc %d bytes\n", new_alloc.size);
			return false;
		}

		benchmark.allocs[place] = new_alloc;
		benchmark.allocs_size++;

		benchmark.free_space -= new_alloc.size;
		benchmark.allocs_number++;

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
	if(DO_PRINT)
      printf("Free: %ld | Allocs_size: %d\n",benchmark.free_space, benchmark.allocs_size);
}

bool run_deallocations(struct parameters p) {
	// Dealloc de una cantidad al azar [0-cantidad_de_allocs]
	// del array
	int expected_deallocs = rand() % benchmark.allocs_size;
	int down_to = benchmark.allocs_size - (expected_deallocs > benchmark.max_deallocs ? benchmark.max_deallocs : expected_deallocs);
	run_deallocations_down_to(down_to);
	benchmark.max_deallocs = (int)(benchmark.max_deallocs * p.dealloc_falldown);
}

void save_result(struct parameters p) {
	struct result r;

	strncpy(r.description, p.description, 255);

	r.max_allocs = p.max_allocs;
	r.mean = p.mean;
	r.jitter = p.jitter;
	r.dealloc_falldown = p.dealloc_falldown;
	r.total_bytes = benchmark.mapping_size;
	r.deallocate_after_fail = p.deallocate_after_fail;

	r.allocs_number = benchmark.allocs_number;
	r.deallocs_number = benchmark.deallocs_number;


	double total_run_time = (double)(benchmark.end_time - benchmark.start_time) / CLOCKS_PER_SEC;
	double avg_alloc_time =  total_run_time / benchmark.allocs_number * 1000;
	
	r.avg_alloc_time = avg_alloc_time;

	r.free_bytes = benchmark.free_space;
	r.wasted_space = (double)benchmark.free_space / benchmark.mapping_size * 100;
	r.mean_jitter_ratio = (double)p.jitter / p.mean;
	r.free_mean_ratio = (double)benchmark.free_space / p.mean;
	r.total_mean_ratio = (double)benchmark.mapping_size / p.mean;

	long overhead = REGION_SIZE * benchmark.allocs_size;

	r.wasted_space_overhead_accounted = (double)benchmark.free_space / (benchmark.mapping_size - overhead) * 100;
	r.overhead_percentage = (double)overhead / (benchmark.mapping_size - benchmark.free_space) * 100;

	results[current_result] = r;

}

void run_benchmark(struct parameters p) {
	

	init_benchmark(p);

	benchmark.mapping_size = getpagesize() * MAPPING_SIZE;
	benchmark.free_space = benchmark.mapping_size;
	benchmark.max_deallocs = p.max_allocs;

	benchmark.allocs_size = 0;

	benchmark.allocs_number = 0;
	benchmark.deallocs_number = 0;
	benchmark.runs = 0;

	benchmark.start_time = clock();

	int previous_allocs = 0;
	int previous_deallocs = 0;

	bool alloc_result;

	while(true) {
		benchmark.runs++;
		alloc_result = run_allocations(p);
		if(!alloc_result && (benchmark.max_deallocs == 0 || !p.deallocate_after_fail))
			break;
		int old_max_deallocs = benchmark.max_deallocs;
		run_deallocations(p);
		if(DO_PRINT)
      printf("Allocs: %d | Deallocs: %d | ",
		benchmark.allocs_number - previous_allocs,
		benchmark.deallocs_number - previous_deallocs);
		if(DO_PRINT)
      printf("Running diference: %d | Max deallocs: %d\n",
		benchmark.allocs_number - benchmark.deallocs_number, old_max_deallocs);
		previous_allocs = benchmark.allocs_number;
		previous_deallocs = benchmark.deallocs_number;
	}

	benchmark.end_time = clock();

	save_result(p);

	if(DO_PRINT) {
    printf("TEST STOPPED\n");
  	printf("FULL DEALLOCATION\n");
  }
	run_full_deallocation();

}

/**
  * ECE254 Linux Dynamic Memory Management Lab
  * @file: main_test.c
  * @brief: The main file to write tests of memory allocation algorithms
  */ 

/* includes */
/* system provided header files. You may add more */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* non-system provided header files. 
   Do not include more user-defined header files here
 */
#include "mem.h"

int main(int argc, char *argv[]){
	int num = 0;
	int algo = 0; // default algorithm to test is best fit  
	// void *p1, *p2, *p3, *p4, *q;
	void *p[20];
	void *q[20];
	block_ptr *tmp;
	int i, size, can_fit;
	
	can_fit = 0;
	
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <0/1>. 0 for best fit and 1 for worst fit \n", argv[0]);
		exit (1);
	} else if (!strcmp(argv[1], "1") || !strcmp(argv[1], "0")) {
		algo = atoi(argv[1]);
	} else {
		fprintf(stderr, "Invalid argument, please specify 0 or 1\n");
		exit(1);
	}
	
	if ( algo == 0 ) {
		printf(" Start Best fit test \n");
		best_fit_memory_init(8192);	// initizae 2KB, best fit
		// printf("my Stucture Size: sz=%d\n", struct_size);
		
		for(i = 0; i < 20; i++){
			size = 380 + 10*(i%3 + 1);
			p[i] = best_fit_alloc(size);
			// print_list(0);
		}
		
		tmp = MemBlock;
		// cut the memeory into many blocks with different sizes
		// printf("Cut, change the occupied state for each Block\n");
		while(tmp){
			
			tmp->occupied = 0;
			tmp = tmp->next;
			
		}
		
		printf("Allocate more =================================================\n");
		
		for(i = 0; i < 30; i++){
			size = 40 + 4*(i%4 + 1);
			q[i] = best_fit_alloc(size);
			print_list(0);
			if(q[i]){
				can_fit++;
			}
		}
		
		// if ( p != NULL ) {
			// best_fit_dealloc(p);	
		// }
		num = best_fit_count_extfrag(380);
		printf(" Best fit test \n");
		
	} else if ( algo == 1 ) {

		printf(" Start Worst fit test \n");
		worst_fit_memory_init(8192);	// initizae 2KB, best fit
		// printf("my Stucture Size: sz=%d\n", struct_size);
		
		for(i = 0; i < 20; i++){
			size = 380 + 10*(i%3 + 1);
			p[i] = worst_fit_alloc(size);
			
		}
		
		tmp = MemBlock;
		// cut the memeory into many blocks with different sizes
		// printf("Cut, change the occupied state for each Block\n");
		while(tmp){
			
			tmp->occupied = 0;
			tmp = tmp->next;
			
		}
		
		printf("Allocate more =================================================\n");
		
		for(i = 0; i < 30; i++){
			size = 40 + 4*(i%4 + 1);
			q[i] = worst_fit_alloc(size);
			print_list(0);
			
			if(q[i]){
				can_fit++;
			}
		}
		
		// if ( p != NULL ) {
			// best_fit_dealloc(p);	
		// }
		num = worst_fit_count_extfrag(380);
		printf(" Worst fit test \n");
	} else {
		fprintf(stderr, "Should not reach here!\n");
		exit(1);
	}

	printf("num = %d\n", num);
	printf("can_fit = %d\n", can_fit);
	return 0;
}

void print_list(int algo) { //print the linked list
	block_ptr * current;
	if (algo == 0) {
		current = MemBlock;
	}


	while (current != NULL) {
		printf("Address: %d | Size: %lu | Using: %d | Next: %d | \n", (size_t)current - base, current->size, current->occupied, (size_t)current->next - base);
		current = current->next;
	}
	printf("\n");
}
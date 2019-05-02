/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include "mem.h"

/* defines */
typedef struct Block {
	int occupied;
	// void *base; 
	int size;
	struct Block *previous;
	struct Block *next;
}block_ptr;

// used to calculate the size of the 
int struct_size = sizeof(block_ptr);
int base;
// DEFINE int INF = 999999;

/* global varaiables */
block_ptr *MemBlock;
block_ptr *headerBlock;
/* Functions */

/* memory initializer */
int best_fit_memory_init(size_t size)
{
//	struct Block *MemBlock;
	
	// You call malloc once here to obtain the memory to be managed.	
	MemBlock = malloc(size);
	
	if(!MemBlock || size%4 != 0 || size <= 0){
		return -1;
	}
	
	MemBlock->size = size;
	MemBlock->occupied = 0;
	MemBlock->previous = NULL;
	MemBlock->next = NULL;
	
	base = (size_t)MemBlock;
	// printf(" Base Value %d\n", base);
	
	// print_list(0);

	return 0;

}

int worst_fit_memory_init(size_t size)
{

//	struct Block *MemBlock;
	
	// You call malloc once here to obtain the memory to be managed.	
	MemBlock = malloc(size);
	
	if(!MemBlock || size%4 != 0 || size == 0){
		return -1;
	}
	
	MemBlock->size = size;
	MemBlock->occupied = 0;
	MemBlock->previous = NULL;
	MemBlock->next = NULL;
	
	base = (size_t)MemBlock;
	// printf(" Base Value %d\n", base);

	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	// these vars used to find the block to alloc
	block_ptr *newBlock, *currentBlock, *smallestBlock;
	int size_alloc, delt, smallest_size;
	int can_fit = 0; 		// a flag to check if the allocation request could be fit
	// int base;
	
	//these vars used to update the linked list after allocating
	block_ptr *nextBlock, *splitBlock;
	int split_size;
	
	if(size <= 0){
		return NULL;
	}
	
	// update the actual size need to be allocated
	size_alloc = size + struct_size;
	delt = 4 - size_alloc%4;
	if(delt == 4){
		delt = 0;
	}
	size_alloc += delt;
	
	// printf(" Actual size alloc %d\n", size_alloc );
	// set the initial blcok ptr and the smallest block size;
	smallest_size = 999999;
	currentBlock = MemBlock;
	smallestBlock = currentBlock;
	

	// printf(" MemBlock Actual address: %d\n", MemBlock );
	// Do linear search to find and update the block with smallest size
	while (currentBlock != NULL ){
		if(currentBlock->occupied == 0 && currentBlock->size < smallest_size && (currentBlock->size)-struct_size >= size_alloc){
			smallestBlock = currentBlock;
			smallest_size = currentBlock->size;
			can_fit = 1;
		}
		
		currentBlock = currentBlock->next;
	}
	
	// printf(" smallestBlock address: %d\n", smallestBlock );

	
	if(can_fit == 0){
		printf(" Cannot find best fit block\n");

		return NULL;
	}
	
	// update the linked list after allocating
	
	splitBlock = (block_ptr *)((size_t)smallestBlock + size_alloc);
	// printf(" splitBlock : %d\n", splitBlock );
	
	// update the Block ptr
	block_ptr *tmp = smallestBlock->next;
	smallestBlock->next = splitBlock;
	// printf(" smallestBlock->next : %d ... should be equal to previous print \n", smallestBlock->next );
	splitBlock->next = tmp;
	// printf(" splitBlock->next : %d\n", splitBlock->next );
	
	if(tmp){
		tmp->previous = splitBlock;
		// printf(" tmp : %d\n", tmp );
	}
	
	splitBlock->previous = smallestBlock;
	// printf(" smallestBlock->size : %d\n", smallestBlock->size );

	
	// update the blcok size
	smallestBlock->size = size_alloc;
	splitBlock->size = smallest_size - size_alloc;

	
	// update the state
	splitBlock->occupied = 0;
	smallestBlock->occupied = 1;
	
	// printf(" Allocate Done! \n", MemBlock );
	// print_list(0);
	
	return smallestBlock;
}


void *worst_fit_alloc(size_t size)
{
		// these vars used to find the block to alloc
	block_ptr *newBlock, *currentBlock, *largestBlock;
	int size_alloc, delt, largest_size;
	int can_fit = 0; 		// a flag to check if the allocation request could be fit
	
	//these vars used to update the linked list after allocating
	block_ptr *nextBlock, *splitBlock;
	int split_size;
	
	if(size <= 0){
		return NULL;
	}
	
	// update the actual size need to be allocated
	size_alloc = size + struct_size;
	delt = 4 - size_alloc%4;
	if(delt == 4){
		delt = 0;
	}
	size_alloc += delt;
	
	
	// set the initial blcok ptr and the smallest block size;
	largest_size = 0;
	currentBlock = MemBlock;
	largestBlock = currentBlock;
	
	// Do linear search to find and update the block with smallest size
	while (currentBlock != NULL ){
		if(currentBlock->occupied == 0 && currentBlock->size > largest_size && (currentBlock->size) - struct_size >= size_alloc){
			largestBlock = currentBlock;
			largest_size = currentBlock->size;
			can_fit = 1;
		}
		
		currentBlock = currentBlock->next;
	}
	
	if(can_fit == 0){
		return NULL;
	}
	
	// update the linked list after allocating
	
	splitBlock = (block_ptr *)((size_t)largestBlock + size_alloc);
	// printf(" splitBlock : %d\n", splitBlock );
	
	// update the Block ptr
	block_ptr *tmp = largestBlock->next;
	largestBlock->next = splitBlock;
	// printf(" largestBlock->next : %d ... should be equal to previous print \n", largestBlock->next );
	splitBlock->next = tmp;
	// printf(" splitBlock->next : %d\n", splitBlock->next );
	
	if(tmp){
		tmp->previous = splitBlock;
		// printf(" tmp : %d\n", tmp );
	}
	
	splitBlock->previous = largestBlock;
	// printf(" largestBlock->size : %d\n", largestBlock->size );

	
	// update the blcok size
	largestBlock->size = size_alloc;
	splitBlock->size = largest_size - size_alloc;

	
	// update the state
	splitBlock->occupied = 0;
	largestBlock->occupied = 1;
	
	// print_list(0);
	return largestBlock;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr_in) 
{
	// 	
		/*local variables*/
	block_ptr *ptr = ptr_in;
	block_ptr *next_node = ptr->next;

	block_ptr *previous_node = ptr->previous;

	int free_block_size = 0;

	//printf("Var finished np\n");
	if (ptr->occupied == 0){
		printf("Double free error!\n");
	}

	// empty current 
	ptr->occupied  = 0;

	//next node available
	if(next_node != NULL){
		//next node empty
		if(next_node->occupied == 0){
			free_block_size = ptr->size + next_node->size;
			ptr->size = free_block_size;
			//next node is not the last node 
			if(next_node->next != NULL){
				//current node point to next next
				ptr->next = next_node->next;
				next_node->next->previous = next_node->previous;
			}else{
				//current become last
				ptr->next = NULL;
			}
			//not sure if you can free
			// free (ptr->next);
		}
	}

	// printf("Check next np\n");

	//previous node available
	if(ptr->previous != NULL){
		//previous node empty
		if(previous_node->occupied == 0){
			
			// update size here:
			free_block_size = ptr->size + previous_node->size;
			
			// Leo: change previous instead of ptr
			ptr->previous->size = free_block_size;
			// printf("Previous size : %d\n", ptr->previous->size);
			// printf("Previous size : %d\n", MemBlock->size);
			
			// ptr->size = free_block_size;
			
			
			//previous node is no the first node
			if(previous_node->previous != NULL){ 
				// ptr->previous = previous_node->previous;
				// previous_node->previous->next = previous_node->next;
				
				
				if(ptr->next){
					previous_node->next = ptr->next;
					ptr->next->previous = previous_node;
				} else{
					previous_node->next = NULL;
				}
				
			}else{ 
				// previous_node is header
				//current become first
				if(ptr->next){
					previous_node->next = ptr->next;
					ptr->next->previous = previous_node;
				} else{
					previous_node->next = NULL;
				}

			}
			// free (ptr);
		}
	}

	
	// print_list(0);
	return;
}

void worst_fit_dealloc(void *ptr_in) 
{

	/*local variables*/
	block_ptr *ptr = ptr_in;
	block_ptr *next_node = ptr->next;
	block_ptr *previous_node = ptr->previous;

	int free_block_size = 0;

	//printf("Var finished np\n");
	if (ptr->occupied == 0){
		printf("Double free error!\n");
	}

	// empty current 
	ptr->occupied  = 0;

	//next node available
	if(next_node != NULL){
		//next node empty
		if(next_node->occupied == 0){
			free_block_size = ptr->size + next_node->size;
			ptr->size = free_block_size;
			//next node is not the last node 
			if(next_node->next != NULL){
				//current node point to next next
				ptr->next = next_node->next;
				next_node->next->previous = next_node->previous;
			}else{
				//current become last
				ptr->next = NULL;
			}
			//not sure if you can free
			// free (ptr->next);
		}
	}

	// printf("Check next np\n");

	//previous node available
	if(ptr->previous != NULL){
		//previous node empty
		if(previous_node->occupied == 0){
			
			// update size here:
			free_block_size = ptr->size + previous_node->size;
			
			// Leo: change previous instead of ptr
			ptr->previous->size = free_block_size;
			// printf("Previous size : %d\n", ptr->previous->size);
			// printf("Previous size : %d\n", MemBlock->size);
			
			// ptr->size = free_block_size;
			
			
			//previous node is no the first node
			if(previous_node->previous != NULL){ 
				// ptr->previous = previous_node->previous;
				// previous_node->previous->next = previous_node->next;
				
				
				if(ptr->next){
					previous_node->next = ptr->next;
					ptr->next->previous = previous_node;
				} else{
					previous_node->next = NULL;
				}
				
			}else{ 
				// previous_node is header
				//current become first
				if(ptr->next){
					previous_node->next = ptr->next;
					ptr->next->previous = previous_node;
				} else{
					previous_node->next = NULL;
				}

			}
			// free (ptr);
		}
	}
	// print_list(0);
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
	printf("check frag \n");
	// To be completed by students
	block_ptr * currentBlock;
	int cnt = 0;
	if(!MemBlock){
		printf("lose the head block \n");
		return -1;
	}
	
	currentBlock = MemBlock;
	while(currentBlock){
		if(currentBlock->size < size){
			cnt++;
		}
		currentBlock = currentBlock->next;
	}
	
	return cnt;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
	block_ptr * currentBlock;
	int cnt = 0;
	if(!MemBlock){
		printf("lose the head block \n");
		return -1;
	}
	
	currentBlock = MemBlock;
	while(currentBlock){
		if(currentBlock->size < size){
			cnt++;
		}
		currentBlock = currentBlock->next;
	}
	
	return cnt;
}

//int main(int argc, char *argv[]){
//	int num = 0;
//	int algo = 0; // default algorithm to test is best fit  
//	// void *p1, *p2, *p3, *p4, *q;
//	void *p[20];
//	void *q[20];
//	block_ptr *tmp;
//	int i, size, can_fit;
//	
//	can_fit = 0;
//	
//	if (argc != 2) {
//		fprintf(stderr, "Usage: %s <0/1>. 0 for best fit and 1 for worst fit \n", argv[0]);
//		exit (1);
//	} else if (!strcmp(argv[1], "1") || !strcmp(argv[1], "0")) {
//		algo = atoi(argv[1]);
//	} else {
//		fprintf(stderr, "Invalid argument, please specify 0 or 1\n");
//		exit(1);
//	}
//	
//	if ( algo == 0 ) {
//		printf(" Start Best fit test \n");
//		best_fit_memory_init(8192);	// initizae 2KB, best fit
//		// printf("my Stucture Size: sz=%d\n", struct_size);
//		
//		for(i = 0; i < 20; i++){
//			size = 380 + 10*(i%3 + 1);
//			p[i] = best_fit_alloc(size);
//			// print_list(0);
//		}
//		
//		tmp = MemBlock;
//		// cut the memeory into many blocks with different sizes
//		// printf("Cut, change the occupied state for each Block\n");
//		while(tmp){
//			
//			tmp->occupied = 0;
//			tmp = tmp->next;
//			
//		}
//		
//		printf("Allocate more =================================================\n");
//		
//		for(i = 0; i < 30; i++){
//			size = 40 + 4*(i%4 + 1);
//			q[i] = best_fit_alloc(size);
//			print_list(0);
//			if(q[i]){
//				can_fit++;
//			}
//		}
//		
//		// if ( p != NULL ) {
//			// best_fit_dealloc(p);	
//		// }
//		num = best_fit_count_extfrag(380);
//		printf(" Best fit test \n");
//		
//	} else if ( algo == 1 ) {
//
//		printf(" Start Worst fit test \n");
//		worst_fit_memory_init(8192);	// initizae 2KB, best fit
//		// printf("my Stucture Size: sz=%d\n", struct_size);
//		
//		for(i = 0; i < 20; i++){
//			size = 380 + 10*(i%3 + 1);
//			p[i] = worst_fit_alloc(size);
//			
//		}
//		
//		tmp = MemBlock;
//		// cut the memeory into many blocks with different sizes
//		// printf("Cut, change the occupied state for each Block\n");
//		while(tmp){
//			
//			tmp->occupied = 0;
//			tmp = tmp->next;
//			
//		}
//		
//		printf("Allocate more =================================================\n");
//		
//		for(i = 0; i < 30; i++){
//			size = 40 + 4*(i%4 + 1);
//			q[i] = worst_fit_alloc(size);
//			print_list(0);
//			
//			if(q[i]){
//				can_fit++;
//			}
//		}
//		
//		// if ( p != NULL ) {
//			// best_fit_dealloc(p);	
//		// }
//		num = worst_fit_count_extfrag(380);
//		printf(" Worst fit test \n");
//	} else {
//		fprintf(stderr, "Should not reach here!\n");
//		exit(1);
//	}
//
//	printf("num = %d\n", num);
//	printf("can_fit = %d\n", can_fit);
//	return 0;
//}
//
//void print_list(int algo) { //print the linked list
//	block_ptr * current;
//	if (algo == 0) {
//		current = MemBlock;
//	}
//
//
//	while (current != NULL) {
//		printf("Address: %d | Size: %lu | Using: %d | Next: %d | \n", (size_t)current - base, current->size, current->occupied, (size_t)current->next - base);
//		current = current->next;
//	}
//	printf("\n");
//}



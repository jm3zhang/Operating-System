/**
 * @brief: ECE254 Keil ARM RL-RTX Task Management Lab starter file that calls os_tsk_get()
 * @file: main_task_exp.c
 * @date: 2015/09/13
 */
/* NOTE: This release is for students to use */

#include <LPC17xx.h>
#include "uart_polling.h" 
#include <RTL.h>
#include "../../RTX_CM3/INC/RTL_ext.h" /* extended interface header file */
#include <stdio.h>
#include <string.h>

#define NUM_FNAMES 7
#define NUM_BOXES 3
#define BOX_SIZE 4

_declare_box (mypool, BOX_SIZE, NUM_BOXES);
void *box_ptr;


struct func_info {
  void (*p)();      /* function pointer */
  char name[16];    /* name of the function */
};

extern void os_idle_demon(void);
__task void task1(void);
__task void task2(void);
__task void task3(void);
__task void task4(void);
__task void task5(void);
__task void init (void);
 
char *state2str(unsigned char state, char *str);
char *fp2name(void (*p)(), char *str);

OS_MUT g_mut_uart;
OS_TID g_tid = 255;

int  g_counter = 0;  // a global counter
char g_str[16];
char g_tsk_name[16];

struct func_info g_task_map[NUM_FNAMES] = \
{
  /* os_idle_demon function ptr to be initialized in main */
  {NULL,  "os_idle_demon"}, \
  {task1, "task1"},   \
  {task2, "task2"},   \
  {task3, "task3"},   \
  {task4, "task4"},   \
  {task5, "task5"},   \
  {init,  "init" }
};

/* TASK 1 */
__task void task1(void)
{   
    int i;
	void *return_p;
	
// 	void *ptr1;
// 	void *ptr2;
// 	void *ptr3;
// 	
// 	OS_RESULT result;
// 	ptr1 = os_mem_alloc(&mypool);
// 	ptr2 = os_mem_alloc(&mypool);
// 	ptr3 = os_mem_alloc(&mypool);
// 	printf("block one allocated; \n");
// 	printf("block two allocated; \n");	
// 	printf("block three allocated; \n");
	
	while (1) {
		//while(1){
		for(i = 0; i < 8; i++) {
			return_p = os_mem_alloc(&mypool);
			if(return_p != NULL){
				box_ptr = return_p;
				os_mut_wait(g_mut_uart, 0xFFFF);
				printf("Task 1 ----- Allocate one more memory location \n");
				os_mut_release(g_mut_uart);
			}
		}
		// WAIT_MEM

        os_dly_wait(100);
		
		os_tsk_delete_self();
	}
}


/* TASK 2 */
__task void task2(void)
{
	while(1){
        U8 i=1;
        RL_TASK_INFO task_info;
        
      
        os_mut_wait(g_mut_uart, 0xFFFF);
        printf("TID\tNAME\t\tPRIO\tSTATE   \t%%STACK\n");
        os_mut_release(g_mut_uart);
        
        for(i = 0; i <6; i++) { // this is a lazy way of doing loop.
            if (os_tsk_get(i+1, &task_info) == OS_R_OK) {
                os_mut_wait(g_mut_uart, 0xFFFF);  
                printf("%d\t%s\t\t%d\t%s\t%d%%\n", \
                       task_info.task_id, \
                       fp2name(task_info.ptask, g_tsk_name), \
                       task_info.prio, \
                       state2str(task_info.state, g_str),  \
                       task_info.stack_usage);
                os_mut_release(g_mut_uart);
            } 
        }
        
        if (os_tsk_get(0xFF, &task_info) == OS_R_OK) {
            os_mut_wait(g_mut_uart, 0xFFFF);  
            printf("%d\t%s\t\t%d\t%s\t%d%%\n", \
                   task_info.task_id, \
                   fp2name(task_info.ptask, g_tsk_name), \
                   task_info.prio, \
                   state2str(task_info.state, g_str),  \
                   task_info.stack_usage);
            os_mut_release(g_mut_uart);
        } 
        os_dly_wait(5);
    }
}

/* Task 3: Free memory */
__task void task3(void)
{
      while(1){
	  void *ptr;
	  os_mut_wait(g_mut_uart, 0xFFFF);
		printf("Task 3 trying to add one more task, but since pool is already full, it will become WAIT_MEM \n");
	  os_mut_release(g_mut_uart);
		 ptr = os_mem_alloc(&mypool);
			if(ptr == NULL){
				//Memory pool is full; state of task 1 should be WAIT_MEM 
				
				os_mut_wait(g_mut_uart, 0xFFFF);
				printf("memory pool is full, task 3 cannot add. \n");
				os_mut_release(g_mut_uart);
				//printf("memory pool is full, new added task blocked and goes to waiting list.");
				
			} else {
				box_ptr = ptr;		
				os_mut_wait(g_mut_uart, 0xFFFF);
				printf("Task 3 ----- get a location \n");
				os_mut_release(g_mut_uart);
			}
		 os_dly_wait(100);
		 os_tsk_delete_self();
    }
}

 __task void task4(void)
{

	
	while(1){
		
		OS_RESULT result;
		os_dly_wait(11);
		result = os_mem_free(mypool, box_ptr);
		if(result == OS_R_OK){
			os_mut_wait(g_mut_uart, 0xFFFF);
			printf("Task 4 -----------  memory free succeed! \n");
			os_mut_release(g_mut_uart);
		} else {
// 			os_mut_wait(g_mut_uart, 0xFFFF);
// 			printf("something wrong \n");
// 			os_mut_release(g_mut_uart);
		}
		

    }
}


__task void task5(void)
 {
     while(1){
		void *ptr;
		
		os_dly_wait(20);
		os_mut_wait(g_mut_uart, 0xFFFF);
		printf("Task 5 in ------ high priority, will be run soon \n");
		os_mut_release(g_mut_uart);
		ptr = os_mem_alloc(&mypool);
			if(ptr == NULL){
				//Memory pool is full; state of task 1 should be WAIT_MEM 
				
				//printf("memory pool is full, new added task blocked and goes to waiting list.");
				
			} else {
				//box_ptr = ptr;		
				os_mut_wait(g_mut_uart, 0xFFFF);
				printf("Task 5 get a location. Should be earlier than 3, which entered earilier, but lower priority.\n");
				os_mut_release(g_mut_uart);
			}
			
			os_dly_wait (20);
			// die
			os_tsk_delete_self();
     }
 }

/*--------------------------- init ------------------------------------*/
/* initialize system resources and create other tasks                  */
/*---------------------------------------------------------------------*/
__task void init(void)
{

	_init_box (&mypool, sizeof mypool, 3);
		
	os_mut_init(&g_mut_uart);
  
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: TID = %d\n", os_tsk_self());
	os_mut_release(g_mut_uart);
  
	g_tid = os_tsk_create(task1, 1);  /* task 1 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task1 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
  
	g_tid = os_tsk_create(task2, 1);  /* task 2 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task2 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
  
   g_tid = os_tsk_create(task3, 1);  /* task 2 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task3 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
  
   g_tid = os_tsk_create(task4, 1);  /* task 2 at priority 1 */
 	os_mut_wait(g_mut_uart, 0xFFFF);
 	printf("init: created task4 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
   
   g_tid = os_tsk_create(task5, 3);  /* task 2 at priority 1 */
 	os_mut_wait(g_mut_uart, 0xFFFF);
 	printf("init: created task5 with TID %d\n", g_tid);
 	os_mut_release(g_mut_uart);
  
	os_tsk_delete_self();     /* task MUST delete itself before exiting */
}

/**
 * @brief: convert state numerical value to string represenation      
 * @param: state numerical value (macro) of the task state
 * @param: str   buffer to save the string representation, 
 *               buffer to be allocated by the caller
 * @return:the string starting address
 */
char *state2str(unsigned char state, char *str)
{
	switch (state) {
	case INACTIVE:
		strcpy(str, "INACTIVE");
		break;
	case READY:
		strcpy(str, "READY   ");
		break;
	case RUNNING:
		strcpy(str, "RUNNING ");
		break;
	case WAIT_DLY:
		strcpy(str, "WAIT_DLY");
		break;
	case WAIT_ITV:
		strcpy(str, "WAIT_ITV");
		break;
	case WAIT_OR:
		strcpy(str, "WAIT_OR");
		break;
	case WAIT_AND:
		strcpy(str, "WAIT_AND");
		break;
	case WAIT_SEM:
		strcpy(str, "WAIT_SEM");
		break;
	case WAIT_MBX:
		strcpy(str, "WAIT_MBX");
		break;
	case WAIT_MUT:
		strcpy(str, "WAIT_MUT");
		break;
    case WAIT_MEM:
		strcpy(str, "WAIT_MEM");
		break;
	default:
		strcpy(str, "UNKNOWN");    
	}
	return str;
}

/** 
 * @brief: get function name by function pointer
 * @param: p the entry point of a function (i.e. function pointer)
 * @param: str the buffer to return the function name
 * @return: the function name string starting address
 */
char *fp2name(void (*p)(), char *str)
{
	int i;
	unsigned char is_found = 0;
  
	for ( i = 0; i < NUM_FNAMES; i++) {
		if (g_task_map[i].p == p) {
			str = strcpy(str, g_task_map[i].name);  
			is_found = 1;
			break;
		}
	}
	if (is_found == 0) {
		strcpy(str, "ghost");
	}
	return str;
}

int main(void)
{
	SystemInit();         /* initialize the LPC17xx MCU */
	uart0_init();         /* initilize the first UART */
	

  
	/* fill the fname map with os_idle_demon entry point */
	g_task_map[0].p = os_idle_demon;
  
	printf("Calling os_sys_init()...\n");
	os_sys_init(init);    /* initilize the OS and start the first task */
}

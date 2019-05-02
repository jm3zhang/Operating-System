/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_MEMBOX_ext.C
 *      Purpose: Interface functions for blocking 
 *               fixed memory block management system
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code is extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *      Includes
 *---------------------------------------------------------------------------*/
 
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_MemBox.h"
#include "rt_HAL_CM.h"
#include "rt_Task.h"       /* added in ECE254 lab keil_proc */ 
#include "rt_MemBox_ext.h" /* added in ECE254 lab keil_proc */  
//#include "RTL_ext.h"

//...
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_Robin.h"

/* ECE254 Lab Comment: You may need to include more header files */

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

struct OS_XCB queue;

/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/

/*==========================================================================*/
/*  The following are added for ECE254 Lab Task Management Assignmet       */
/*==========================================================================*/

/*---------------- rt_alloc_box_s, task blocks when out of memory-----------*/

/**  
   @brief: Blocking memory allocation routine.
 */
void *rt_alloc_box_s (void *p_mpool) {
	
	/* Add your own code here. Change the following line accordingly */
	void *mp_ptr;
	int task_id;
	P_TCB task_ptr;
	mp_ptr = rt_alloc_box(p_mpool);
	if (mp_ptr == NULL) {
		// The memory pool is full; now get the current taskID
		task_id = rt_tsk_self();
		
		task_ptr = os_active_TCB[task_id - 1];
		
		// put it into the waiting list
		rt_put_prio(&queue, task_ptr);
		rt_block(0xffff, WAIT_MEM);
		return NULL;
		
	}
	else 
		return mp_ptr;
	
}


/*----------- rt_free_box_s, pair with _s memory allocators ----------------*/
/**
 * @brief: free memory pointed by ptr, it will unblock a task that is waiting
 *         for memory.
 * @return: OS_R_OK on success and OS_R_NOK if ptr does not belong to gp_mpool 
 */
OS_RESULT rt_free_box_s (void *p_mpool, void *box) {
	/* Add your own code here. Change the following line accordingly */
	int free_mem; 
	P_TCB wait_task;
	free_mem = rt_free_box(p_mpool, box);
	if (free_mem == 0) {
		if (&queue.p_lnk != NULL) {
			wait_task = rt_get_first(&queue);
			wait_task->ret_val = (U32)box;
			rt_dispatch(wait_task);
			return (OS_R_OK);
		}
	}
	return (OS_R_NOK);
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

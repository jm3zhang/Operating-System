/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_TASK_ext.C
 *      Purpose: Interface functions for extended task management  
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/
/*--------------------------- rt_tsk_count_get ------------------------------*/
/* added in ECE254 lab keil_rtx */
int rt_tsk_count_get (void) {
	/* Add your own code here. Change the following line accordingly */
    int count, i;
    P_TCB task;
    
    for (i = 0; i < os_maxtaskrun; i++){
      task = os_active_TCB[i];
      if (os_active_TCB[i] != NULL &&  task->state != INACTIVE){
        count++;
      }
    }   
    
    if (os_idle_TCB.state != INACTIVE)
    {
        count++;
    }
    
    // plus one idle task
	return count;
}

/*--------------------------- rt_tsk_get ------------------------------------*/
/* added in ECE254 lab keil_proc */
OS_RESULT rt_tsk_get (OS_TID task_id, RL_TASK_INFO *p_task_info) {
	/* Add your own code here. Change the following lines accordingly */
  
    P_TCB task;
    int i;
    for (i = 0; i < os_maxtaskrun; i++) {
        task = os_active_TCB[i];
        if (os_active_TCB[i] != NULL &&  task->task_id == task_id) {
            //if inactive nothing need to be recorded
            if (task->state == INACTIVE) {
                return OS_R_NOK;
            }
            
            p_task_info->task_id = task->task_id;
            p_task_info->state = task->state;
            p_task_info->prio = task->prio;
            p_task_info->ptask = task->ptask;
            
            // ================ this is referred from LEARN group discussion =======
            if (task->state != 2){
            // if the task is not running
                U32 sizeInAddr = (U16)os_stackinfo;
                p_task_info->stack_usage = (U8)(((U32)(task->stack) + sizeInAddr - (U32)(task->tsk_stack))*100/sizeInAddr);
            }
            else {
            // if the task is running
                U32 sizeInAddr = (U16)os_stackinfo;
                p_task_info->stack_usage = (U8)(((U32)(task->stack) + sizeInAddr - (U32)rt_get_PSP())*100/sizeInAddr);
            }
            // =====================================================================
            return OS_R_OK;
        }
    }
    
    return OS_R_NOK;

}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

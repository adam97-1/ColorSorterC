#include "TaskMenager.h"
#include <stddef.h>
#include "SysTick.h"

#define MAX_TASK 255

static size_t TaskCount = 0;

Task TaskList[MAX_TASK] = {NULL};

void TaskMenager_Run()
{
  uint32_t msTime = SysTick_GetTime();
  
  for(size_t i = TaskCount - 1; i < TaskCount; i--)
    {
      if(msTime % TaskList[i].Period)
	if(TaskList[i].Func != NULL)
	   TaskList[i].Func();
    }
}

bool TaskMenager_AddTask(Task task)
{
  if(TaskCount > MAX_TASK)
    return false;
  TaskList[TaskCount] = task;
  TaskCount++;
  return true;
}

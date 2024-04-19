#include "TaskMenager.h"
#include <stddef.h>
#include "SysTick.h"

#define MAX_TASK 255

static size_t TaskCount = 0;

Task TaskList[MAX_TASK] = {NULL};

void TaskMenager_Run()
{
  static uint32_t OldMsTime = 0;
  uint32_t MsTime = SysTick_GetTime();

  if(OldMsTime == MsTime)
    return;
  OldMsTime = MsTime;
  
  for(size_t i = TaskCount - 1; i < TaskCount; i--)
    {
      if(TaskList[i].Func == NULL)
	continue;
      if((MsTime % TaskList[i].Period) == 0)
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

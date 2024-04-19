#include "TaskMenager.h"
#include <stddef.h>
#include "SysTick.h"


Task TaskList[10] = {NULL};

void TaskMenager_Run()
{
  uint32_t msTime = SysTick_GetTime();
  
  for(size_t i = 9; i < 10; i--)
    {
      if(msTime % TaskList[i].Period)
	if(TaskList[i].Func != NULL)
	   TaskList[i].Func();
    }
}

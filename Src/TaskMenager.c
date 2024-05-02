#include "TaskMenager.h"
#include <stddef.h>
#include "SysTick.h"
#include "PinTester.h"
#include "ClearStack.h"

#define MAX_TASK 255

static size_t m_taskCount = 0;

Task m_taskList[MAX_TASK] = { NULL };

void TaskMenager_Init()
{
	ADD_TO_CLEAR();
	PinTeste_Init();
	CLEAR_STACK();
}

void TaskMenager_Run()
{
	ADD_TO_CLEAR();
	static uint32_t oldMsTime = 0;
	uint32_t msTime = SysTick_GetTime();
	CLEAR_STACK();

//	if (oldMsTime == msTime)
//		return;
	oldMsTime = msTime;

	PinTeste_SetHigh();
	CLEAR_STACK();

	for (size_t i = 0; i < m_taskCount; i++)
	{
		if (m_taskList[i].Func == NULL)
			continue;
		if ((msTime % m_taskList[i].Period) == 0)
		{
			m_taskList[i].Func();
			CLEAR_STACK();
		}
	}

	PinTeste_SetLow();
	CLEAR_STACK();
}

bool TaskMenager_AddTask(Task task)
{
	ADD_TO_CLEAR();
	if (m_taskCount > MAX_TASK)
		return false;
	m_taskList[m_taskCount] = task;
	m_taskCount++;
	return true;
}

#include "TaskMenager.h"
#include <stddef.h>
#include "SysTick.h"
#include "PinTester.h"

#define MAX_TASK 255

static size_t m_taskCount = 0;

Task m_taskList[MAX_TASK] = { NULL };

void TaskMenager_Init()
{
	PinTeste_Init();
}

void TaskMenager_Run()
{
	static uint32_t oldMsTime = 0;
	uint32_t msTime = SysTick_GetTime();

	if (oldMsTime == msTime)
		return;
	oldMsTime = msTime;

	PinTeste_SetHigh();

	for (size_t i = 0; i < m_taskCount; i++)
	{
		if (m_taskList[i].Func == NULL)
			continue;
		if ((msTime % m_taskList[i].Period) == 0)
			m_taskList[i].Func();
	}

	PinTeste_SetLow();
}

bool TaskMenager_AddTask(Task task)
{
	if (m_taskCount > MAX_TASK)
		return false;
	m_taskList[m_taskCount] = task;
	m_taskCount++;
	return true;
}

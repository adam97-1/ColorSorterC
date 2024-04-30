#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  void (*Func)(void);
  uint32_t Period;
  uint8_t Prioryty;
}Task;

void TaskMenager_Init();
void TaskMenager_Run();
bool TaskMenager_AddTask(Task task);

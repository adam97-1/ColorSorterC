#pragma once

#include <stdint.h>

typedef struct
{
  void (*Func)(void);
  uint32_t Period;
  uint8_t Prioryty;
}Task;

extern Task TaskList[];
void TaskMenager_Run();



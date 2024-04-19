#pragma once

#include <stdbool.h>

void IrDetectorCol_Init();
void IrDetectorCol_SetRisingEngeStateFunc(void (*func)(void));
void IrDetectorCol_SetFallingEngeStateFunc(void (*func)(void));
bool IrDetectorCol_GetState();

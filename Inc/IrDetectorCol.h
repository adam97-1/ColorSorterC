#pragma once

#include <stdbool.h>

void IrDetectorCol_Init();
void IrDetectorCol_SetRisingEdgeStateFunc(void (*func)(void));
void IrDetectorCol_SetFallingEdgeStateFunc(void (*func)(void));
bool IrDetectorCol_GetState();

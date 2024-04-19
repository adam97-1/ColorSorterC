#pragma once

#include <stdbool.h>

void IrDetectorSel_Init();
void IrDetectorSel_SetRisingEdgeStateFunc(void (*func)(void));
void IrDetectorSel_SetFallingEdgeStateFunc(void (*func)(void));
bool IrDetectorSel_GetState();

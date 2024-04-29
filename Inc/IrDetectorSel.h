#pragma once

#include <stdbool.h>
#include <stdint.h>

void IrDetectorSel_Init(uint32_t msPeriod);
void IrDetectorSel_SetRisingEdgeStateFunc(void (*func)(void));
void IrDetectorSel_SetFallingEdgeStateFunc(void (*func)(void));
bool IrDetectorSel_GetState();

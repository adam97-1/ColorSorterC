#pragma once

#include <stdbool.h>
#include <stdint.h>

void IrDetectorCol_Init(uint32_t msPeriod);
void IrDetectorCol_SetRisingEdgeStateFunc(void (*func)(void));
void IrDetectorCol_SetFallingEdgeStateFunc(void (*func)(void));
bool IrDetectorCol_GetState();

#pragma once

#include <string.h>
#include <stdint.h>

extern uint8_t* m_endStacks[512];
extern uint16_t m_intexStack;

#define CLEAR_STACK() {\
	if(m_intexStack != 0)\
	{ \
		size_t stos_rozmiar = (void*)m_endStacks[m_intexStack - 1] - (void*)m_endStacks[m_intexStack]; \
		memset(m_endStacks[m_intexStack], 0xAA, stos_rozmiar); \
		m_endStacks[m_intexStack] = 0; \
		m_intexStack--; \
	} \
}

#define ADD_TO_CLEAR() {\
    m_intexStack++; \
    uint8_t *sp; \
    __asm__("mov %0, sp" : "=r"(sp)); \
	m_endStacks[m_intexStack] = sp; \
}

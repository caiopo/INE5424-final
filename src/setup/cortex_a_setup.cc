// EPOS Cortex-A SETUP

#include <system/config.h>

extern "C" { void _vector_table() __attribute__ ((used, naked, section(".init"))); }

// Interrupt Vector Table
void _vector_table()
{
    ASM("\t\n\
    _vector_table: \t\n\
        b   _reset                                                           \t\n\
        b   _undefined_instruction                                           \t\n\
        b   _software_interrupt                                              \t\n\
        b   _prefetch_abort                                                  \t\n\
        b   _data_abort                                                      \t\n\
        nop                           // Reserved                            \t\n\
        b   _int_entry                                                       \t\n\
        b   _fiq                                                             \t\n\
                                                                             \t\n\
    _reset:                                                                  \t\n\
        mrc p15, 0, r2, c0, c0, 5                                            \t\n\
		ands r2, r2, #0x03                                                   \t\n\
		mov r2, r2, LSL #14                                                  \t\n\
		ldr r1, =__boot_stack__												 \t\n\
		sub r1, r1, r2														 \t\n\
		mov sp, r1															 \t\n\
                                                                             \t\n\
        b _start                                                             \t\n\
        ");
}
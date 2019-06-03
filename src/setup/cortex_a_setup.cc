// EPOS Cortex-A SETUP

#include <system/config.h>

extern "C" { void _vector_table() __attribute__ ((used, naked, section(".init"))); }
// extern "C" { void _startup_mmu() __attribute__ ((used, ?, section(".init"))); }
// extern "C" { void _startup_cache() __attribute__ ((used, ?, section(".init"))); }


// Interrupt Vector Table
void _vector_table()
{
    ASM("\t\n\
        b   _start                                                           \t\n\
        b   _undefined_instruction                                           \t\n\
        b   _software_interrupt                                              \t\n\
        b   _prefetch_abort                                                  \t\n\
        b   _data_abort                                                      \t\n\
        nop                           // Reserved                            \t\n\
        b   _int_entry                                                       \t\n\
        b   _fiq                                                             \t\n\
        .word 0xCAFECAFE                                                     \t\n\
        ");
}

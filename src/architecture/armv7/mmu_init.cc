// EPOS ARMv7 MMU Mediator Initialization

#include <architecture/mmu.h>

extern "C" void * __data_start;
extern "C" void * _edata;
extern "C" void * __bss_start;
extern "C" void * _end;

void _startup_mmu();

__BEGIN_SYS

void MMU::init()
{
    db<Init, MMU>(TRC) << "MMU::init()" << endl;

    db<Init, MMU>(INF) << "MMU::init::dat.b=" << &__data_start << ",dat.e=" << &_edata << ",bss.b=" << &__bss_start << ",bss.e=" << &_end << endl;

    _startup_mmu();

    // For machines that do not feature a real MMU, frame size = 1 byte
    // TODO: The stack left at the top of the memory for INIT is freed at Thread::init()
    free(&_end, pages(Memory_Map::SYS_STACK - reinterpret_cast<unsigned int>(&_end)));
}

__END_SYS

void _startup_mmu()
{
    ASM("\t\n\
    mmu_start:                                                                                \t\n\
        // Disable MMU                                                                        \t\n\
        MRC p15, 0, r1, c1, c0, 0	// Atribui-se ao R1 o valor do registrador 1 do           \t\n\
                                    // coprocessor 15                                         \t\n\
        BIC r1, r1, #0x1           	// Atribui-se ao bit 0 em R1 o valor 0, para              \t\n\
                                    // desligar a MMU                                         \t\n\
        MCR p15, 0, r1, c1, c0, 0	// Escreve-se no reg 1 do coprocessor 15                  \t\n\
                                    // o que ha em R1, desabilitando a MMU                    \t\n\
                                                                                              \t\n\
        // Disable L1 Caches                                                                  \t\n\
        MRC p15, 0, r1, c1, c0, 0	// Read Control Register configuration data               \t\n\
        BIC r1, r1, #(0x1 << 12)	// Disable I Cache                                        \t\n\
        BIC r1, r1, #(0x1 << 2)		// Disable D Cache                                        \t\n\
        MCR p15, 0, r1, c1, c0, 0	// Write Control Register configuration data              \t\n\
                                                                                              \t\n\
        // Invalidate L1 Caches                                                               \t\n\
        // Invalidate Instruction cache                                                       \t\n\
        MOV r1, #0                                                                            \t\n\
        MCR p15, 0, r1, c7, c5, 0                                                             \t\n\
                                                                                              \t\n\
        // Invalidate Data cache                                                              \t\n\
        // to make the code general purpose, we calculate the                                 \t\n\
        // cache size first and loop through each set + way                                   \t\n\
        MRC p15, 1, r0, c0, c0, 0  	// Read Cache Size ID                                     \t\n\
        LDR r3, =#0x1ff                                                                       \t\n\
        AND r0, r3, r0, LSR #13		// r0 = no. of sets - 1                                   \t\n\
        MOV r1, #0			        // r1 = way counter way_loop                              \t\n\
        way_loop:                                                                             \t\n\
        MOV r3, #0			        // r3 = set counter set_loop                              \t\n\
        set_loop:                                                                             \t\n\
        MOV r2, r1, LSL #30                                                                   \t\n\
        ORR r2, r3, LSL #5 		    // r2 = set/way cache operation format                    \t\n\
        MCR p15, 0, r2, c7, c6, 2 	// Invalidate line described by r2                        \t\n\
        ADD r3, r3, #1 			    // Increment set counter                                  \t\n\
        CMP r0, r3 			        // Last set reached yet?                                  \t\n\
        BGT set_loop 			    // if not, iterate set_loop                               \t\n\
        ADD r1, r1, #1 			    // else, next                                             \t\n\
        CMP r1, #4 			        // Last way reached yet?                                  \t\n\
        BNE way_loop 			    // if not, iterate way_loop                               \t\n\
                                                                                              \t\n\
        // Invalidate TLB                                                                     \t\n\
        MCR p15, 0, r1, c8, c7, 0                                                             \t\n\
                                                                                              \t\n\
        // Aqui é criada uma L1 translation table na RAM que divide                           \t\n\
        // todo o espaço de endereçamento de 4GB em seções de 1 MB,                           \t\n\
        // todas com Full Access e Strongly Ordered                                           \t\n\
        LDR r0, =0xDE2			        // Atribui-se ao R0 parte do descriptor               \t\n\
        LDR r1, =0xFA0000       	    // Atribui-se ao R1 endereço base                     \t\n\
                                        // da L1 tranlastion table                            \t\n\
        LDR r3, = 4095         		    // R3 se torna o contador para o loop                 \t\n\
                                                                                              \t\n\
        write_pte:                  	// Label do loop para escrita das                     \t\n\
                                        // page table entry (PTE) da translation table        \t\n\
            ORR r2, r0, r3, LSL #20     // Atribui-se ao R2 OR entre o endereço               \t\n\
                                        // e os bits padrão da PTE                            \t\n\
        STR r2, [r1, r3, LSL #2]    	// Escreve-se a PTE na translation table              \t\n\
                                        // (endereço de escrita é o ttb_address somado        \t\n\
                                        // com contador e multiplicado por 4)                 \t\n\
        SUB r3, r3, #1              	// Decrementa-se contador do loop                     \t\n\
        CMP r3, #-1                 	// Faz-se a comparação para verificar                 \t\n\
                                        // se loop acabou                                     \t\n\
        BNE write_pte               	// Caso o loop não tenha acabado,                     \t\n\
                                        // escreve mais uma pte                               \t\n\
                                                                                              \t\n\
        // Faz-se a primeira entrada da translation table                                     \t\n\
        // cacheable, normal, write-back, write allocate                                      \t\n\
        BIC r0, r0, #0xC		    // Limpa-se CB bits                                       \t\n\
        ORR r0, r0, #0X4 		    // Write-back, write allocate                             \t\n\
        BIC r0, r0, #0x7000 		// Limpa-se TEX bits                                      \t\n\
        ORR r0, r0, #0x5000 		// Faz-se TEX write-back e write allocate                 \t\n\
        ORR r0, r0, #0x10000 		// Torna compartilhável                                   \t\n\
        STR r0, [r1]			    // Escreve-se na primeira entrada                         \t\n\
                                                                                              \t\n\
        // Inicializa a MMU                                                                   \t\n\
        MOV r1,#0x0                                                                           \t\n\
            MCR p15, 0, r1, c2, c0, 2   // Escrita do Translation Table Base Control Register \t\n\
        LDR r1, =0xFA0000   		    // Atribui-se ao R1 endereço base                     \t\n\
                                        // da L1 tranlastion table                            \t\n\
        MCR p15, 0, r1, c2, c0, 0	    // Escreve-se no reg 1 do coprocessor 15 o que ha     \t\n\
                                        // em r1 (endereco base da tranlastion table)         \t\n\
                                                                                              \t\n\
        // In this simple example, we don't use TRE or Normal Memory Remap Register.          \t\n\
        // Set all Domains to Client                                                          \t\n\
        LDR r1, =0x55555555                                                                   \t\n\
        MCR p15, 0, r1, c3, c0, 0       // Write Domain Access Control Register               \t\n\
                                                                                              \t\n\
        // Enable MMU                                                                         \t\n\
        MRC p15, 0, r1, c1, c0, 0	    // Atribui-se ao R1 o valor do registrador 1 do       \t\n\
                                        // coprocessor 15                                     \t\n\
        ORR r1, r1, #0x1           	    // Atribui-se ao bit 0 em R1 o valor 1, para          \t\n\
                                        // ligar a MMU                                        \t\n\
        MCR p15, 0, r1, c1, c0, 0	    // Escreve-se no reg 1 do coprocessor 15              \t\n\
                                        // o que há em R1, habilitando a MMU                  \t\n\
        ");
}


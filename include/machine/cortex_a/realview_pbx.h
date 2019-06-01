// EPOS LM3S811 (ARM Cortex-M3) MCU Mediator Declarations

#ifndef __realview_pbx_h
#define __realview_pbx_h

#include <architecture/cpu.h>
#include <architecture/tsc.h>
#include <machine/rtc.h>
#include <system.h>

__BEGIN_SYS

class RealView_PBX
{
    friend class TSC;

protected:
    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;

public:
    static const unsigned int IRQS = 96;
    static const unsigned int TIMERS = 4;
    static const unsigned int UARTS = 4;
    static const unsigned int GPIO_PORTS = 3;
    static const bool supports_gpio_power_up = false;

    // Base addresses for memory-mapped I/O devices
    enum {
        // http://infocenter.arm.com/help/topic/com.arm.doc.dui0411d/DUI0411D_realview_platform_baseboard_ug.pdf
        // https://wiki.osdev.org/User:Pancakes/arm_qemu_realview-pb-a
        
        SCR_BASE = 0x10000000,
        SYSTEM_CONTROLLER_BASE = 0x10001000,

        UART0_BASE = 0x10009000,
        UART1_BASE = 0x1000a000,
        UART2_BASE = 0x1000b000,
        UART3_BASE = 0x1000c000,

        WDT0_BASE = 0x10010000,

        GPIOA_BASE = 0x10013000,
        GPIOB_BASE = 0x10014000,
        GPIOC_BASE = 0x10015000,

        IC0_BASE = 0x1e000000,
        IC1_BASE = 0x1e001000,
        IC2_BASE = 0x1e002000,
        IC3_BASE = 0x1e003000,

        // ??
        TIMER0_BASE = 0x10011000,
        TIMER1_BASE = 0x10011020,
        TIMER2_BASE = 0x10012000,
        TIMER3_BASE = 0x10012020,

        GIC_BASE    = 0x1f000000,

        /* 0x10000000 System registers.  */
        /* 0x10001000 System controller.  */
        /* 0x10002000 Two-Wire Serial Bus.  */
        /* 0x10003000 Reserved.  */
        /* 0x10004000 AACI.  */
        /* 0x10005000 MCI.  */
        /* 0x10006000 KMI0.  */
        /* 0x10007000 KMI1.  */
        /* 0x10009000 UART0.  */
        /* 0x1000a000 UART1.  */
        /* 0x1000b000 UART2.  */
        /* 0x1000c000 UART3.  */
        /* 0x1000d000 SSPI.  */
        /* 0x1000e000 SCI.  */
        /* 0x1000f000 Reserved.  */
        /* 0x10010000 Watchdog.  */
        /* 0x10011000 Timer 0+1.  */
        /* 0x10012000 Timer 2+3.  */
        /* 0x10013000 GPIO 0.  */
        /* 0x10014000 GPIO 1.  */
        /* 0x10015000 GPIO 2.  */
        /* 0x10002000 Two-Wire Serial Bus - DVI. (PB) */
        /* 0x10017000 RTC.  */
        /* 0x10018000 DMC.  */
        /* 0x10019000 PCI controller config.  */
        /* 0x10020000 CLCD.  */
        /* 0x10030000 DMA Controller.  */
        /* 0x10080000 SMC.  */
        /* 0x1e000000 GIC1. (PB) */
        /* 0x1e001000 GIC2. (PB) */
        /* 0x1e002000 GIC3. (PB) */
        /* 0x1e003000 GIC4. (PB) */
        /* 0x40000000 NOR flash.  */
        /* 0x44000000 DoC flash.  */
        /* 0x48000000 SRAM.  */
        /* 0x4c000000 Configuration flash.  */
        /* 0x4e000000 Ethernet.  */
        /* 0x4f000000 USB.  */
        /* 0x50000000 PISMO.  */
        /* 0x54000000 PISMO.  */
        /* 0x58000000 PISMO.  */
        /* 0x5c000000 PISMO.  */
        /* 0x60000000 PCI.  */
        /* 0x60000000 PCI Self Config.  */
        /* 0x61000000 PCI Config.  */
        /* 0x62000000 PCI IO.  */
        /* 0x63000000 PCI mem 0.  */
        /* 0x64000000 PCI mem 1.  */
        /* 0x68000000 PCI mem 2.  */
    };

    // System Control Registers offsets to SCR_BASE
    enum {                              // Description                                          Type    Value after reset
        ID              = 0x000, // System Identity
        SW              = 0x004,
        LED             = 0x008,
        OSC0            = 0x00C,
        OSC1            = 0x010,
        OSC2            = 0x014,
        OSC3            = 0x018,
        OSC4            = 0x01C,
        LOCK            = 0x020,
        HUNDRED_HZ      = 0x024,
        CONFIGDATA      = 0x028,
        FLAGS           = 0x030,
        FLAGSSET        = 0x030,
        FLAGSCLR        = 0x034,
        NVFLAGS         = 0x038,
        NVFLAGSSET      = 0x038,
        NVFLAGSCCLR     = 0x03C,
        PCICTL          = 0x044,
        MCI             = 0x048,
        FLASH           = 0x04C,
        CLCD            = 0x050,
        CLCDSER         = 0x054,
        BOOTCS          = 0x058,
        TWENTY_FOUR_MHZ = 0x05C,
        MISC            = 0x060,
        DMAPSR0         = 0x064,
        DMAPSR1         = 0x068,
        DMAPSR2         = 0x06C,
        IOSEL           = 0x070,
        PLDCTL          = 0x074,
        BUSID           = 0x080,
        PROCID          = 0x084,
        OSCRESET        = 0x08C,
        VOLTAGE         = 0x0A0,
        TEST_OCS        = 0x0D4,
    };

protected:
    RealView_PBX() {}

    static void reboot() {
        // Reg32 val = scs(AIRCR) & (~((-1u / VECTKEY) * VECTKEY));
        // val |= 0x05fa * VECTKEY | SYSRESREQ;
        // scs(AIRCR) = val;
    }

    static void delay(const RTC::Microsecond & time) {
        // assert(Traits<TSC>::enabled);
        // unsigned long long ts = static_cast<unsigned long long>(time) * TSC::frequency() / 1000000;
        // tsc(GPTMTAILR) = ts;
        // tsc(GPTMTAPR) = ts >> 32;
        // tsc(GPTMCTL) |= TAEN;
        // while(!(tsc(GPTMRIS) & TATO_INT));
        // tsc(GPTMCTL) &= ~TAEN;
        // tsc(GPTMICR) |= TATO_INT;
    }

    static const UUID & uuid() { return System::info()->bm.uuid; } // TODO: System_Info is not populated in this machine

    // Device enabling
    static void enable_uart(unsigned int unit) {
        // assert(unit < UARTS);
        // power_uart(unit, FULL);
        // gpioa(AFSEL) |= 3 << (unit * 2);                // Pins A[1:0] are multiplexed between GPIO and UART 0. Select UART.
        // gpioa(DEN) |= 3 << (unit * 2);                  // Enable digital I/O on Pins A[1:0]
    }
    static void enable_usb(unsigned int unit) {}

    // Power Management
    static void power_uart(unsigned int unit, const Power_Mode & mode) {
        // assert(unit < UARTS);
        // switch(mode) {
        // case ENROLL:
        // 	break;
        // case DISMISS:
        // 	break;
        // case SAME:
        // 	break;
        // case FULL:
        // 	break;
        // case LIGHT:
        // 	break;
        // case SLEEP:
        //     scr(RCGC1) |= 1 << unit;                   // Activate UART "unit" clock
        //     scr(RCGC2) |= 1 << unit;                   // Activate port "unit" clock
        //     break;
        // case OFF:
        //     scr(RCGC1) &= ~(1 << unit);                // Deactivate UART "unit" clock
        //     scr(RCGC2) &= ~(1 << unit);                // Deactivate port "unit" clock
        //     break;
        // }
    }

    static void power_user_timer(unsigned int unit, const Power_Mode & mode) {
        // assert(unit < TIMERS);
        // switch(mode) {
        // case ENROLL:
        // 	break;
        // case DISMISS:
        // 	break;
        // case SAME:
        // 	break;
        // case FULL:
        // 	break;
        // case LIGHT:
        // 	break;
        // case SLEEP:
        //     scr(RCGC1) |= 1 << (unit + 16);             // Activate GPTM "unit" clock
        //     break;
        // case OFF:
        //     scr(RCGC1) &= ~(1 << (unit + 16));          // Deactivate GPTM "unit" clock
        //     break;
        // }
    }

    static void power_usb(unsigned int unit, const Power_Mode & mode) {}


    // GPIO
    static void gpio_init() {}
    static void power_gpio(unsigned int unit, const Power_Mode & mode) {
        // assert(unit < UARTS);
        // switch(mode) {
        // case ENROLL:
        // 	break;
        // case DISMISS:
        // 	break;
        // case SAME:
        // 	break;
        // case FULL:
        // 	break;
        // case LIGHT:
        // 	break;
        // case SLEEP:
        //     scr(RCGC2) |= 1 << unit;                   // Activate port "unit" clock
        //     break;
        // case OFF:
        //     scr(RCGC2) &= ~(1 << unit);                // Deactivate port "unit" clock
        //     break;
        // }
    }
    void gpio_pull_up(unsigned int port, unsigned int pin) { 
        // gpio(port, PUR) &= 1 << pin;
    }
    void gpio_pull_down(unsigned int port, unsigned int pin) {
        // gpio(port, PDR) &= 1 << pin;
    }
    void gpio_floating(unsigned int port, unsigned int pin) {
        // gpio(port, ODR) &= 1 << pin;
    }

    // ADC (not implemented for this model)
    static void adc_config(unsigned char channel);

    // PWM (not implemented for this model)
    static void pwm_config(unsigned int timer, char gpio_port, unsigned int gpio_pin);

    // IEEE 802.15.4 (not present in this model)
    static void power_ieee802_15_4(const Power_Mode & mode);

public:
    // static volatile Reg32 & scr(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(SCR_BASE)[o / sizeof(Reg32)]; }
    // static volatile Reg32 & scs(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(IC0_BASE)[o / sizeof(Reg32)]; }

    static volatile Reg32 & systick(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(TIMER3_BASE)[o / sizeof(Reg32)]; }
    static volatile Reg32 & tsc(unsigned int o)     { return reinterpret_cast<volatile Reg32 *>(TIMER1_BASE)[o / sizeof(Reg32)]; }
    static volatile Reg32 & timer0(unsigned int o)  { return reinterpret_cast<volatile Reg32 *>(TIMER0_BASE)[o / sizeof(Reg32)]; }
    static volatile Reg32 & timer1(unsigned int o)  { return reinterpret_cast<volatile Reg32 *>(TIMER2_BASE)[o / sizeof(Reg32)]; }

protected:
    static void pre_init();
    static void init();
};

typedef RealView_PBX Machine_Model;

__END_SYS

#endif

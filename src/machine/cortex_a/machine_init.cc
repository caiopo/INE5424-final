// EPOS Cortex-A Mediator Initialization

#include <machine.h>

__BEGIN_SYS

void Machine::pre_init(System_Info * si)
{
    if (Traits<IC>::enabled)
        IC::init();

    Display::init();

    Machine_Model::pre_init();

    if (Traits<System>::multicore)
        smp_init(Traits<Build>::CPUS);
}

void Machine::init()
{
    db<Init, Machine>(TRC) << "Machine::init()" << endl;

    if(Traits<Timer>::enabled)
        Timer::init();

    Machine_Model::init();

#ifdef __USB_H
    if(Traits<USB>::enabled)
        USB::init();
#endif
}

__END_SYS

#include <utility/ostream.h>
#include <time.h>

using namespace EPOS;

OStream cout;

int main()
{
    cout << "Hello world!" << endl;
    Alarm::delay(1000);
    cout << "Hello world again!" << endl;

    return 0;
}

// MMU::alloc() failed!
// MMU::alloc() failed!
// Heap::alloc(this=0x00014398): out of memory!
// PANIC!
// Machine::reboot()
// Timer not installed!
// Heap::alloc(this=0x00014398): out of memory!
// PANIC!
// Machine::reboot()
// Heap::alloc(this=0x00014398): out of memory!
// PANIC!
// Machine::reboot()

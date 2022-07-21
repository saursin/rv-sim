#include "mem.h"
#include "core.h"
#include <memory>

using namespace rvsim;

int main(int argc, char ** argv) 
{
    std::unique_ptr<Memory> ram(new Memory(0x00000000, 64*1024, true));
    std::unique_ptr<Memory> rom(new Memory(0x00000000, 64*1024, true));
    
    Core c(0);
    c.attach_memdevice(rom.get());
    c.attach_memdevice(ram.get());

    c.step();
    c.step();
    c.step();
    c.step();
    c.step();


    return 0;
}
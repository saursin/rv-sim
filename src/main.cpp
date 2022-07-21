#include "mem.h"
#include "core.h"
#include "log.h"
#include <memory>

using namespace rvsim;

int main(int argc, char ** argv) 
{
    std::unique_ptr<Memory> rom(new Memory(0x00000000, 64*1024, false));
    std::unique_ptr<Memory> ram(new Memory(0x04000000, 64*1024, false));

    // populate memory
    // uint32_t code[] = {
    //     0x3e800093,
    //     0x7d008113,
    //     0xc1810193,
    //     0x83018213,
    //     0x3e820293
    // };

    // for (unsigned i=0; i<sizeof(code)/sizeof(uint32_t); i++)
    // {
    //     Word_alias instr;
    //     instr.word = code[i];
    //     rom->write(instr.byte, i*4, 4);
    // }

    rom->loadBinImage("build/rom.bin", 0x00000000);
    log("loading binary image...\n");

    rom->set_write_protect(true);
    
    
    // initialize core
    Core c(0);

    // attach memories
    c.attach_memdevice(rom.get());
    c.attach_memdevice(ram.get());


    for(uint64_t cyc=0; cyc<20; cyc++)
    {
        c.step();
    }

    return 0;
}
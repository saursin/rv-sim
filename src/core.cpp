#include "core.h"
#include <iostream>

using namespace rvsim;

/////////////////////////////////////////////////////////////
// RegFile

Reg RegFile::read(unsigned regnum) 
{
    if (regnum == 0)
        return 0;
    else
        return rf_[regnum-1];
}

void RegFile::write(unsigned regnum, Reg value) 
{
    if (regnum != 0)
        rf_[regnum-1] = value;
}

void RegFile::clear(uint32_t erase_val=0x00000000) 
{
    for(unsigned i=1; i<32; i++)
        rf_[i] = erase_val;
}


/////////////////////////////////////////////////////////////
// Core

Core::Core(uint32_t id):
    id_(id),
    pc_(RESET_PC_ADDR)
{
    reset();
}

void Core::attach_memdevice(MemDevice *md)
{
    mmu_.attach(md);
}

void Core::reset()
{
    rf_.clear();
    pc_ = RESET_PC_ADDR;
}

void Core::step()
{
    // fetch
    Word_alias w;
    mmu_.read(w.byte, pc_, 4);
    printf("Core %d Fetch : [%08x] %08x\n", id_, pc_, w.word);
    pc_ += 4;

    // decode    
    RVInstr instr(w.word);
    

    


    // // execute
    // if (instr.opcode == OPCODE_LUI)
    // {

    // }

    

    
}
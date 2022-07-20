#pragma once
#include "core.h"

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
#pragma once
#include "rvdefs.h"
#include "mem.h"

#include <vector>

namespace rvsim {

class RegFile 
{
private:
    Reg rf_[31];

public:
    Reg read(unsigned regnum);
    void write(unsigned regnum, Reg value);
    void clear(uint32_t erase_val);
};







class Core 
{
public:
    Core();
    void step();
    
private:
    RegFile rf_;
    Reg pc_;

};



} // namespace rvsim
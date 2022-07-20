#pragma once
#include "rvdefs.h"

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

};



} // namespace rvsim
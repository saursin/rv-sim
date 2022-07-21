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


struct RVInstr
{
    uint32_t ins;

    RVInstr(uint32_t instr): ins(instr)
    {}

    uint8_t opcode()
    {
        // opcode in bits 6..0
        return ins & 0x7f;
    }

    uint8_t rd()
    {
        // rd in bits 11..7
        return (ins >> 7) & 0x1f;
    }

    uint8_t rs1()
    {
        // rs1 in bits 19..15
        return (ins >> 15) & 0x1f;
    }

    uint8_t rs2()
    {
        // rs2 in bits 24..20
        return (ins >> 20) & 0x1f;
    }

    uint8_t func3()
    {
        // funct3 in bits 14..12
        return (ins >> 12) & 0x7;
    }

    uint8_t func7()
    {
        // funct7 in bits 31..25
        return (ins >> 25) & 0x7f;
    }

    uint64_t imm_i()
    {
        // imm[11:0] = inst[31:20]
        return ((uint64_t) (int32_t) (ins & 0xfff00000)) >> 20;
    }

    uint64_t imm_s()
    {
        // imm[11:5] = inst[31:25], imm[4:0] = inst[11:7]
        return ((int64_t)(int32_t)(ins & 0xfe000000) >> 20)
            | ((ins >> 7) & 0x1f);
    }

    uint64_t imm_b()
    {
        // imm[12|10:5|4:1|11] = inst[31|30:25|11:8|7]
        return ((int64_t)(int32_t)(ins & 0x80000000) >> 19)
        | ((ins & 0x80) << 4) // imm[11]
        | ((ins >> 20) & 0x7e0) // imm[10:5]
        | ((ins >> 7) & 0x1e); // imm[4:1]}
    }

    uint64_t imm_u()
    {
        // imm[31:12] = inst[31:12]
        return (int64_t)(int32_t)(ins & 0xfffff000);
    }

    uint64_t imm_j()
    {
        // imm[20|10:1|11|19:12] = inst[31|30:21|20|19:12]
        return (uint64_t)((int64_t)(int32_t)(ins & 0x80000000) >> 11)
        | (ins & 0xff000) // imm[19:12]
        | ((ins >> 9) & 0x800) // imm[11]
        | ((ins >> 20) & 0x7fe); // imm[10:1]
    }

    uint32_t shamt()
    {
        // shamt(shift amount) only required for immediate shift instructions
        // shamt[4:5] = imm[5:0]
        return (uint32_t) (imm_i() & 0x1f); // TODO: 0x1f / 0x3f ?
    }
};


class Core
{
public:
    Core(uint32_t id);
    void attach_memdevice(MemDevice *md);
    void reset();
    void step();

private:
    uint32_t id_;
    RegFile rf_;
    Reg pc_;

    MMU mmu_;
};



} // namespace rvsim
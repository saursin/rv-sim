#include "core.h"
#include "log.h"

#include <iostream>
#include <assert.h>

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
    // log_core(id_, "Fetch PC=%08x: %08x\n", pc_, w.word);

    // decode & execute
    RVInstr instr(w.word);

    // log_core(id_, "Decode %08x: opcode=0x%07x func3=0x%x func3=0x%x\n", instr.ins, instr.opcode(), instr.func3(), instr.func7());
        
    if (instr.opcode() == OPCODE_LUI)
    {
        log_core(id_, "Execute PC=%08x: LUI: x%d = %08lx\n", pc_, instr.rd(), instr.imm_u());
        rf_.write(instr.rd(), instr.imm_u());
    }
    else if(instr.opcode() == OPCODE_AUIPC)
    {
        log_core(id_, "Execute AUIPC: x%d = %08lx + PC(%08x)\n", instr.rd(), instr.imm_u());
        rf_.write(instr.rd(), instr.imm_u());
    }
    else if (instr.opcode() == OPCODE_ALU)
    {
        switch(instr.func3())
        {
            // case FUNC3_ADDI:    rf_.write(instr.rd(),  rf_.read(instr.rs1()) + instr.imm_i()); break;
            // case FUNC3_SLTI:    rf_.write(instr.rd(),  (rf_.read(instr.rs1()) < instr.imm_i()) ? 1 : 0); break;
            // case FUNC3_ANDI:    rf_.write(instr.rd(),  rf_.read(instr.rs1()) & instr.imm_i()); break;
            // case FUNC3_ORI:     rf_.write(instr.rd(),  rf_.read(instr.rs1()) | instr.imm_i()); break;
            // case FUNC3_XORI:    rf_.write(instr.rd(),  rf_.read(instr.rs1()) ^ instr.imm_i()); break;

            default: break;
        }
    }

    // Print register view
    #ifdef LOG_REGVIEW
    log_core(id_, "======== Register View ========\n");
    printf(" x0 (zero) : 0x%08x    x16 (a6)   : 0x%08x\n", rf_.read(0), rf_.read(16));
    printf(" x1 (ra)   : 0x%08x    x17 (a7)   : 0x%08x\n", rf_.read(1), rf_.read(17));
    printf(" x2 (sp)   : 0x%08x    x18 (s2)   : 0x%08x\n", rf_.read(2), rf_.read(18));
    printf(" x3 (gp)   : 0x%08x    x19 (s3)   : 0x%08x\n", rf_.read(3), rf_.read(19));
    printf(" x4 (tp)   : 0x%08x    x20 (s4)   : 0x%08x\n", rf_.read(4), rf_.read(20));
    printf(" x5 (t0)   : 0x%08x    x21 (s5)   : 0x%08x\n", rf_.read(5), rf_.read(21));
    printf(" x6 (t1)   : 0x%08x    x22 (s6)   : 0x%08x\n", rf_.read(6), rf_.read(22));
    printf(" x7 (t2)   : 0x%08x    x23 (s7)   : 0x%08x\n", rf_.read(7), rf_.read(23));
    printf(" x8 (s0/fp): 0x%08x    x24 (s8)   : 0x%08x\n", rf_.read(8), rf_.read(24));
    printf(" x9 (s1)   : 0x%08x    x25 (s9)   : 0x%08x\n", rf_.read(9), rf_.read(25));
    printf("x10 (a0)   : 0x%08x    x26 (s10)  : 0x%08x\n", rf_.read(10), rf_.read(26));
    printf("x11 (a1)   : 0x%08x    x27 (s11)  : 0x%08x\n", rf_.read(11), rf_.read(27));
    printf("x12 (a2)   : 0x%08x    x28 (t3)   : 0x%08x\n", rf_.read(12), rf_.read(28));
    printf("x13 (a3)   : 0x%08x    x29 (t4)   : 0x%08x\n", rf_.read(13), rf_.read(29));
    printf("x14 (a4)   : 0x%08x    x30 (t5)   : 0x%08x\n", rf_.read(14), rf_.read(30));
    printf("x15 (a5)   : 0x%08x    x31 (t6)   : 0x%08x\n\n", rf_.read(15), rf_.read(31));
    #endif

    pc_ += 4;
}
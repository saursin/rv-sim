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
        log_core(id_, "PC=%08x: Execute LUI: x%d <= 0x%08lx\n", pc_, instr.rd(), instr.imm_u());
        rf_.write(instr.rd(), instr.imm_u());
    }
    else if(instr.opcode() == OPCODE_AUIPC)
    {
        log_core(id_, "PC=%08x: Execute AUIPC: x%d <= 0x%08lx + PC(0x%08x)\n", pc_, instr.rd(), instr.imm_u());
        rf_.write(instr.rd(), instr.imm_u());
    }
    else if (instr.opcode() == OPCODE_ALU)
    {
        if(instr.func3() == FUNC3_ADDI)
        {
            log_core(id_, "PC=%08x: Execute ADDI: x%d <= x%d(0x%08x) + 0x%08lx\n", pc_, instr.rd(), instr.rs1(), rf_.read(instr.rs1()), instr.imm_i());
            rf_.write(instr.rd(), rf_.read(instr.rs1()) + instr.imm_i());
        }
        else if(instr.func3() == FUNC3_ANDI)
        {
            log_core(id_, "PC=%08x: Execute ANDI: x%d <= x%d(0x%08x) & 0x%08lx\n", pc_, instr.rd(), instr.rs1(), rf_.read(instr.rs1()), instr.imm_i());
            rf_.write(instr.rd(), rf_.read(instr.rs1()) & instr.imm_i());
        }
        else if(instr.func3() == FUNC3_ORI)
        {
            log_core(id_, "PC=%08x: Execute ORI: x%d <= x%d(0x%08x) | 0x%08lx\n", pc_, instr.rd(), instr.rs1(), rf_.read(instr.rs1()), instr.imm_i());
            rf_.write(instr.rd(), rf_.read(instr.rs1()) | instr.imm_i());
        }
        else if(instr.func3() == FUNC3_XORI)
        {
            log_core(id_, "PC=%08x: Execute XORI: x%d <= x%d(0x%08x) ^ 0x%08lx\n", pc_, instr.rd(), instr.rs1(), rf_.read(instr.rs1()), instr.imm_i());
            rf_.write(instr.rd(), rf_.read(instr.rs1()) ^ instr.imm_i());
        }
    }

    // Print register view
    #ifdef LOG_REGVIEW
    log_core(id_, "======== Register View ======================================================\n");
    fprintf(stderr, "(zero) x0: 0x%08x  (fp/s0)  x8: 0x%08x     (a6) x16: 0x%08x     (s8) x24: 0x%08x\n", rf_.read(0), rf_.read(8),  rf_.read(16), rf_.read(24));
    fprintf(stderr, "  (ra) x1: 0x%08x     (s1)  x9: 0x%08x     (a7) x17: 0x%08x     (s9) x25: 0x%08x\n", rf_.read(1), rf_.read(9),  rf_.read(17), rf_.read(25));
    fprintf(stderr, "  (sp) x2: 0x%08x     (a0) x10: 0x%08x     (s2) x18: 0x%08x    (s10) x26: 0x%08x\n", rf_.read(2), rf_.read(10), rf_.read(18), rf_.read(26));
    fprintf(stderr, "  (gp) x3: 0x%08x     (a1) x11: 0x%08x     (s3) x19: 0x%08x    (s11) x27: 0x%08x\n", rf_.read(3), rf_.read(11), rf_.read(19), rf_.read(27));
    fprintf(stderr, "  (tp) x4: 0x%08x     (a2) x12: 0x%08x     (s4) x20: 0x%08x     (t3) x28: 0x%08x\n", rf_.read(4), rf_.read(12), rf_.read(20), rf_.read(28));
    fprintf(stderr, "  (t0) x5: 0x%08x     (a3) x13: 0x%08x     (s5) x21: 0x%08x     (t4) x29: 0x%08x\n", rf_.read(5), rf_.read(13), rf_.read(21), rf_.read(29));
    fprintf(stderr, "  (t1) x6: 0x%08x     (a4) x14: 0x%08x     (s6) x22: 0x%08x     (t5) x30: 0x%08x\n", rf_.read(6), rf_.read(14), rf_.read(22), rf_.read(30));
    fprintf(stderr, "  (t2) x7: 0x%08x     (a5) x15: 0x%08x     (s7) x23: 0x%08x     (t6) x31: 0x%08x\n\n", rf_.read(7), rf_.read(15), rf_.read(23), rf_.read(31));
    #endif

    pc_ += 4;
}
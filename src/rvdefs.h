#pragma once
#include <stdint.h>

#define XLEN 32

union Word_alias
{
    uint8_t byte [4];
    uint16_t hword [2];
    uint32_t word;
};

// Datatype for a register
using Reg = uint32_t;
using Reg_s = int32_t;

// Other data types
using Byte      = uint8_t;
using halfWord  = uint16_t;
using Word      = uint32_t;
using Double    = uint64_t;


// Memory map
#define ROM_SIZE 64*1024
#define ROM_ADDR 0x00000000

#define RAM_SIZE 32*1024
#define RAM_ADDR 0x04000000

// Reset address
#define RESET_PC_ADDR 0x00000000


//////////////////////////////////////////////////////////////////
// ISA DEFS

// imm[31:12] rd 0110111 LUI
#define OPCODE_LUI  0b0110111

// imm[31:12] rd 0010111 AUIPC
#define OPCODE_AUIPC 0b0010111

// imm[20|10:1|11|19:12] rd 1101111 JAL
#define OPCODE_JAL  0b1101111

// imm[11:0] rs1 000 rd 1100111 JALR
#define OPCODE_JALR 0b1100111


// imm[12|10:5] rs2 rs1 000 imm[4:1|11] 1100011 BEQ
// imm[12|10:5] rs2 rs1 001 imm[4:1|11] 1100011 BNE
// imm[12|10:5] rs2 rs1 100 imm[4:1|11] 1100011 BLT
// imm[12|10:5] rs2 rs1 101 imm[4:1|11] 1100011 BGE
// imm[12|10:5] rs2 rs1 110 imm[4:1|11] 1100011 BLTU
// imm[12|10:5] rs2 rs1 111 imm[4:1|11] 1100011 BGEU
#define OPCODE_B    0b1100011
#define FUNC3_BEQ   0b000
#define FUNC3_BNE   0b001
#define FUNC3_BLT   0b100
#define FUNC3_BGE   0b101
#define FUNC3_BLUT  0b110
#define FUNC3_BGEU  0b111


// imm[11:0] rs1 000 rd 0000011 LB
// imm[11:0] rs1 001 rd 0000011 LH
// imm[11:0] rs1 010 rd 0000011 LW
// imm[11:0] rs1 100 rd 0000011 LBU
// imm[11:0] rs1 101 rd 0000011 LHU
#define OPCODE_L    0b0000011
#define FUNC3_LB    0b000
#define FUNC3_LH    0b001
#define FUNC3_LW    0b010
#define FUNC3_LBU   0b100
#define FUNC3_LHU   0b101


// imm[11:5] rs2 rs1 000 imm[4:0] 0100011 SB
// imm[11:5] rs2 rs1 001 imm[4:0] 0100011 SH
// imm[11:5] rs2 rs1 010 imm[4:0] 0100011 SW
#define OPCODE_S    0b0100011
#define FUNC3_SB    0b000
#define FUNC3_SH    0b001
#define FUNC3_SW    0b010



#define OPCODE_ALU  0b0010011

// imm[11:0] rs1 000 rd 0010011 ADDI
// imm[11:0] rs1 010 rd 0010011 SLTI
// imm[11:0] rs1 011 rd 0010011 SLTIU
// imm[11:0] rs1 100 rd 0010011 XORI
// imm[11:0] rs1 110 rd 0010011 ORI
// imm[11:0] rs1 111 rd 0010011 ANDI
#define FUNC3_ADDI  0b000
#define FUNC3_SLTI  0b010
#define FUNC3_SLTIU 0b011
#define FUNC3_XORI  0b100
#define FUNC3_ORI   0b110
#define FUNC3_ANDI  0b111

// 0000000 shamt rs1 001 rd 0010011 SLLI
// 0000000 shamt rs1 101 rd 0010011 SRLI
// 0100000 shamt rs1 101 rd 0010011 SRAI

#define FUNC3_SLLI  0b001
#define FUNC3_SRLI  0b101
#define FUNC3_SRAI  0b101

#define FUNC7_SLLI  0b0000000
#define FUNC7_SRLI  0b0000000
#define FUNC7_SRAI  0b0100000


// 0000000 rs2 rs1 000 rd 0110011 ADD
// 0100000 rs2 rs1 000 rd 0110011 SUB
// 0000000 rs2 rs1 001 rd 0110011 SLL
// 0000000 rs2 rs1 010 rd 0110011 SLT
// 0000000 rs2 rs1 011 rd 0110011 SLTU
// 0000000 rs2 rs1 100 rd 0110011 XOR
// 0000000 rs2 rs1 101 rd 0110011 SRL
// 0100000 rs2 rs1 101 rd 0110011 SRA
// 0000000 rs2 rs1 110 rd 0110011 OR
// 0000000 rs2 rs1 111 rd 0110011 AND


// 0000 pred succ 00000 000 00000 0001111 FENCE
// 0000 0000 0000 00000 001 00000 0001111 FENCE.I
// 000000000000 00000 000 00000 1110011 ECALL
// 000000000001 00000 000 00000 1110011 EBREAK
// csr rs1 001 rd 1110011 CSRRW
// csr rs1 010 rd 1110011 CSRRS
// csr rs1 011 rd 1110011 CSRRC
// csr zimm 101 rd 1110011 CSRRWI
// csr zimm 110 rd 1110011 CSRRSI
// csr zimm 111 rd 1110011 CSRRCI
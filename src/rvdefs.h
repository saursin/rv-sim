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

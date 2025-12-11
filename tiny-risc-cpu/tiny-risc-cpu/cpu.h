#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MEM_SIZE 65536 //65536 = 2^16, because  uint16_t addresses can naturally index.
#define registers 8    //we have 8 general purpose registers

//memory layout constants
#define CODE_END    256         //reserve addresses 0 to 255 for code
#define DATA_START  CODE_END

//let us build the cpu in struct
typedef struct{
    uint16_t mem[MEM_SIZE];         //Memory,  this is like the cpu ram
    uint16_t R[registers];          //REGISTERS R0-R7, alu happens here quickly
    uint16_t PC;                    //pc, address of nect instruction
    bool ZF;                        //zero flag, if operation result is 0, we'll use this
    bool running;                   //cpu is running - true or halt - false
} cpu_t;

//opcode enum : 4 bit opcodes (0x0 - 0xF).
//using 4 bit opcodes so they fit in the top 4 bits of our 16bit instruction word
enum {
    OP_NOP = 0x0,
    OP_ADD = 0x1,
    OP_SUB = 0x2,
    OP_AND = 0x3,
    OP_OR  = 0x4,
    OP_XOR = 0x5,
    OP_LD  = 0x6,
    OP_ST  = 0x7,
    OP_BEQ = 0x8,
    OP_JMP = 0x9,
    OP_HALT= 0xF
};

//  Helper to build a 16-BIT instruction word
//  |   opcode    |   rd  |   rs  |   rt  |
static inline uint16_t R_TYPE (uint8_t opcode, uint8_t rd, uint8_t rs, uint8_t rt)
{
    return  ((uint16_t)opcode   <<12)    |
            ((uint16_t)rd       <<8 )    |
            ((uint16_t)rs       <<4 )    |
            ((uint16_t)(rt));
}


/**********************************CONTROL APIs**********************************/
//function to reset or init cpu
void cpu_reset(cpu_t *cpu);


//perform single instruction i.e fetch->decode->execute
void cpu_step(cpu_t *cpu);

//loop cpu_run until running becomes false
void cpu_run(cpu_t *cpu);

#endif // !CPU_H

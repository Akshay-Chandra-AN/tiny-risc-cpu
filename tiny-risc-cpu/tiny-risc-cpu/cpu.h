#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdbool.h>

#define MEM_SIZE 65536 //65536 = 2^16, because  uint16_t addresses can naturally index.
#define registers 8    //we have 8 general purpose registers

//let us build the cpu in struct
typedef struct{
    uint16_t mem[MEM_SIZE];         //Memory,  this is like the cpu ram
    uint16_t R[registers];          //REGISTERS R0-R7, alu happens here quickly
    uint16_t PC;                    //pc, address of nect instruction
    bool ZF;                        //zero flag, if operation result is 0, we'll use this
    bool running;                   //cpu is running - true or halt - false
} cpu_t;

//function to reset or init cpu
void cpu_reset(cpu_t *cpu);

#endif // !CPU_H

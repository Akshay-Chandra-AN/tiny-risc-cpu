#include "cpu.h"
#include <inttypes.h>
//cpu reset function
void cpu_reset (cpu_t *cpu)
{
    for(int i=0;i<registers;i++)
    {
        cpu->R[i]=0;        //clearing all the 7 registers
    }
    
    for(int i=0;i<MEM_SIZE;i++)
    {
        cpu->mem[i]=0;        //This simulates RAM being cleared on reset.
    }
    
    
    /*program counter to 0 CPU will start executing from memory address 0.
    This is our simple reset vector */
    cpu->PC = 0;
    
    
    cpu->ZF = false;            //Not using this
    
    /*After reset, we don't assume it's running yet. Later, cpu_run() (when we write it)
     will set running = true and start executing instructions.*/
    cpu->running = false;       //halted
}

void cpu_step(cpu_t *cpu)
{
    if(cpu->PC >= CODE_END) //if pc is more than 256
    {
        printf("security fault : pc out of code region (PC=%U)\n",cpu->PC);
        cpu->running = false;
        return;
    }
    
    //go to the memory address stored in PC and read one instruction from there.
    //then move PC to the next instruction
    /* instruction = memory [pc], pc = pc + 1;*/
    uint16_t instr = cpu->mem[cpu->PC++];      //reads the 16 bit word at address PC
    
    
    //pizza analogy, we have 4 slice, take each slice, using bit masking
    //0xf to mask everything else
    //>> to bring it to lowest nibble
    uint8_t opcode = (instr >> 12) & 0xF;
    uint8_t rd =  (instr >> 8) & 0xF;
    uint8_t rs =  (instr >> 4) & 0xF;
    uint8_t rt = instr & 0xF;
    
    switch(opcode) {
        case OP_NOP: break;
            
        case OP_ADD:
            cpu->R[rd] = cpu->R[rs] + cpu->R[rt];
            cpu->ZF = (cpu->R[rd] == 0);
            break;
            
        case OP_SUB:
            cpu->R[rd] = cpu->R[rs] - cpu->R[rt];
            cpu->ZF = (cpu->R[rd] == 0);
            break;
        
        case OP_AND:
            cpu->R[rd] = cpu->R[rs] & cpu->R[rt];
            cpu->ZF = (cpu->R[rd] == 0);
            break;
        
        case OP_OR:
            cpu->R[rd] = cpu->R[rs] | cpu->R[rt];
            cpu->ZF = (cpu->R[rd] == 0);
            break;
        
        case OP_XOR:
            cpu->R[rd] = cpu->R[rs] ^ cpu->R[rt];
            cpu->ZF = (cpu->R[rd] == 0);
            break;
            
        case OP_LD: {
            uint32_t addr = cpu->mem[cpu->PC++];
            if (addr < MEM_SIZE) {
                cpu->R[rd] = cpu->mem[addr];
                cpu->ZF = (cpu->R[rd] == 0);
            }
            else
            {
                printf("LD: invalid addr %" PRIu32 "\n", addr);
                cpu->running = false;
            }
            break;
        }
            
        case OP_ST:
        {
            uint32_t addr = cpu->mem[cpu->PC++];
            //only allowing writing to the data region
            if(addr >= (uint32_t)DATA_START && addr < MEM_SIZE)
            {
                cpu->mem[addr] = cpu->R[rs];
            }
            else{
                printf("SECURITY FAULT : ST TO INVALID ADDRR%"PRIu32"\n", addr);
                cpu->running = false;
            }
            break;
        }
        
    }
}

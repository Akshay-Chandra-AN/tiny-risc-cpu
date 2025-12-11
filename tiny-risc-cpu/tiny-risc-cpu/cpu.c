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
            
        /*reads next word for target, addr.
        //why wrote protection ? prevents self modifying code or accidental corruption of code
         region - the emulator mimcs a simple (Write XOR Execute)protection.
         */
        case OP_ST: //two word, store
        {
            uint32_t addr = cpu->mem[cpu->PC++];
            
            //enforce write-protection: only allowing writing to the data region (addr>=data_start)
            
            if(addr >= (uint32_t)DATA_START && addr < MEM_SIZE)
            {   //uses rs register as source of data. rd is ignored in this encoding
                cpu->mem[addr] = cpu->R[rs];
            }
            else{
                printf("SECURITY FAULT : ST TO INVALID ADDRR%"PRIu32"\n", addr);
                cpu->running = false;
            }
            break;
        }
        
        case OP_JMP: {
            uint32_t addr = cpu->mem[cpu->PC++];
            
            //VALIDATE JUMP TARGET IS executable
            if(addr < (uint32_t)CODE_END)
            {
                cpu->PC = (uint16_t)addr;
            }
            else{
                printf("security fault : jmp target %"PRIu32"not in code region\n", addr);
                cpu->running = false;
            }
            break;
        }
            
        /*
         reads branch target addr from next memory word. if registers are equal, set PC to addr.
         */
        case OP_BEQ: //conditional branch
        {
            //check for address word
            if(cpu->PC >= MEM_SIZE-1)
            {
                printf("fault: BEQ needs address word but PC = %"PRIu32"out of range\n",(uint32_t)cpu->PC);
                cpu->running = false;
                break;
            }
            uint32_t addr = cpu->mem[cpu->PC++];
            /*
             add a security check, brnach target must be inside code region (addr < CODE_END),
            otherwise a fault. Even if the branch target is allowed to be arbitary numeric,
             the emulator chooses to restrict targets to executable addresses only (fail-fast)
             */
            if(addr < (uint32_t)CODE_END) {  //If registers are equal, sets PC to addr.
                cpu->PC = (uint16_t)addr;
            } else {
                printf("security fault : beq target %"PRIu32"not in code region\n",addr);
                cpu->running = false;
            }
            break;
        }
        
        case OP_HALT:
            cpu->running = false;
            break;
            
        default :
            printf("unknown opcode 0x%X at PC=%"PRIu32 "\n", opcode, (uint32_t)(cpu->PC -1));
            cpu->running = false;
            break;
    }
}

//cpu-run : start executyion until HALT or a fauly sets running  = false;
void cpu_run(cpu_t *cpu)
{
    cpu->running = true;
    while(cpu->running)
    {
        cpu_step(cpu);
    }
}

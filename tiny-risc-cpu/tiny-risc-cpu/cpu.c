#include "cpu.h"

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

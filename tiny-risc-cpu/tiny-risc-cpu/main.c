#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char * argv[]) {
    cpu_t cpu; //this creates one instnace of cpu , on stack
    cpu_reset(&cpu);    //pass the address of cpu to initialize it
    
    //test code 1 start
    printf("cpu reset done\n");
    printf("program counter is %u\n",cpu.PC);
    printf("r0 is %u\n",cpu.R[0]);
    printf("cpu state running = %s\n", cpu.running ? "true":"false");
     //test code 1 end */
    
    /*  test code 2 start
    prepare data in memory (in data region)   */
    cpu.mem[DATA_START + 0]= 10;
    cpu.mem[DATA_START + 1]= 20;
    
    /*
    let us now simple pgm at address 0 - code region
    ld r1, [data start + 0],
    ld r2, [data start + 1,
    add r3, r1, r2
    st r3, [datastart + 2]
    halt
    */
    uint16_t i = 0;
    cpu.mem[i++] = R_TYPE(OP_LD, 1, 0, 0);
    cpu.mem[i++] = DATA_START + 0;
    
    cpu.mem[i++] = R_TYPE(OP_LD, 2, 0, 0);
    cpu.mem[i++] = DATA_START + 1;
    
    cpu.mem[i++] = R_TYPE(OP_ADD, 3, 1, 2);
    
    cpu.mem[i++] = R_TYPE(OP_ST, 0, 3, 0);
    cpu.mem[i++] = DATA_START + 2;
    
    cpu.mem[i++] = R_TYPE(OP_HALT, 0, 0, 0);
    
    //print
    printf("before run : mem %u = %u, mem[%u] = %u\n",
           DATA_START + 0, cpu.mem[DATA_START + 0],
           DATA_START + 1, cpu.mem[DATA_START + 1]);
    
    //RUN THE CPU
    //sets cpu->running  = true and loops cpu_step until halt or a fault.
    cpu_run(&cpu);
    
    //print results
    printf("after run\n");
    printf("R1=%u, R2=%u, R3=%u\n", cpu.R[1], cpu.R[2], cpu.R[3]);
    printf("mem[%u] (result) = %u\n", DATA_START + 2, cpu.mem[DATA_START + 2]);
    return EXIT_SUCCESS;
}

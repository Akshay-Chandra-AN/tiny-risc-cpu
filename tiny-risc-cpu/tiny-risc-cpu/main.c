#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char * argv[]) {
    cpu_t cpu; //this creates one instnace of cpu , on stack
    cpu_reset(&cpu);    //pass the address of cpu to initialize it
    
    printf("cpu reset done\n");
    printf("program counter is %u\n",cpu.PC);
    printf("r0 is %u\n",cpu.R[0]);
    printf("cpu state running = %s\n", cpu.running ? "true":"false");
    return EXIT_SUCCESS;
}

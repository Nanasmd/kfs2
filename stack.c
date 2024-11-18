#include "stack.h"
#include "terminal.h"

/*
 * Affiche la trace de la pile
 * Format lisible demandé par le sujet
 */
void print_stack_trace(void) {
    uint32_t* ebp;
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    terminal_writestring("\nTrace de la pile:\n");
    terminal_writestring("================\n");

    int frame = 0;
    while (ebp) {
        uint32_t eip = *(ebp + 1);
        terminal_writestring("Frame ");
        terminal_write_dec(frame);
        terminal_writestring(" -> EIP: ");
        terminal_write_hex(eip);
        terminal_writestring("\n");

        ebp = (uint32_t*)*ebp;
        frame++;
    }
}

/*
 * Affiche les informations sur l'état actuel de la pile
 */
void print_stack_info(void) {
    uint32_t esp, ebp;
    
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));

    terminal_writestring("\nÉtat de la pile:\n");
    terminal_writestring("=============\n");
    terminal_writestring("ESP: ");
    terminal_write_hex(esp);
    terminal_writestring("\nEBP: ");
    terminal_write_hex(ebp);
    terminal_writestring("\n");
}

#include "shell.h"
#include "keyboard.h"
#include "terminal.h"
#include "stack.h"
#include "gdt.h"

static char input_buffer[SHELL_BUFFER_SIZE];
static uint32_t buffer_pos = 0;

/*
 * Commandes disponibles dans le shell
 * Implémentation du bonus avec commandes utiles
 */
static const shell_command_t commands[] = {
    {"help", "Affiche l'aide", cmd_help},
    {"stack", "Affiche l'état de la pile", cmd_stack},
    {"gdt", "Affiche les infos GDT", cmd_gdt},
    {"reboot", "Redémarre le système", cmd_reboot},
    {"halt", "Arrête le système", cmd_halt},
    {"clear", "Efface l'écran", cmd_clear},
    {NULL, NULL, NULL}
};

void shell_init(void) {
    terminal_writestring("\nKFS2 Shell de Debug\n");
    terminal_writestring("================\n");
    terminal_writestring("Tapez 'help' pour la liste des commandes\n\n");
}

/*
 * Implémentation des commandes
 */
void cmd_help(int argc, char** argv) {
    terminal_writestring("Commandes disponibles:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        terminal_writestring("  ");
        terminal_writestring(commands[i].name);
        terminal_writestring(" - ");
        terminal_writestring(commands[i].description);
        terminal_writestring("\n");
    }
}

void cmd_stack(int argc, char** argv) {
    print_stack_info();
    print_stack_trace();
}

void cmd_gdt(int argc, char** argv) {
    print_gdt_info();
}

void cmd_reboot(int argc, char** argv) {
    terminal_writestring("Redémarrage...\n");
    // Magic reboot
    outb(0x64, 0xFE);
}

void cmd_halt(int argc, char** argv) {
    terminal_writestring("Arrêt du système.\n");
    asm volatile("hlt");
}

void cmd_clear(int argc, char** argv) {
    terminal_clear();
    shell_init();
}

/*
 * Boucle principale du shell
 */
void shell_run(void) {
    char c;
    shell_prompt();
    
    while (1) {
        if (keyboard_haskey()) {
            c = keyboard_getchar();
            
            if (c == '\n') {
                terminal_putchar('\n');
                execute_command();
                buffer_pos = 0;
                shell_prompt();
            }
            else if (c >= ' ' && buffer_pos < SHELL_BUFFER_SIZE - 1) {
                input_buffer[buffer_pos++] = c;
                terminal_putchar(c);
            }
        }
    }
}

#include "gdt.h"
#include "terminal.h"

/*
 * Table GDT placée à l'adresse 0x800
 * Utilisation d'une section spéciale pour le placement
 */
__attribute__((section(".gdt"))) struct GDTEntry gdt_entries[6] = {0};
struct GDTPointer gdt_ptr;

/*
 * Configuration d'une entrée GDT
 * params:
 * - num: index dans la GDT
 * - base: adresse de base du segment
 * - limit: limite du segment
 * - access: flags d'accès
 * - gran: flags de granularité
 */
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

/*
 * Initialisation de la GDT
 * Crée les 6 segments requis par KFS2:
 * - NULL descriptor (obligatoire)
 * - Code kernel
 * - Data kernel
 * - Stack kernel
 * - Code user
 * - Data user
 */
void init_gdt(void) {
    gdt_ptr.limit = (sizeof(struct GDTEntry) * 6) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // Segment NULL (requis)
    gdt_set_gate(0, 0, 0, 0, 0);

    // Segments kernel (ring 0)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0x96, 0xCF); // Stack

    // Segments user (ring 3)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xFA, 0xCF); // Code
    gdt_set_gate(5, 0, 0xFFFFFFFF, 0xF2, 0xCF); // Data

    // Chargement de la GDT
    gdt_flush(&gdt_ptr);
}

/*
 * Affiche les informations sur la GDT
 * Format lisible pour le debugging
 */
void print_gdt_info(void) {
    terminal_writestring("\nInformations GDT:\n");
    terminal_writestring("Base: 0x800\n");
    terminal_writestring("Taille: ");
    terminal_write_hex(gdt_ptr.limit + 1);
    terminal_writestring(" octets\n\n");

    const char* types[] = {
        "NULL",
        "Code Kernel",
        "Data Kernel",
        "Stack Kernel",
        "Code User",
        "Data User"
    };

    for (int i = 0; i < 6; i++) {
        terminal_writestring("Segment ");
        terminal_write_dec(i);
        terminal_writestring(": ");
        terminal_writestring(types[i]);
        terminal_writestring("\n");
    }
}

#include "gdt.h"

// GDT placée à l'adresse 0x800 grâce au linker script
struct GDTEntry gdt[7] __attribute__((section(".gdt")));
struct GDTPointer gdt_ptr;

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    gdt[num].granularity |= gran & 0xF0;

    gdt[num].access = access;
}

void init_gdt()
{
    // Configure le pointeur GDT
    gdt_ptr.limit = (sizeof(struct GDTEntry) * 7) - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    // Descripteur null
    gdt_set_gate(0, 0, 0, 0, 0);

    // Segment de code du kernel
    gdt_set_gate(1, 0, 0xFFFFFFFF, GDT_CODE_PL0, 0xCF);

    // Segment de données du kernel
    gdt_set_gate(2, 0, 0xFFFFFFFF, GDT_DATA_PL0, 0xCF);

    // Segment de pile du kernel
    gdt_set_gate(3, 0, 0xFFFFFFFF, GDT_STACK_PL0, 0xCF);

    // Segment de code utilisateur
    gdt_set_gate(4, 0, 0xFFFFFFFF, GDT_CODE_PL3, 0xCF);

    // Segment de données utilisateur
    gdt_set_gate(5, 0, 0xFFFFFFFF, GDT_DATA_PL3, 0xCF);

    // Segment de pile utilisateur
    gdt_set_gate(6, 0, 0xFFFFFFFF, GDT_STACK_PL3, 0xCF);

    // Charge la nouvelle GDT
    load_gdt(&gdt_ptr);
}
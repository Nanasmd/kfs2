/*
 * KFS2 - Implémentation GDT
 * 
 * Ce fichier définit la structure de la Global Descriptor Table
 * conformément aux exigences du sujet KFS2:
 * - Placement à 0x800
 * - 6 segments (code/données/stack pour kernel et user)
 */

#ifndef GDT_H
#define GDT_H

#include <stdint.h>

/*
 * Structure d'une entrée GDT (8 octets)
 * Format conforme aux spécifications Intel
 */
struct GDTEntry {
    uint16_t limit_low;    // Limite (bits 0-15)
    uint16_t base_low;     // Base (bits 0-15)
    uint8_t  base_middle;  // Base (bits 16-23)
    uint8_t  access;       // Flags d'accès
    uint8_t  granularity;  // Granularité et limite (bits 16-19)
    uint8_t  base_high;    // Base (bits 24-31)
} __attribute__((packed));

/*
 * Pointeur vers la GDT pour LGDT
 * Structure utilisée pour informer le BIOS
 */
struct GDTPointer {
    uint16_t limit;        // Taille de la GDT - 1
    uint32_t base;         // Adresse de base de la GDT (0x800)
} __attribute__((packed));

// Prototypes des fonctions
void init_gdt(void);
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void print_gdt_info(void);

// Fonction assembleur externe
extern void gdt_flush(struct GDTPointer* gdt_ptr);

#endif

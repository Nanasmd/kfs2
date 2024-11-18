#ifndef STACK_H
#define STACK_H

#include <stdint.h>

/*
 * Fonctions de gestion et d'affichage de la pile
 * Implémente la fonctionnalité requise d'affichage
 * de la pile en format lisible
 */

void print_stack_trace(void);
void print_stack_info(void);
uint32_t get_ebp(void);
uint32_t get_esp(void);

#endif

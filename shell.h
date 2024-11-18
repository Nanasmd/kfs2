#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

#define SHELL_BUFFER_SIZE 256
#define MAX_ARGS 16

/*
 * Shell de debug (Bonus)
 * Permet d'exécuter des commandes de diagnostic
 */

typedef struct {
    const char* name;
    const char* description;
    void (*function)(int argc, char** argv);
} shell_command_t;

void shell_init(void);
void shell_run(void);

#endif

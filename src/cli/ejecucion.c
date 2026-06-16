/*
 * Copyright (C) 2026 Giovanni Alfredo Garciliano Diaz
 *
 * This file is part of Lily.
 *
 * Lily is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Lily is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Lily. If not, see <https://www.gnu.org/licenses/>.
 */

#include "ejecucion.h"

static void f_ejecutora_interactiva_h(void);
static void f_ejecutora_interactiva_m(struct lily_lua_ejecucion_maquina* maquina, int* c);
static void f_ejecutora_interactiva_r(struct lily_lua_ejecucion_maquina* maquina, int* c);

void f_ejecutora_interactiva(struct lily_lua_ejecucion_maquina* maquina, struct lily_lua_ejecucion_ctx* ctx) {
    int c;
    // La ejecución de corrido fue seleccionada
    if (!(ctx->paso_a_paso) && ctx->estado == COD_OK) {
        return;
    }
    else ctx->paso_a_paso = true;

    // Buffer
    struct termios tios_nb, tios_b;
    tcgetattr(STDIN_FILENO, &tios_nb);
    tcgetattr(STDIN_FILENO, &tios_b);
    tios_nb.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &tios_nb);

    bool seguir = false;
    bool finalizar = false;
    uint64_t valor_pc = 0;
    while (!seguir) {
        lily_lua_ejecucion_leer_registro(maquina, maquina->pc, (uint8_t*) &valor_pc, false);
        printf("$:0x%lx %u> ", valor_pc, ctx->estado);
        c = getchar();
        switch (c) {
            //case 'c':
            //puts("\nComplement!");
            //break;
            //case 'd':
            //puts("\nCore dump not implemented yet");
            //break;
        case 'e':
            ctx->estado = COD_OK;
            ctx->paso_a_paso = false;
            putchar('\n');
            seguir = true;
            break;
        case 'h':
            f_ejecutora_interactiva_h();
            break;
        case 'm':
            tcsetattr(STDIN_FILENO, TCSANOW, &tios_b);
            f_ejecutora_interactiva_m(maquina, &c);
            tcsetattr(STDIN_FILENO, TCSANOW, &tios_nb);
            break;
        case 4:
            puts(_("\nGoodbye, core dump!"));
            seguir = true;
            finalizar = true;
            break;
        case 'q':
            puts(_("\nGoodbye"));
            seguir = true;
            finalizar = true;
            break;
        case 'r':
            tcsetattr(STDIN_FILENO, TCSANOW, &tios_b);
            f_ejecutora_interactiva_r(maquina, &c);
            tcsetattr(STDIN_FILENO, TCSANOW, &tios_nb);
            break;
        case 's':
            putchar('\n');
            ctx->estado = COD_OK;
            ctx->paso_a_paso = true;
            seguir = true;
            break;
        case '\n':
            break;
        default:
            puts(_("\nBad command"));
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &tios_b);
    if (finalizar) ctx->estado = COD_LUA_EJECUCION_MAQUINA_DETENIDA_USUARIO;
}

void f_ejecutora_desatendida(struct lily_lua_ejecucion_maquina* maquina, struct lily_lua_ejecucion_ctx* ctx) {
    (void) maquina;
    (void) ctx;
    return;
}

static void f_ejecutora_interactiva_h(void) {
    puts(_("\nAvailable commands:"));
    //puts(_("c reg         Complements the value of the specified register reg"));
    //puts(_("d             Generate a core dump"));
    puts(_("e             Execute or resume after a stop until a halt instruction"));
    puts(_("h             Show this help"));
    puts(_("m addr [int]  Print or modify the value of memory at address <addr>"));
    puts(_("q             Abort the execution and exit"));
    puts(_("r reg [int]   Print or modify the value of register <reg>"));
    puts(_("s             Execute a single instruction and stops"));
    puts(_("<EOF>         Abort the execution, generate a core dump, and exit"));
    putchar('\n');
    puts(_("addr is a valid memory address in hexadecimal"));
    puts(_("reg is a valid name for a register"));
    puts(_("int is a valid positive integer in hexadecimal"));
}

static void f_ejecutora_interactiva_m(struct lily_lua_ejecucion_maquina* maquina, int* c) {
    uint64_t direccion, valor, valor_anterior;
    int devueltos;
    bool malformado = false;
    putchar(' ');
    devueltos = scanf("%lx", &direccion);
    if (feof(stdin)) {
        while ((*c = getchar()) != '\n' && *c != EOF);
        clearerr(stdin);
        putchar('\n');
    }
    else {
        if (devueltos == 1) {
            // Hubo éxito leyendo el primer parámetro
            if ((*c = getchar()) != '\n') {
                // Hay más caracteres
                ungetc(*c, stdin);
                // Intentamos leer el segundo parámetro
                devueltos = scanf("%lx", &valor);
                if ((*c = getchar()) != '\n') {
                    // Hay basura
                    ungetc(*c, stdin);
                    malformado = true;
                }
                else {
                    // No hay basura, hemos leído dos parámetros bien
                    if (lily_lua_ejecucion_escribir_memoria(maquina, direccion, (uint8_t*) &valor, (uint8_t*) &valor_anterior, false) == -1)
                        printf(_("Memory address 0x%08lx out of range\n"), direccion);
                    else
                        printf("\x1b[33;1m[0x%08lx]\x1b[0m = \x1b[2m0x%02x\x1b[0m -> 0x%02x\n", direccion, (uint8_t) valor_anterior, (uint8_t) valor);
                }
            }
            else {
                // Hemos leído un parámetro bien
                if (lily_lua_ejecucion_leer_memoria(maquina, direccion, (uint8_t*) &valor, false) == -1)
                    printf(_("Memory address 0x%08lx out of range\n"), direccion);
                else
                    printf("\x1b[33;1m[0x%08lx]\x1b[0m = 0x%02x\n", direccion, (uint8_t) valor);
            }
        }
        else {
            // Hubo errores al leer el primer parámetro
            malformado = true;
        }
    }
    if (malformado) {
        puts(_("Error reading value"));
        while ((*c = getchar()) != '\n') ;
    }
}

static void f_ejecutora_interactiva_r(struct lily_lua_ejecucion_maquina *maquina, int *c) {
    char registro[64] = {0};
    uint64_t valor, valor_anterior;
    int devueltos;
    bool malformado = false;
    putchar(' ');
    devueltos = scanf("%[A-Za-z0-9]", registro);
    if (feof(stdin)) {
        while ((*c = getchar()) != '\n' && *c != EOF);
        clearerr(stdin);
        putchar('\n');
    }
    else {
        if (devueltos == 1) {
            // Hubo éxito leyendo el primer parámetro
            if ((*c = getchar()) != '\n') {
                // Hay más caracteres
                ungetc(*c, stdin);
                // Intentamos leer el segundo parámetro
                devueltos = scanf("%lx", &valor);
                if ((*c = getchar()) != '\n') {
                    // Hay basura
                    ungetc(*c, stdin);
                    malformado = true;
                }
                else {
                    // No hay basura, hemos leído dos parámetros bien
                    if (lily_lua_ejecucion_escribir_registro(maquina, registro, (uint8_t*) &valor, (uint8_t*) &valor_anterior, false) == -1) {
                        printf(_("Register '%s' is not defined\n"), registro);
                    }
                    else {
                        printf("\x1b[33;1m%%%s\x1b[0m = \x1b[2m0x%02lx\x1b[0m -> 0x%02lx\n", registro, valor_anterior, valor);
                    }
                }
            }
            else {
                // Hemos leído un parámetro bien
                if (lily_lua_ejecucion_leer_registro(maquina, registro, (uint8_t*) &valor, false) == -1) {
                    printf(_("Register '%s' is not defined\n"), registro);
                }
                else {
                    printf("\x1b[33;1m%%%s\x1b[0m = 0x%lx\n", registro, valor);
                }
            }
        }
        else {
            // Hubo errores al leer el primer parámetro
            malformado = true;
        }
    }
    if (malformado) {
        puts(_("Error reading value"));
        while ((*c = getchar()) != '\n') ;
    }
}

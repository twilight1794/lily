/*
 * @file a_lexico_directivas.h
 * Algunas definiciones para reconocer directivas del preprocesador Lily
 */

#ifndef LILY_C_A_LEXICO_DIRECTIVAS
#define LILY_C_A_LEXICO_DIRECTIVAS

char* lex_directivas[] = {
    // Definición de constantes
    "DB",
    "DWL",
    "DDL",
    "DDM",
    "DQL",
    "DQM",
    "DR",
    "DRD",
    "DFS",
    // Definición de símbolos
    "CONST",
    "VAR",
    // Condicionales
    "IF",
    "IFDEF",
    "IFNDEF",
    "ELSE",
    "ELIF",
    // Bucles
    "WHILE",
    "LOOP",
    // Inclusión de archivos
    "INC",
    // Control de ensamblado
    "ORG",
    "STOP",
    // Estructuras de datos
    "STRUCT",
    "UNION",
    // Subrutinas
    "MACRO",
    "PROC",
    // Miscelánea
    "END",
    NULL
};

#endif
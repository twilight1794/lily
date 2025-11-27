#include "a_lexico.h"

void lex_modo_comentario(const char* blob, size_t* i) {
    do (*i)++;
    while (blob[*i] != '\n' && blob[*i] != 0);
}

int lex_lexico(const char* blob, struct LDE_LDE* simbolos) {
    // Variables a utilizar
    enum Lily_Error err;
    struct Lex_Simbolo* sim;
    struct LDE_Nodo* nodo;

    if (simbolos == NULL) return COD_MALLOC_FALLO;
    size_t i=0;
    // Modo fundamental
    do {
        if (blob[i] == ';') lex_modo_comentario(blob, &i);
        else if (blob[i] == '.') {
            // Es una directiva
            sim = Lex_Simbolo_Create();
            err = lex_modo_directiva(blob, &i, sim);
            if (err != COD_OK) return err;
            nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        }
        else if (blob[i] == '$') {
            // Es una referencia a etiqueta
            sim = Lex_Simbolo_Create();
            err = lex_modo_r_etiqueta(blob, &i, sim);
            if (err != COD_OK) return err;
            nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        }
        else if (blob[i] == '%') {
            // Es un objeto
            sim = Lex_Simbolo_Create();
            err = lex_modo_objeto(blob, &i, sim);
            if (err != COD_OK) return err;
            nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        }
        else if (lex_esblanco(blob[i])) while (lex_esblanco(blob[i])) i++; // Espacios en blanco
        else if (isalpha(blob[i])){
            // Es una etiqueta o directiva
            sim = Lex_Simbolo_Create();
            err = lex_modo_ambiguo(blob, &i, sim);
            if (err != COD_OK) return err;
            nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        }
        else if (blob[i] != 0) return COD_A_LEXICO_CARACTER_INVALIDO;
    } while (blob[i] != 0);
    return COD_OK;
}
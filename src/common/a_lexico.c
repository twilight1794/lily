#include "a_lexico.h"

struct Lex_Simbolo* Lex_Simbolo_Create() {
    return calloc(1, sizeof(struct Lex_Simbolo));
}

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
        if (blob[i] == ';') {
            // Es un comentario
            lex_modo_comentario(blob, &i);
            continue;
        }
        if (blob[i] == '.') {
            // Es una directiva
            err = lex_modo_directiva(blob, &i, &sim);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else if (err != COD_A_LEXICO_RECON_ERRONEO) return err;
        }
        if (blob[i] == '$') {
            // Es una referencia a etiqueta/variable
            err = lex_modo_r_etiqueta(blob, &i, &sim);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else if (err != COD_A_LEXICO_RECON_ERRONEO) return err;
        }
        if (blob[i] == '%') {
            // Es un objeto
            err = lex_modo_objeto(blob, &i, &sim);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else if (err != COD_A_LEXICO_RECON_ERRONEO) return err;
        }
        if (lex_esblanco(blob[i])) while (lex_esblanco(blob[i])) i++; // Espacios en blanco
        if (blob[i] == 0x27 || blob[i] == 0x22) {
            // Es una cadena
            err = lex_modo_cadena(blob, &i, &sim, blob[i]);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else if (err != COD_A_LEXICO_RECON_ERRONEO) return err;
        }
        if (blob[i] == '0' && blob[i + 1] == 'x') {
            // Es un número en otra base
            blob += 2;
            err = lex_modo_numero(blob, &i, &sim, blob[i-1]);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else if (err != COD_A_LEXICO_RECON_ERRONEO) return err;
        }
        if (isdigit(blob[i])) {
            // Es un número decimal
            err = lex_modo_numero(blob, &i, &sim, 0);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else if (err != COD_A_LEXICO_RECON_ERRONEO) return err;
        }
        if (lex_esoperador(blob[i])) {
            // Es probablemente un operador
            err = lex_modo_operador(blob, &i, &sim);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else if (err != COD_A_LEXICO_RECON_ERRONEO) return err;
        }
        if (isalpha(blob[i])){
            // Es un número hexadecimal xxxxh, mnemónico, etiqueta
            err = lex_modo_ambiguo(blob, &i, &sim);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else if (err != COD_A_LEXICO_RECON_ERRONEO) return err;
        }
        if (blob[i] != 0) return COD_A_LEXICO_CARACTER_INVALIDO;
    } while (blob[i] != 0);
    return COD_OK;
}
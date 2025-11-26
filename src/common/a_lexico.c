#include "a_lexico.h"

int exp_lexico(char* blob, struct LDE_LDE* simbolos) {
    enum Lily_Error error;
    if (simbolos == NULL) return COD_MALLOC_FALLO;
    size_t i=0;
    // Modo fundamental
    do {
        // Comentarios solos
        if (blob[i] == ';') modo_comentario(blob, &i);
        else if (blob[i] == ' ' || blob[i] == '\r' || blob[i] == '\t' || blob[i] == '\n'){
            i++;
        } else if (isalpha(blob[i])){
            struct Exp_Lex_Simbolo* sim = Exp_Lex_Simbolo_Create();
            err = modo_ambiguo(blob, &i, sim);
            if (err != COD_OK) return err;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        }
        else if (blob[i] != 0) return COD_A_LEXICO_CARACTER_INVALIDO;
    } while (blob[i] != 0);
    return COD_OK;
}
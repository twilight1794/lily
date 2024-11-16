#include "z80_lexico.h"

struct Z80_Lex_Simbolo* Z80_Lex_Simbolo_Create(){
    struct Z80_Lex_Simbolo* sim = (struct Z80_Lex_Simbolo*) malloc (sizeof(struct Z80_Lex_Simbolo));
    if (sim == NULL) return NULL;
    sim->tipo = SIMB_INDETERMINADO;
    sim->valor = NULL;
    sim->etiqueta = NULL;
    sim->expresion = NULL;
    return sim;
}

void modo_comentario(char* blob, size_t* i){
    char c = blob[*i];
    while (c != '\n' && c != 0){
        (*i)++;
        c = blob[*i];
    }
    return;
}

int modo_ambiguo(char* blob, size_t* i, struct Z80_Lex_Simbolo* sim){
    char c;
    bool puede_ser_etiqueta = true;
    bool puede_ser_instruccion = true;
    bool es_modo_expresion = false;
    char* tkn = Cadena_Create();
    tkn = Cadena_Add(tkn, &c);
    (*i)++;
    // En este punto, solo queremos caracteres alfanuméricos o _
    while (isalnum(c) || c == '_'){
        tkn = Cadena_Add(tkn, blob+*i);
        c = blob[*i];
        i++;
    }
    // Después, podemos esperar:
    while (c != '\n'){
        // Estamos en modo expresión
        if (es_modo_expresion && c != '\n'){
            if (sim->expresion == NULL){
                sim->expresion = Cadena_Create();
                if (sim->expresion == NULL) return COD_MALLOC_FALLO;
            }
            sim->expresion = Cadena_Add(sim->expresion, &c);
            (*i)++;
            c = blob[*i];
        }
        // Estamos en modo instrucción
        else if (!puede_ser_etiqueta && puede_ser_instruccion && isalpha(c)){
            if (sim->valor == NULL){
                sim->valor = Cadena_Create();
            }
            sim->valor = Cadena_Add(sim->valor, &c);
            (*i)++;
            c = blob[*i];
        // Dos puntos: era modo etiqueta, pasamos al modo instrucción
        } else if (puede_ser_etiqueta && c == ':'){
            puede_ser_etiqueta = false;
            sim->etiqueta = tkn;
            tkn = NULL;
            (*i)++;
            c = blob[*i];
        // Espacios fuera del modo expresión, ignorar
        } else if (c == ' ' || c == '\r' || c == '\t') ;
        // Otra vez caracteres:...
        else if (c != '\n'){
            // ...era modo instrucción, salimos de él
            if (puede_ser_instruccion){
                puede_ser_etiqueta = false;
                puede_ser_instruccion = false;
                sim->tipo = SIMB_INSTRUCCION;
                sim->valor = tkn;
                tkn = NULL;
            // ...entramos al modo expresión
            } else {
                es_modo_expresion = true;
            }
        } else if (c == '\n') break;
        else return COD_Z80_LEXICO_CARACTER_INVALIDO;
    }
    if (sim->tipo != SIMB_INSTRUCCION) sim->tipo = SIMB_ETI;
    return COD_OK;
}

int z80_lexico(char* blob, struct LDE_LDE* simbolos){
    int err;
    if (simbolos == NULL) return COD_MALLOC_FALLO;
    char c;
    size_t i=0;
    // Modo fundamental
    do {
        c = blob[i];
        if (c == ';') modo_comentario(blob, &i);
        else if (c == ' ' || c == '\r' || c == '\t' || c == '\n') {
            i++;
            continue;
        } else if (isalpha(c)){
            struct Z80_Lex_Simbolo* sim = Z80_Lex_Simbolo_Create();
            err = modo_ambiguo(blob, &i, sim);
            if (err) return err;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
        }
        else if (c != 0) return COD_Z80_LEXICO_CARACTER_INVALIDO;
    } while (c != 0);
    return COD_OK;
}

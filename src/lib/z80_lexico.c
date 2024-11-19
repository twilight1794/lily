#include "z80_lexico.h"
#include "exp_lexico.h"

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
    while (blob[*i] != '\n' && blob[*i] != 0){
        (*i)++;
    }
    return;
}

int modo_expresion(char* blob, size_t* i, struct Z80_Lex_Simbolo* sim){
    // Ignorar espacios iniciales
    while (blob[*i] == ' ' || blob[*i] == '\t' || blob[*i] == '\r') (*i)++;
    if (blob[*i] == '\n' || blob[*i] == 0) return COD_OK;
    // Si después hallamos una ;, es un comentario
    else if (blob[*i] == ';'){
        modo_comentario(blob, i);
        return COD_OK;
    }

    // Estamos listos para leer la expresión
    char* tmp = Cadena_Create();
    if (tmp == NULL) return COD_MALLOC_FALLO;
    // Capturar todo hasta el fin de línea
    while (blob[*i] != '\n' && blob[*i] != 0){
        tmp = Cadena_Add(tmp, blob+(*i));
        (*i)++;
    }

    // Tokenizar expresión
    sim->expresion = LDE_Create();
    if (sim->expresion == NULL) return COD_MALLOC_FALLO;
    int cod = exp_lexico(blob, i, sim->expresion);
    if (cod) return cod;
    return COD_OK;
}

int modo_instruccion(char* blob, size_t* i, struct Z80_Lex_Simbolo* sim){
    // Ignorar espacios iniciales
    while (blob[*i] == ' ' || blob[*i] == '\t' || blob[*i] == '\r') (*i)++;
    if (blob[*i] == '\n' || blob[*i] == 0) return COD_OK;
    // Si después hallamos una ;, es un comentario
    else if (blob[*i] == ';'){
        modo_comentario(blob, i);
        return COD_OK;
    }
    // Si después hallamos una no-letra, error
    else if (!isalpha(blob[*i])) return COD_Z80_LEXICO_CARACTER_INVALIDO;

    // Estamos listos para leer la instrucción
    if (sim->valor == NULL){
        sim->valor = Cadena_Create();
        if (sim->valor == NULL) return COD_MALLOC_FALLO;
    }
    sim->tipo = SIMB_INSTRUCCION;
    while (isalpha(blob[*i])){
        sim->valor = Cadena_Add(sim->valor, blob+(*i));
        (*i)++;
    }
    int err = COD_OK;
    if (blob[*i] == ';') modo_comentario(blob, i);
    else err = modo_expresion(blob, i, sim);
    return err;
}

int modo_ambiguo(char* blob, size_t* i, struct Z80_Lex_Simbolo* sim){
    int err = COD_OK;
    char* tkn = Cadena_Create();
    tkn = Cadena_Add(tkn, blob+(*i));
    (*i)++;
    // En este punto, solo queremos caracteres alfanuméricos o _
    while (isalnum(blob[*i]) || blob[*i] == '_'){
        tkn = Cadena_Add(tkn, blob+(*i));
        (*i)++;
    }
    // Después, podemos esperar varias cosas
    while (blob[*i] != '\n'){
        // Dos puntos: era modo etiqueta, pasamos al modo instrucción
        if (blob[*i] == ':'){
            sim->etiqueta = tkn;
            tkn = NULL;
            (*i)++;
            int err = modo_instruccion(blob, i, sim);
            if (err) break;
        // Espacios fuera del modo expresión, ignorar
        } else if (blob[*i] == ' ' || blob[*i] == '\r' || blob[*i] == '\t') (*i)++;
        // Otra vez caracteres: era modo instrucción, pasamos al modo expresión
        else if (blob[*i] != '\n'){
            sim->tipo = SIMB_INSTRUCCION;
            sim->valor = tkn;
            tkn = NULL;
            err = modo_expresion(blob, i, sim);
            if (err) break;
        }/* else {
            err = COD_Z80_LEXICO_CARACTER_INVALIDO;
            break;
        }*/
    }
    if (sim->tipo != SIMB_INSTRUCCION){
        // Era una etiqueta sin instrucción
        if (sim->etiqueta != NULL) sim->tipo = SIMB_ETI;
        // Era una instrucción sola
        else {
            sim->tipo = SIMB_INSTRUCCION;
            sim->valor = tkn;
        }
    }
    return err;
}

int z80_lexico(char* blob, struct LDE_LDE* simbolos){
    int err;
    if (simbolos == NULL) return COD_MALLOC_FALLO;
    size_t i=0;
    // Modo fundamental
    do {
        // Comentarios solos
        if (blob[i] == ';') modo_comentario(blob, &i);
        else if (blob[i] == ' ' || blob[i] == '\r' || blob[i] == '\t' || blob[i] == '\n'){
            i++;
            continue;
        } else if (isalpha(blob[i])){
            struct Z80_Lex_Simbolo* sim = Z80_Lex_Simbolo_Create();
            err = modo_ambiguo(blob, &i, sim);
            if (err) return err;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        }
        else if (blob[i] != 0) return COD_Z80_LEXICO_CARACTER_INVALIDO;
    } while (blob[i] != 0);
    return COD_OK;
}

#include "../lib/a_lexico.h"

struct Lex_Simbolo* lex_simbolo_create() {
    return calloc(1, sizeof(struct Lex_Simbolo));
}

void lex_modo_comentario(const char* blob, size_t* i) {
    do (*i)++;
    while (blob[*i] != '\n' && blob[*i] != 0);
}

enum Lily_Error lex_modo_directiva(const char* blob, size_t* i, struct Lex_Simbolo** sim) {
    const size_t i_inicial = *i;
    (*i)++; // Nos saltamos el punto

    // Obtener cadena a comparar
    char c = blob[*i];
    char* cad_tentativa = Cadena_Create();
    if (cad_tentativa == NULL) return COD_MALLOC_FALLO;
    while (isalpha(c)) {
        Cadena_Add(cad_tentativa, &c);
        (*i)++;
        c = blob[*i];
    }
    // Ver razón de fin: solo debe detenerse en blanco o fin de archivo
    if (!lex_esblanco(c) && c != 0) {
        *i = i_inicial;
        return COD_A_LEXICO_RECON_ERRONEO;
    }
    // Comparar por cada directiva posible
    const char* cad_directiva = lex_directivas[0];
    while (cad_directiva != 0) {
        if (!strcmp(cad_directiva, cad_tentativa)) {
            // Directiva encontrada
            *sim = lex_simbolo_create();
            if (*sim == NULL) {
                free(cad_tentativa);
                return COD_MALLOC_FALLO;
            }
            (*sim)->tipo = SIMB_DIRECTIVA;
            (*sim)->valor = cad_tentativa;
            return COD_OK;
        }
        cad_directiva++;
    }

    // No hubo resultados :(
    free(cad_tentativa);
    return COD_A_LEXICO_DIRECTIVA_INVALIDA;
}

enum Lily_Error lex_modo_r_etiqueta(const char* blob, size_t* i, struct Lex_Simbolo** sim) {
    const size_t i_inicial = *i;
    (*i)++; // Nos saltamos el $

    // Obtener cadena a comparar
    char c = blob[*i];
    char* cad_tentativa = Cadena_Create();
    if (cad_tentativa == NULL) return COD_MALLOC_FALLO;
    /// Primero, debe empezar por una letra o _
    if (!isalpha(c) && c != '_') {
        *i = i_inicial;
        free(cad_tentativa);
        return COD_A_LEXICO_CARACTER_INVALIDO;
    }
    Cadena_Add(cad_tentativa, &c);
    (*i)++;
    /// Ya después, puede ser cualquier letra, dígito o _
    while (isalpha(c) || isdigit(c) || c == '_') {
        Cadena_Add(cad_tentativa, &c);
        (*i)++;
        c = blob[*i];
    }
    // Dado a que esto solo debe usarse dentro de una expresión, pueden sucederle muchos tipos de símbolos, así que no hay comprobación de fin
    *sim = lex_simbolo_create();
    if (*sim == NULL) {
        free(cad_tentativa);
        return COD_MALLOC_FALLO;
    }
    (*sim)->tipo = SIMB_VARIABLE;
    (*sim)->valor = cad_tentativa;
    return COD_OK;
}

enum Lily_Error lex_modo_objeto(const char* blob, size_t* i, struct Lex_Simbolo** sim) {
    (*i)++;

    // Obtener cadena a comparar
    char c = blob[*i];
    char* cad_tentativa = Cadena_Create();
    if (cad_tentativa == NULL) return COD_MALLOC_FALLO;
    while (isalpha(c)) {
        Cadena_Add(cad_tentativa, &c);
        (*i)++;
        c = blob[*i];
    }
    // Dado a que esto solo debe usarse dentro de una expresión, pueden sucederle muchos tipos de símbolos, así que no hay comprobación de fin
    *sim = lex_simbolo_create();
    if (*sim == NULL) {
        free(cad_tentativa);
        return COD_MALLOC_FALLO;
    }
    (*sim)->tipo = SIMB_OBJETO;
    (*sim)->valor = cad_tentativa;
    return COD_OK;
}

enum Lily_Error lex_modo_cadena(const char* blob, size_t* i, struct Lex_Simbolo** sim, const char tipo) {
    const size_t i_inicial = *i;
    (*i)++; // Saltamos las comillas

    // Obtener contenido de la cadena
    // Las cadenas son agnósticas: todo lo que esté entre la primera comilla, y otra comilla de la misma clase, es considerado tal cual parte de la cadena, a excepción de los caracteres 0x0a y 0x00. Por ahora, tampoco hay secuencias de escape.
    char c = blob[*i];
    char* contenido = Cadena_Create();
    if (contenido == NULL) return COD_MALLOC_FALLO;
    while (c != 0x0a && c != 0 && c == blob[i_inicial]) {
        Cadena_Add(contenido, &c);
        (*i)++;
        c = blob[*i];
    }
    // Ver razón de fin
    if (c != blob[i_inicial]) {
        // O sea, nos interrumpieron sin cerrar la cadena
        free(contenido);
        return COD_A_LEXICO_FIN_INESPERADO;
    }
    // Terminamos la cadena bien
    *sim = lex_simbolo_create();
    if (*sim == NULL) {
        free(contenido);
        return COD_MALLOC_FALLO;
    }
    if (c == 0x27) (*sim)->tipo = SIMB_CADENA_SIMPLE;
    else (*sim)->tipo = SIMB_CADENA_NUL;
    (*sim)->valor = contenido;
    return COD_OK;
}

enum Lily_Error lex_modo_numero(const char* blob, size_t* i, struct Lex_Simbolo** sim, const char tipo) {
    const size_t i_inicial = *i;

    bool punto = false;
    char c = blob[*i];
    char* valor_texto = Cadena_Create();
    if (tipo == 16) {
        while (isxdigit(c)) {
            Cadena_Add(valor_texto, &c);
            (*i)++;
            c = blob[*i];
        }
    } else if (tipo == 8) {
        while (c > '0' && c <= '7') {
            Cadena_Add(valor_texto, &c);
            (*i)++;
            c = blob[*i];
        }
    } else if (tipo == 2) {
        while (c == '0' || c == '1') {
            Cadena_Add(valor_texto, &c);
            (*i)++;
            c = blob[*i];
        }
    } else {
        while (isdigit(c) || (c == '.' && !punto)) {
            if (c == '.') punto = true;
            Cadena_Add(valor_texto, &c);
            (*i)++;
            c = blob[*i];
        }
    }
    // Ver razón de fin: solo debe detenerse si hay después operador, en blanco o fin de archivo
    if (!lex_esblanco(c) && !lex_esoperador(c) && c != 0) {
        *i = i_inicial;
        return COD_A_LEXICO_CARACTER_INVALIDO;
    } else if (c == '.' && punto) {
        free(valor_texto);
        return COD_A_LEXICO_CARACTER_INVALIDO;
    }
    // Obtener valor
    long* valor = (long*) malloc(sizeof(long));
    for (size_t j = strlen(valor_texto); j > 0; j--) {
        *valor = valor_texto[j-1]*pow((tipo==0?10:tipo), strlen(valor_texto)-j);
    }
    *sim = lex_simbolo_create();
    if (*sim == NULL) {
        free(valor_texto);
        return COD_MALLOC_FALLO;
    }
    (*sim)->tipo = SIMB_NUMERO;
    (*sim)->valor = (void*) valor;
    free(valor_texto);
    return COD_OK;
}

int lex_lexico(const char* blob, struct LDE_LDE* simbolos) {
    // Variables a utilizar
    enum Lily_Error err;
    struct Lex_Simbolo* sim;
    struct LDE_Nodo* nodo;

    if (simbolos == NULL) return COD_MALLOC_FALLO;
    size_t i = 0;
    // Modo fundamental
    do {
        if (blob[i] == ';') {
            // Es un comentario
            lex_modo_comentario(blob, &i);
            continue;
        }
        if (blob[i] == '.') {
            // Es una directiva
            // NOTE: considerar en el futuro números decimales que comiencen por punto
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
            } else return err;
        }
        if (blob[i] == '%') {
            // Es un objeto
            err = lex_modo_objeto(blob, &i, &sim);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else return err;
        }
        if (lex_esblanco(blob[i])) while (lex_esblanco(blob[i])) i++; // Espacios en blanco
        if (blob[i] == 0x27 || blob[i] == 0x22) {
            // Es una cadena
            err = lex_modo_cadena(blob, &i, &sim, blob[i]);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else return err;
        }
        if (blob[i] == '0' && blob[i + 1] == 'x') {
            // Es un número en otra base
            i += 2;
            err = lex_modo_numero(blob, &i, &sim, blob[i-1]);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else return err;
        }
        if (isdigit(blob[i])) {
            // Es un número decimal
            err = lex_modo_numero(blob, &i, &sim, 0);
            if (err == COD_OK) {
                nodo = LDE_Insert(simbolos, LDE_Size(simbolos), (void*) sim);
                if (nodo == NULL) return COD_MALLOC_FALLO;
                continue;
            } else return err;
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
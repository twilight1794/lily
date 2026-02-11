#include "../lib/a_lexico.h"

struct lily_lex_simbolo* lily_lex_simbolo_create(void) {
    return calloc(1, sizeof(struct lily_lex_simbolo));
}

void lily_lex_modo_comentario(const char* blob, size_t* i) {
    do (*i)++;
    while (blob[*i] != '\n' && blob[*i] != 0);
    (*i)++;
}

enum lily_error lily_lex_modo_directiva(const char* blob, size_t* i, struct lily_lex_simbolo** sim) {
    // Comprobar antes si esto pudiera ser un número decimal
    if (isdigit(blob[(*i)+1])) return COD_A_LEXICO_RECON_ERRONEO;

    (*i)++; // Nos saltamos el punto

    // Obtener cadena a comparar
    char* cad_tentativa = lily_cadena_create();
    if (cad_tentativa == NULL) return COD_MALLOC_FALLO;
    while (isalpha(blob[*i])) {
        lily_cadena_add(cad_tentativa, blob+(*i));
        (*i)++;
    }
    // Ver razón de fin: solo debe detenerse en blanco o fin de archivo
    if (!strlen(cad_tentativa)) {
        if (blob[*i] == 0) return COD_A_LEXICO_FIN_INESPERADO;
        return COD_A_LEXICO_CARACTER_INVALIDO;
    }
    // Comparar por cada directiva posible
    for (size_t j = 0; lily_lex_directivas[j] != NULL; j++) {
        if (!strcmp(lily_lex_directivas[j], cad_tentativa)) {
            // Directiva encontrada
            *sim = lily_lex_simbolo_create();
            if (*sim == NULL) {
                free(cad_tentativa);
                return COD_MALLOC_FALLO;
            }
            (*sim)->tipo = SIMB_DIRECTIVA;
            (*sim)->valor = cad_tentativa;
            log_debug_gen(_("a_lexico: +directiva '%s'"), (char*) (*sim)->valor);
            return COD_OK;
        }
    }

    // No hubo resultados :(
    free(cad_tentativa);
    return COD_A_LEXICO_DIRECTIVA_INVALIDA;
}

enum lily_error lily_lex_modo_r_etiqueta(const char* blob, size_t* i, struct lily_lex_simbolo** sim) {
    (*i)++; // Nos saltamos el $

    // Obtener cadena a comparar
    char* cad_tentativa = lily_cadena_create();
    if (cad_tentativa == NULL) return COD_MALLOC_FALLO;
    /// Primero, debe empezar por una letra o _
    if (!isalpha(blob[*i]) && blob[*i] != '_') {
        free(cad_tentativa);
        return COD_A_LEXICO_CARACTER_INVALIDO;
    }
    lily_cadena_add(cad_tentativa, blob+(*i));
    (*i)++;
    /// Ya después, puede ser cualquier letra, dígito o _
    while (isalpha(blob[*i]) || isdigit(blob[*i]) || blob[*i] == '_') {
        lily_cadena_add(cad_tentativa, blob+(*i));
        (*i)++;
    }
    // Dado a que esto solo debe usarse dentro de una expresión, pueden sucederle muchos tipos de símbolos, así que no hay comprobación de fin
    *sim = lily_lex_simbolo_create();
    if (*sim == NULL) {
        free(cad_tentativa);
        return COD_MALLOC_FALLO;
    }
    (*sim)->tipo = SIMB_VARIABLE;
    (*sim)->valor = cad_tentativa;
    log_debug_gen(_("a_lexico: +variable '%s'"), (char*) (*sim)->valor);
    return COD_OK;
}

enum lily_error lily_lex_modo_objeto(const char* blob, size_t* i, struct lily_lex_simbolo** sim) {
    (*i)++;

    // Obtener cadena a comparar
    char* cad_tentativa = lily_cadena_create();
    if (cad_tentativa == NULL) return COD_MALLOC_FALLO;
    while (isalpha(blob[*i])) {
        lily_cadena_add(cad_tentativa, blob+(*i));
        (*i)++;
    }
    // Dado a que esto solo debe usarse dentro de una expresión, pueden sucederle muchos tipos de símbolos, así que no hay comprobación de fin
    *sim = lily_lex_simbolo_create();
    if (*sim == NULL) {
        free(cad_tentativa);
        return COD_MALLOC_FALLO;
    }
    (*sim)->tipo = SIMB_OBJETO;
    (*sim)->valor = cad_tentativa;
    log_debug_gen(_("a_lexico: +objeto '%s'"), (char*) (*sim)->valor);
    return COD_OK;
}

enum lily_error lily_lex_modo_cadena(const char* blob, size_t* i, struct lily_lex_simbolo** sim) {
    const char comilla_inicial = blob[*i];
    (*i)++; // Saltamos las comillas

    // Obtener contenido de la cadena
    // Las cadenas son agnósticas: todo lo que esté entre la primera comilla, y otra comilla de la misma clase, es considerado tal cual parte de la cadena, a excepción de los caracteres 0x0a y 0x00. Por ahora, tampoco hay secuencias de escape.
    char* contenido = lily_cadena_create();
    if (contenido == NULL) return COD_MALLOC_FALLO;
    while (blob[*i] != 0x0a && blob[*i] != 0 && blob[*i] != comilla_inicial) {
        lily_cadena_add(contenido, blob+(*i));
        (*i)++;
    }
    // Ver razón de fin
    if (blob[*i] != comilla_inicial) {
        // O sea, nos interrumpieron sin cerrar la cadena
        free(contenido);
        return COD_A_LEXICO_FIN_INESPERADO;
    }
    (*i)++;
    // Terminamos la cadena bien
    *sim = lily_lex_simbolo_create();
    if (*sim == NULL) {
        free(contenido);
        return COD_MALLOC_FALLO;
    }
    (*sim)->tipo = (comilla_inicial == 0x27)?SIMB_CADENA_SIMPLE:SIMB_CADENA_NUL;
    (*sim)->valor = contenido;
    log_debug_gen(_("a_lexico: +cadena %c%s%c"), comilla_inicial SEP (char*) (*sim)->valor SEP comilla_inicial);
    return COD_OK;
}

enum lily_error lily_lex_modo_numero(const char* blob, size_t* i, struct lily_lex_simbolo** sim, const char tipo) {
    bool punto = false;
    char* valor_texto = lily_cadena_create();
    if (tipo == 'x') {
        while (isxdigit(blob[*i])) {
            lily_cadena_add(valor_texto, blob+(*i));
            (*i)++;
        }
    } else if (tipo == 'o') {
        while (blob[*i] > '0' && blob[*i] <= '7') {
            lily_cadena_add(valor_texto, blob+(*i));
            (*i)++;
        }
    } else if (tipo == 'b') {
        while (blob[*i] == '0' || blob[*i] == '1') {
            lily_cadena_add(valor_texto, blob+(*i));
            (*i)++;
        }
    } else {
        while (isdigit(blob[*i]) || (blob[*i] == '.' && !punto)) {
            if (blob[*i] == '.') punto = true;
            lily_cadena_add(valor_texto, blob+(*i));
            (*i)++;
        }
    }
    // Ver razón de fin: solo debe detenerse si hay después operador, en blanco o fin de archivo
    if ((blob[*i] == '.' && punto) || (!lex_esblanco(blob[*i]) && !lex_esoperador(blob[*i]) && blob[*i] != 0)) {
        free(valor_texto);
        return COD_A_LEXICO_CARACTER_INVALIDO;
    }

    // Obtener valor
    long* valor = (long*) malloc(sizeof(long));
    *valor = 0;
    for (size_t j = strlen(valor_texto); j > 0; j--) {
        long potencia;
        const int exponente = (int) (strlen(valor_texto)-j); // FIX: no deberíamos aceptar números mayores a 20 dígitos en cualquier caso
        if (tipo == 'x') potencia = pow16(exponente);
        else if (tipo == 'o') potencia = pow8(exponente);
        else if (tipo == 'b') potencia = pow2(exponente);
        else potencia = pow10(exponente);
        long valor_digito;
        if (tipo == 'x' && valor_texto[j-1] >= 97) valor_digito = valor_texto[j-1] - 0x57;
        else if (tipo == 'x' && valor_texto[j-1] >= 65) valor_digito = valor_texto[j-1] - 0x37;
        else valor_digito = valor_texto[j-1] - 0x30;
        *valor += valor_digito * potencia;
    }
    *sim = lily_lex_simbolo_create();
    if (*sim == NULL) {
        free(valor_texto);
        return COD_MALLOC_FALLO;
    }
    (*sim)->tipo = SIMB_NUMERO;
    (*sim)->valor = (void*) valor;
    log_debug_gen(_("a_lexico: +número '%s'"), valor_texto);
    free(valor_texto);
    return COD_OK;
}

enum lily_error lily_lex_modo_operador(const char* blob, size_t* i, struct lily_lex_simbolo** sim) {
    // Determinar operador involucrado
    enum lily_lex_tipo_simbolo tipo = SIMB_INDETERMINADO;
    if (blob[*i] == '+') tipo = OP_SUMA;
    else if (blob[*i] == '-') tipo = OP_RESTA;
    else if (blob[*i] == '*') tipo = OP_MULTI;
    else if (blob[*i] == '/') tipo = OP_DIV;
    //OP_MODULO,
    else if (blob[*i] == '.') tipo = OP_MIEMBRO;
    else if (blob[*i] == '&') {
        if (blob[*(i+1)] == '&') {
            tipo = OP_LOG_AND;
            (*i)++;
        }
        else tipo = OP_BIT_AND;
    }
    else if (blob[*i] == '|') {
        if (blob[*(i+1)] == '|') {
            tipo = OP_LOG_OR;
            (*i)++;
        }
        else tipo = OP_BIT_OR;
    }
    else if (blob[*i] == '^') tipo = OP_BIT_XOR;
    else if (blob[*i] == '~') tipo = OP_BIT_NOT;
    else if (blob[*i] == '!') {
        if (blob[*(i+1)] == '=') {
            tipo = OP_DIF;
            (*i)++;
        }
        else tipo = OP_LOG_NEG;
    }
    else if (blob[*i] == '<') {
        if (blob[*(i+1) == '<']) {
            tipo = OP_DESP_IZQ;
            (*i)++;
        }
        else if (blob[*(i+1) == '=']) {
            tipo = OP_MENOR_IGUAL;
            (*i)++;
        }
        else tipo = OP_MENOR_QUE;
    }
    else if (blob[*i] == '>') {
        if (blob[*(i+1) == '>']) {
            tipo = OP_DESP_DER;
            (*i)++;
        }
        else if (blob[*(i+1) == '=']) {
            tipo = OP_MAYOR_IGUAL;
            (*i)++;
        }
        else tipo = OP_MAYOR_QUE;
    }
    else if (blob[*i] == '=') tipo = OP_IGUAL;
    else if (blob[*i] == ',') tipo = SIMB_SEPARADOR;
    else if (blob[*i] == '(') tipo = SIMB_PARENTESIS_AP;
    else if (blob[*i] == ')') tipo = SIMB_PARENTESIS_CI;
    // No debe haber más opciones, ¿verdad?
    (*i)++;

    // Crear objeto
    *sim = lily_lex_simbolo_create();
    if (*sim == NULL) return COD_MALLOC_FALLO;
    (*sim)->tipo = SIMB_OPERADOR;
    (*sim)->subtipo = tipo;
    log_debug_gen(_("a_lexico: +operador %d"), (*sim)->subtipo);
    return COD_OK;
}

enum lily_error lily_lex_modo_ambiguo(const char* blob, size_t* i, struct lily_lex_simbolo** sim) {
    // Obtener cadena a analizar
    char* cad_tentativa = lily_cadena_create();
    if (cad_tentativa == NULL) return COD_MALLOC_FALLO;
    bool tiene_guion = false;
    bool tiene_colon = false;
    while (isalpha(blob[*i]) || isdigit(blob[*i]) || blob[*i] == '_') {
        if (blob[*i] == '_') tiene_guion = true;
        lily_cadena_add(cad_tentativa, blob+(*i));
        (*i)++;
    }
    if (blob[*i] == ':') {
        tiene_colon = true;
        (*i)++;
    }
    // Excluir mnemónico inválido
    if (tiene_guion && !tiene_colon) {
        free(cad_tentativa);
        return COD_A_LEXICO_MNEMONICO_INVALIDO;
    }
    // Crear objeto
    *sim = lily_lex_simbolo_create();
    if (*sim == NULL) {
        free(cad_tentativa);
        return COD_MALLOC_FALLO;
    }
    (*sim)->tipo = tiene_colon?SIMB_ETI:SIMB_MNEMO;
    (*sim)->valor = cad_tentativa;
    log_debug_gen(_("a_lexico: +%s %s"), tiene_colon?"etiqueta":"mnemo" SEP (char*) (*sim)->valor);
    return COD_OK;
}

enum lily_error lily_lex_lexico(const char* blob, struct lily_lde_lde* simbolos, struct lily_lex_error_ctx* error_ctx) {
    // Variables a utilizar
    enum lily_error cod = COD_OK;
    enum lily_lex_tipo_simbolo tipo_tentativo;
    size_t i = 0;
    size_t i_inicial = 0;
    struct lily_lex_simbolo* sim;
    struct lily_lde_nodo* nodo;

    if (simbolos == NULL) return COD_MALLOC_FALLO;
    // Modo fundamental
    do {
        tipo_tentativo = SIMB_INDETERMINADO;
        if (blob[i] == ';') {
            // Es un comentario
            lily_lex_modo_comentario(blob, &i);
            continue;
        }
        if (blob[i] == '.') {
            // Es una directiva
            tipo_tentativo = SIMB_DIRECTIVA;
            i_inicial = i;
            cod = lily_lex_modo_directiva(blob, &i, &sim);
            if (cod == COD_OK) {
                nodo = lily_lde_insert(simbolos, lily_lde_size(simbolos), (void*) sim);
                if (nodo == NULL) {
                    cod = COD_MALLOC_FALLO;
                    break;
                }
                continue;
            }
            if (cod != COD_A_LEXICO_RECON_ERRONEO) break;
        }
        if (blob[i] == '$') {
            // Es una referencia a etiqueta/variable
            tipo_tentativo = SIMB_VARIABLE;
            i_inicial = i;
            cod = lily_lex_modo_r_etiqueta(blob, &i, &sim);
            if (cod == COD_OK) {
                nodo = lily_lde_insert(simbolos, lily_lde_size(simbolos), (void*) sim);
                if (nodo == NULL) {
                    cod = COD_MALLOC_FALLO;
                    break;
                }
                continue;
            }
            break;
        }
        if (blob[i] == '%') {
            // Es un objeto
            tipo_tentativo = SIMB_OBJETO;
            i_inicial = i;
            cod = lily_lex_modo_objeto(blob, &i, &sim);
            if (cod == COD_OK) {
                nodo = lily_lde_insert(simbolos, lily_lde_size(simbolos), (void*) sim);
                if (nodo == NULL) {
                    cod = COD_MALLOC_FALLO;
                    break;
                }
                continue;
            }
           break;
        }
        if (lex_esblanco(blob[i])) {
            // Espacios en blanco
            while (lex_esblanco(blob[i])) i++;
            continue;
        }
        if (blob[i] == 0x27 || blob[i] == 0x22) {
            // Es una cadena
            tipo_tentativo = (blob[i] == 0x27)?SIMB_CADENA_SIMPLE:SIMB_CADENA_NUL;
            i_inicial = i;
            cod = lily_lex_modo_cadena(blob, &i, &sim);
            if (cod == COD_OK) {
                nodo = lily_lde_insert(simbolos, lily_lde_size(simbolos), (void*) sim);
                if (nodo == NULL) {
                    cod = COD_MALLOC_FALLO;
                    break;
                }
                continue;
            }
            break;
        }
        if (blob[i] == '0' && (blob[i + 1] == 'x' || blob[i + 1] == 'o' || blob[i + 1] == 'b')) {
            // Es un número en otra base
            tipo_tentativo = SIMB_NUMERO;
            i_inicial = i;
            i += 2;
            cod = lily_lex_modo_numero(blob, &i, &sim, blob[i-1]);
            if (cod == COD_OK) {
                nodo = lily_lde_insert(simbolos, lily_lde_size(simbolos), (void*) sim);
                if (nodo == NULL) {
                    cod = COD_MALLOC_FALLO;
                    break;
                }
                continue;
            }
            break;
        }
        if (isdigit(blob[i]) || blob[i] == '.') {
            // Es un número decimal
            tipo_tentativo = SIMB_NUMERO;
            i_inicial = i;
            cod = lily_lex_modo_numero(blob, &i, &sim, 0);
            if (cod == COD_OK) {
                nodo = lily_lde_insert(simbolos, lily_lde_size(simbolos), (void*) sim);
                if (nodo == NULL) {
                    cod = COD_MALLOC_FALLO;
                    break;
                }
                continue;
            }
            break;
        }
        if (lex_esoperador(blob[i])) {
            // Es ~probablemente~ un operador
            tipo_tentativo = SIMB_OPERADOR;
            i_inicial = i;
            cod = lily_lex_modo_operador(blob, &i, &sim);
            if (cod == COD_OK) {
                nodo = lily_lde_insert(simbolos, lily_lde_size(simbolos), (void*) sim);
                if (nodo == NULL) {
                    cod = COD_MALLOC_FALLO;
                    break;
                }
                continue;
            }
            break;
        }
        if (isalpha(blob[i])){
            // Es un mnemónico o etiqueta
            tipo_tentativo = SIMB_ETI;
            i_inicial = i;
            cod = lily_lex_modo_ambiguo(blob, &i, &sim);
            if (cod == COD_OK) {
                nodo = lily_lde_insert(simbolos, lily_lde_size(simbolos), (void*) sim);
                if (nodo == NULL) {
                    cod = COD_MALLOC_FALLO;
                    break;
                }
                continue;
            }
            break;
        }
        if (blob[i] != 0) {
            cod = COD_A_LEXICO_CARACTER_INVALIDO;
            break;
        }
    } while (blob[i] != 0);

    if (cod != COD_OK && error_ctx != NULL) {
        error_ctx->tipo = tipo_tentativo;
        error_ctx->i_inicial = i_inicial;
        error_ctx->i_desp = i;
    }
    return cod;
}

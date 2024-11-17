#include "exp_lexico.h"

/*
 * Devuelve el nombre del tipo de dato usando su identificador
 * @param v Tipo de valor
 * @return Cadena correspondiente al operador
 */
char* exp_cadena_operador(enum LILY_EXP_TIPO_OP v){
    switch (v){
        case OP_SUMA:
        return "+";
        case OP_RESTA:
        return "-";
        case OP_MULTI:
        return "*";
        case OP_DIV:
        return "/";
        case OP_MODULO:
        return "%";
        case OP_MIEMBRO:
        return ".";
        case OP_BIT_AND:
        return "&";
        case OP_BIT_OR:
        return "|";
        case OP_BIT_XOR:
        return "^";
        case OP_COMP_2:
        return "-";
        case OP_COMP_1:
        return "~";
        case OP_LOG_AND:
        return "&&";
        case OP_LOG_OR:
        return "||";
        case OP_LOG_NEG:
        return "!";
        case OP_DESP_IZQ:
        return "<<";
        case OP_DESP_DER:
        return ">>";
        case OP_MENOR_QUE:
        return "<";
        case OP_MAYOR_QUE:
        return ">";
        case OP_MENOR_IGUAL:
        return "<=";
        case OP_MAYOR_IGUAL:
        return ">=";
        case OP_IGUAL:
        return "=";
        case OP_DIF:
        return "!=";
    }
    return NULL;
}

/* Analiza una expresión y genera los símbolos de la expresión
 * @param blob Cadena de texto que contiene una expresión en formato infijo
 * @param i Puntero al índice
 * @param simbolos Lista que va a almacenar los símbolos relevantes
 */
int exp_lexico(char* blob, size_t* i, struct LDE_LDE* simbolos){
    do {
        // Es un espacio
        if (blob[*i] == ' ' || blob[*i] == '\t') (*i)++;
        // Es un número
        else if (isdigit(blob[*i])){
        /// 16x: 0x\d
        /// 16x: \dh
        /// 10x: \d
        /// 8x: 0o\d
        /// 2x: 0b\d
        // Es un registro
        // Es una bandera
        // Es un registro o bandera C
        // Es un operador
        } else if (strchr("+-*/%.&|^~!<>=", blob[*i])){
            struct Exp_Simbolo* sim = (struct Exp_Simbolo*) malloc(sizeof(struct Exp_Simbolo)); // FIX: crear constructor
            if (sim == NULL) return COD_MALLOC_FALLO;
            sim->tipo = VAL_OP;
            sim->valor = Cadena_Create();
            sim->valor = Cadena_Add(sim->valor, blob+(*i));
            if (blob[*i] == '&' && blob[(*i)+1] == '&') sim->valor = Cadena_Add(sim->valor, "&");
            else if (blob[*i] == '|' && blob[(*i)+1] == '|') sim->valor = Cadena_Add(sim->valor, "|");
            else if (blob[*i] == '<' && blob[(*i)+1] == '<') sim->valor = Cadena_Add(sim->valor, "<");
            else if (blob[*i] == '>' && blob[(*i)+1] == '>') sim->valor = Cadena_Add(sim->valor, ">");
            if ((blob[*i] == '<' || blob[*i] == '>' || blob[*i] == '!') && blob[(*i)+1] == '=')
              sim->valor = Cadena_Add(sim->valor, "=");
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            if (!strcmp(sim->valor, "&&") || !strcmp(sim->valor, "||") ||
                !strcmp(sim->valor, "<<") || !strcmp(sim->valor, ">>") ||
                !strcmp(sim->valor, "<=") || !strcmp(sim->valor, ">=") ||
                !strcmp(sim->valor, "!=")) (*i)++;
            (*i)++;
        // Es una etiqueta
        } else if (isalpha(blob[*i]) || blob[*i] == '_'){
            struct Exp_Simbolo* sim = (struct Exp_Simbolo*) malloc(sizeof(struct Exp_Simbolo));
            if (sim == NULL) return COD_MALLOC_FALLO;
            sim->tipo = VAL_ETIQUETA;
            sim->valor = Cadena_Create();
            while (isalnum(blob[*i]) || blob[*i] == '_'){
                sim->valor = Cadena_Add(sim->valor, blob+(*i));
                (*i)++;
            }
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        // Es una cadena
        } else if (blob[*i] == '"'){
            struct Exp_Simbolo* sim = (struct Exp_Simbolo*) malloc(sizeof(struct Exp_Simbolo));
            if (sim == NULL) return COD_MALLOC_FALLO;
            sim->tipo = VAL_CADENA;
            sim->valor = Cadena_Create();
            (*i)++;
            while ((blob[*i] == 0 || blob[*i] == '\n') && (blob[*i] != '"')){
                sim->valor = Cadena_Add(sim->valor, blob+(*i));
                (*i)++;
            }
            (*i)++;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        // Es el inicio de un grupo
        } else if (blob[*i] == '['){
            struct Exp_Simbolo* sim = (struct Exp_Simbolo*) malloc(sizeof(struct Exp_Simbolo));
            if (sim == NULL) return COD_MALLOC_FALLO;
            sim->tipo = VAL_PARENTESIS_AP;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            (*i)++;
        // Es el final de un grupo
        } else if (blob[*i] == ']'){
            struct Exp_Simbolo* sim = (struct Exp_Simbolo*) malloc(sizeof(struct Exp_Simbolo));
            if (sim == NULL) return COD_MALLOC_FALLO;
            sim->tipo = VAL_PARENTESIS_CI;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            (*i)++;
        // Es el inicio de una indirección
        } else if (blob[*i] == '('){
            struct Exp_Simbolo* sim = (struct Exp_Simbolo*) malloc(sizeof(struct Exp_Simbolo));
            if (sim == NULL) return COD_MALLOC_FALLO;
            sim->tipo = VAL_DESP_AP;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            (*i)++;
        // Es el final de una indirección
        } else if (blob[*i] == ')'){
            struct Exp_Simbolo* sim = (struct Exp_Simbolo*) malloc(sizeof(struct Exp_Simbolo));
            if (sim == NULL) return COD_MALLOC_FALLO;
            sim->tipo = VAL_DESP_CI;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            (*i)++;
        // Es un separador de parámetros
        } else if (blob[*i] == ','){
            struct Exp_Simbolo* sim = (struct Exp_Simbolo*) malloc(sizeof(struct Exp_Simbolo));
            if (sim == NULL) return COD_MALLOC_FALLO;
            sim->tipo = VAL_SEPARADOR;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            (*i)++;
        } else return COD_CARACTER_INVALIDO;
    } while (blob[*i] != 0);
    return COD_OK;
}

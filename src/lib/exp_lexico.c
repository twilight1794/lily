#include "exp_lexico.h"

char* isregister(char* blob, size_t* i){
    char* cad = Cadena_Create();
    while (isupper(blob[*i])){
        cad = Cadena_Add(cad, blob+(*i));
        (*i)++;
    }
    if (!strcmp(cad, "A") || !strcmp(cad, "B") || !strcmp(cad, "D") || !strcmp(cad, "E") || !strcmp(cad, "H") || !strcmp(cad, "L") || !strcmp(cad, "HL") || !strcmp(cad, "AF") || !strcmp(cad, "BC") || !strcmp(cad, "DE") || !strcmp(cad, "IX") || !strcmp(cad, "IY")) return cad;
    (*i) -= strlen(cad);
    return NULL;
}

char* isflag(char* blob, size_t* i){
    char* cad = Cadena_Create();
    while (isupper(blob[*i])){
        cad = Cadena_Add(cad, blob+(*i));
        (*i)++;
    }
    if (!strcmp(cad, "NZ") || !strcmp(cad, "NC") || !strcmp(cad, "PO") || !strcmp(cad, "PE") || !strcmp(cad, "Z") || !strcmp(cad, "P") || !strcmp(cad, "M")) return cad;
    (*i) -= strlen(cad);
    return NULL;
}

long* isnumber(char* blob, size_t* i){
    char* cad = Cadena_Create();
    bool puede_ser_hex = false;
    bool es_hex = false;
    while (isxdigit(blob[*i]) || blob[*i] == 'h' || blob[*i] == 'H'){
        puede_ser_hex =  !isdigit(blob[*i]);
        es_hex = blob[*i] == 'h' || blob[*i] == 'H';
        if (es_hex) break;
        cad = Cadena_Add(cad, blob+(*i));
        (*i)++;
    }
    if (!(puede_ser_hex && !es_hex) && cad[0] != 0){
        long* tmp = (long *) malloc(sizeof(long));
        *tmp = strtol(cad, NULL, es_hex?16:10);
        free(cad);
        return tmp;
    }
    (*i) -= strlen(cad);
    return NULL;
}

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

int exp_lexico(char* blob, size_t* i, struct LDE_LDE* simbolos){
    do {
        struct Exp_Simbolo* sim = (struct Exp_Simbolo*) malloc(sizeof(struct Exp_Simbolo));
        if (sim == NULL) return COD_MALLOC_FALLO;

        // Es un espacio
        if (blob[*i] == ' ' || blob[*i] == '\t') (*i)++;
        // Es un registro
        else if ((sim->valor = isregister(blob, i), sim->valor != NULL)){
            sim->tipo = VAL_REGISTRO;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        // Es una bandera
        } else if ((sim->valor = isflag(blob, i), sim->valor != NULL)){
            sim->tipo = VAL_BANDERA;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        // Es un registro o bandera C
        } else if (blob[*i] == 'C'){
            sim->tipo = VAL_AMB_C;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        // Es un número 0x
        } else if (blob[*i] == '0' && blob[(*i)+1] == 'x' && isxdigit(blob[(*i)+2])){
            sim->tipo = VAL_NUMERO;
            char* tmp = Cadena_Create();
            (*i) += 2;
            while (isxdigit(blob[*i])){
                tmp = Cadena_Add(tmp, blob+(*i));
                (*i)++;
            }
            sim->valor = (long *) malloc(sizeof(long));
            *((long*) sim->valor) = strtol(tmp, NULL, 16);
            free(tmp);
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        // Es un número 0o
        } else if (blob[*i] == '0' && blob[(*i)+1] == 'o' && (blob[(*i)+2] >= 0x30 && blob[(*i)+2] <= 0x37)){
            sim->tipo = VAL_NUMERO;
            char* tmp = Cadena_Create();
            (*i) += 2;
            while (blob[*i] >= 0x30 && blob[*i] <= 0x37){
                tmp = Cadena_Add(tmp, blob+(*i));
                (*i)++;
            }
            sim->valor = (long *) malloc(sizeof(long));
            *((long*) sim->valor) = strtol(tmp, NULL, 8);
            free(tmp);
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        // Es un número 0b
        } else if (blob[*i] == '0' && blob[(*i)+1] == 'b' && (blob[(*i)+2] == '0' || blob[(*i)+2] == '1')){
            sim->tipo = VAL_NUMERO;
            char* tmp = Cadena_Create();
            (*i) += 2;
            while (blob[(*i)+2] == '0' || blob[(*i)+2] == '1'){
                tmp = Cadena_Add(tmp, blob+(*i));
                (*i)++;
            }
            sim->valor = (long *) malloc(sizeof(long));
            *((long *) sim->valor) = strtol(tmp, NULL, 2);
            free(tmp);
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        // Es un número hexadecimal o decimal
        } else if ((sim->valor = isnumber(blob, i), sim->valor != NULL)){
            sim->tipo = VAL_NUMERO;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        // Es un operador
        } else if (strchr("+-*/%.&|^~!<>=", blob[*i])){
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
            sim->tipo = VAL_CADENA;
            sim->valor = Cadena_Create();
            (*i)++;
            while (blob[*i] != 0 && blob[*i] != '\n' && blob[*i] != '"'){
                sim->valor = Cadena_Add(sim->valor, blob+(*i));
                (*i)++;
            }
            (*i)++;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
        // Es el inicio de un grupo
        } else if (blob[*i] == '['){
            sim->tipo = VAL_PARENTESIS_AP;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            (*i)++;
        // Es el final de un grupo
        } else if (blob[*i] == ']'){
            sim->tipo = VAL_PARENTESIS_CI;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            (*i)++;
        // Es el inicio de una indirección
        } else if (blob[*i] == '('){
            sim->tipo = VAL_DESP_AP;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            (*i)++;
        // Es el final de una indirección
        } else if (blob[*i] == ')'){
            sim->tipo = VAL_DESP_CI;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            (*i)++;
        // Es un separador de parámetros
        } else if (blob[*i] == ','){
            sim->tipo = VAL_SEPARADOR;
            struct LDE_Nodo* nodo = LDE_Insert(simbolos, LDE_Size(simbolos), sim);
            if (nodo == NULL) return COD_MALLOC_FALLO;
            (*i)++;
        // Era un comentario
        } else if (blob[*i] == ';'){
            while (blob[*i] != 0 && blob[*i] != '\n') (*i)++;
        } else return COD_CARACTER_INVALIDO;
    } while (blob[*i] != 0 && blob[*i] != '\n');
    return COD_OK;
}

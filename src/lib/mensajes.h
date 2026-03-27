/**
 * @file mensajes.h
 * Funciones para comunicación con el sistema cliente.
 */

#ifndef LILY_L_MENSAJES
#define LILY_L_MENSAJES

enum lily_lily_mensaje_tipo {
    LILY_MENSAJE_TLOG, /**< Mensajes de log */
    LILY_MENSAJE_TADVERTENCIA, /**< Situaciones definidas que requieren notificarse al usuario */
    LILY_MENSAJE_TETIQUETA, /**< Definiciones de etiquetas */
    LILY_MENSAJE_TVARIABLE, /**< Definiciones de variables y constantes */
    LILY_MENSAJE_TMEMORIA, /**< Lecturas y escrituras de memoria */
    LILY_MENSAJE_TREGISTRO, /**< Lecturas y escrituras de registros */
    LILY_MENSAJE_TPILA, /**< Operaciones sobre la pila, de estar definida */
    LILY_MENSAJE_TDISPOSITIVO, /**< E/S a dispositivos */
    LILY_MENSAJE_TINTERRUPCION, /**< Eventos de interrupciones */
};

typedef int (*f_mensajes_ptr)(enum lily_lily_mensaje_tipo, int, char*, void*);

#endif

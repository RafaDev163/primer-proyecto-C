#ifndef TAREAS_H
#define TAREAS_H

#include <stddef.h>   // size_t

//MACROS
//MACROS PARA ESCRITURA ARCHIVO BINARIO
#define ESCRITURA_OK   0
#define ERR_ARCHIVO    1
#define ERR_ESCRITURA  2
//MACROS PARA LECTURA DEL ARCHIVO BINARIO
#define LECTURA_OK     0
#define ERR_LECT_ARCH  1
#define ERR_LECT_FORM  2
#define ERR_LECT_MEM   3
//MACRO PARA EL TOTAL DE CARACTERES DE LA TAREA
#define MAX_SIZE_TAREA 100

// Estructuras para la ejecucion del programa (memoria)
typedef enum {
    TAREA_PENDIENTE = 0,
    TAREA_COMPLETADA = 1
} EstadoTarea;

typedef struct Tarea {
    char tarea[MAX_SIZE_TAREA];
    EstadoTarea estado;
    struct Tarea *next;
} Tarea;

// Estructura para el almacenamiento en disco de los datos
typedef struct {
    int32_t estado;               // fija el tamaño del enum en disco
    char    tarea[MAX_SIZE_TAREA];
} TareaOnDisk;

/* CRUD básico en lista simple */
int agregar_nueva_tarea(Tarea **head, const char *nueva_tarea);
void listar_tareas(const Tarea *head);
int marcar_tarea_por_indice(Tarea *head, size_t index, EstadoTarea estado);

/* Utilitarias */
void limpiar_buffer();
int liberar_lista(Tarea *head); //funcion usada en int leer_tareas_bin(Tarea **out_head, uint32_t *out_count, const char *nombre_archivo) para liberar listas leidas.
void liberar_tareas(Tarea **head); //funcion para liberar las tareas en main.c para que no haya riesgo de fuga o riesgo de usar puntero "colgado".
size_t contar_tareas(const Tarea *head); // funcion para contar todas las tareas (completas y pendientes)
size_t contar_por_estado(const Tarea *head, EstadoTarea estado);// funcion para contar tareas dependiendo el estado
void liberar_tareas(Tarea **head);// funcion para liberar memoria dinamica reservada para la lista de tareas

/* Persistencia (texto plano) */
int guardar_en_archivo(const Tarea *head, const char *nombre_archivo);
int cargar_desde_archivo(Tarea **head, const char *nombre_archivo);

/* Almacenamiento en disco .bin */
int escribir_tareas_bin(const Tarea *head, const char *nombre_archivo);
int leer_tareas_bin(Tarea **out_head, uint32_t *out_count, const char *nombre_archivo);


#endif

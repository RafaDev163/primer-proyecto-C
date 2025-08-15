#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tareas.h"

void limpiar_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int liberar_lista(Tarea *head){
    while (head) {
        Tarea *tmp = head->next;
        free(head);
        head = tmp;
    }
    return 0;
}

int agregar_nueva_tarea(Tarea **head, const char *nueva_tarea) {
    if (!head || !nueva_tarea || nueva_tarea[0] == '\0') {
        return -1; // argumentos inválidos
    }

    Tarea *nueva = malloc(sizeof *nueva);
    if (!nueva) {
        perror("malloc");
        return -2; // sin memoria
    }

    // Copia segura con truncado
    // snprintf garantiza terminación en '\0'
    snprintf(nueva->tarea, MAX_SIZE_TAREA, "%s", nueva_tarea);

    nueva->estado = TAREA_PENDIENTE;
    nueva->next   = *head;  // inserción al frente
    *head = nueva;
    return 0;
}

void listar_tareas(const Tarea *head) {
    size_t i = 0;
    for (const Tarea *ptr = head; ptr != NULL; ptr = ptr->next, ++i) {
        const char *marca = (ptr->estado == TAREA_COMPLETADA) ? "COMPLETE" : "PENDING";
        printf("[%zu] (%-9s) %s\n", i, marca, ptr->tarea);
    }
}

int marcar_tarea_por_indice(Tarea *head, size_t index, EstadoTarea estado) {
    size_t i = 0;
    for (Tarea *ptr = head; ptr != NULL; ptr = ptr->next, ++i) {
        if (i == index) {
            ptr->estado = estado;
            printf("La tarea No. %zu fue marcada como %s\n",
                   index,
                   (estado == TAREA_COMPLETADA) ? "completada" : "pendiente");
            return 0; // éxito
        }
    }
    printf("No se encontró la tarea con índice %zu\n", index);
    return 1; // no encontrado
}

int escribir_tareas_bin(const Tarea *head, const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "wb");
    if (!archivo) return ERR_ARCHIVO;

    // 1) contar registros
    uint32_t count = 0;
    for (const Tarea *ptr = head; ptr; ptr = ptr->next) ++count;

    // 2) escribir cantidad
    if (fwrite(&count, sizeof(count), 1, archivo) != 1) {
        fclose(archivo);
        return ERR_ESCRITURA;
    }

    // 3) escribir cada registro (sin el puntero next)
    for (const Tarea *ptr = head; ptr; ptr = ptr->next) {
        TareaOnDisk out = {0};
        out.estado = (int32_t)ptr->estado;
        // Copia segura garantizando terminación nula
        snprintf(out.tarea, sizeof(out.tarea), "%s", ptr->tarea);

        if (fwrite(&out, sizeof(out), 1, archivo) != 1) {
            fclose(archivo);
            return ERR_ESCRITURA;
        }
    }

    fclose(archivo);
    return ESCRITURA_OK;
}

int leer_tareas_bin(Tarea **out_head, uint32_t *out_count, const char *nombre_archivo){
    if (!out_head) return ERR_LECT_FORM;
    *out_head = NULL;
    if (out_count) *out_count = 0;

    FILE *archivo = fopen(nombre_archivo, "rb");
    if (!archivo) return ERR_LECT_ARCH;

    uint32_t count = 0;
    if (fread(&count, sizeof(count), 1, archivo) != 1) {
        fclose(archivo);
        return ERR_LECT_FORM;
    }

    Tarea *head = NULL, *tail = NULL;
    for (uint32_t i = 0; i < count; ++i) {
        TareaOnDisk in;
        if (fread(&in, sizeof(in), 1, archivo) != 1) {
            fclose(archivo);
            liberar_lista(head);
            return ERR_LECT_FORM;
        }

        Tarea *n = (Tarea *)malloc(sizeof(Tarea));
        if (!n) {
            fclose(archivo);
            liberar_lista(head);
            return ERR_LECT_MEM;
        }

        // reconstruir nodo en memoria
        snprintf(n->tarea, sizeof(n->tarea), "%s", in.tarea);
        n->estado = (EstadoTarea)in.estado;
        n->next = NULL;

        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
    }

    fclose(archivo);
    if (out_count) *out_count = count;
    *out_head = head;
    return LECTURA_OK;
}
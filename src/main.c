#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tareas.h"

int main() {
    printf("=== To-Do List ===\n");
    printf("(Proyecto Semana 1)\n");
    Tarea *head = NULL;
    int opc;
    char Tarea[MAX_SIZE_TAREA];
    uint32_t count = 0; // cuenta para la lista de tareas.
    int lectura = leer_tareas_bin(&head, &count, "tareas.bin");

    if(lectura == ERR_LECT_ARCH){
        // Primera ejecucion archivo inexistente, lista vacia, se creara el archivo cuando se agregue la primera tarea.
        printf("Archivo no existe, se creara uno nuevo (tareas.bin)\n");
    }else if (lectura != LECTURA_OK){
        printf ("Error leyendo el archivo");
        return 1;
    }else{
        printf("Tareas cargadas con exito!\n");
    }
    

    do
    {
        printf("Menu Principal\n");
        printf("Opciones:\n1.- Agregar Tarea.\n2.- Listar Tareas.\n3.- Marcar Tareas Completadas.\n4.- Salir\nOPCION: ");
        scanf("%d",&opc);
        switch (opc)
        {
        case 1:
                printf("\n");
                printf("Escriba la nueva tarea: ");
                limpiar_buffer();
                fgets(Tarea, sizeof(Tarea), stdin);
                Tarea[strcspn(Tarea, "\n")] = '\0';
                agregar_nueva_tarea(&head, Tarea);
                escribir_tareas_bin(head, "tareas.bin");
                printf("Tarea agregada con exito!\n");
                printf("\n");
            break;
        case 2:
                printf("\n");
                listar_tareas(head);
                printf("\n");
            break;
        case 3:
                size_t IDX;
                printf("\n");
                printf("Escribe el indice de la tarea que quieras marcar como completa: ");
                scanf("%zu", &IDX);
                marcar_tarea_por_indice(head, IDX, TAREA_COMPLETADA);
                escribir_tareas_bin(head, "tareas.bin");
                printf("HECHO!\n");
                printf("\n");
            break;
        case 4:
                printf("SALIENDO...\n");
            break;
        default:
                printf("OPCION INVALIDA, INTENTE DE NUEVO.!\n");
                printf("\n");
            break;
        }
    } while (opc != 4);
    // Aquí irá el menú principal y llamadas a funciones
    liberar_lista(head);

    return 0;
}

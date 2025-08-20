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
    
    uint32_t count = 0; // cuenta para la lista de tareas.
    int lectura = leer_tareas_bin(&head, &count, "tareas.bin");

    if(lectura == ERR_LECT_ARCH){
        // Primera ejecucion archivo inexistente, lista vacia, se creara el archivo cuando se agregue la primera tarea.
        printf("Archivo no existe, se creara uno nuevo (tareas.bin)\n");
    }else if (lectura != LECTURA_OK){
        fprintf (stderr, "Error leyendo el archivo");
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
        case 1: {
                char tarea[MAX_SIZE_TAREA];
                printf("\n");
                printf("Escriba la nueva tarea: ");
                limpiar_buffer();
                if(fgets(tarea, sizeof(tarea), stdin)){
                    tarea[strcspn(tarea, "\n")] = '\0';
                    if(tarea[0] != '\0') agregar_nueva_tarea(&head, tarea);
                }
                escribir_tareas_bin(head, "tareas.bin");
                printf("Tarea agregada y guardada con exito!\n");
                printf("\n");
                }         
            break;
        case 2: {
                printf("\n");
                listar_tareas(head);
                size_t tot = contar_total(head);
                size_t comp = contar_por_estado(head, TAREA_COMPLETADA);
                printf("Total: %-10zu | Pendientes: %-10zu | Completadas: %-10zu\n", tot, tot-comp, comp);
                printf("\n");
                }   
            break;
        case 3: {
                size_t IDX;
                printf("\n");
                printf("Escribe el indice de la tarea que quieras marcar como completa: ");
                scanf("%zu", &IDX);
                int ok = marcar_tarea_por_indice(head, IDX, TAREA_COMPLETADA);
                if(ok != 0) {
                    fprintf(stderr, "Indice %zu invalido. \n", IDX);
                }
                escribir_tareas_bin(head, "tareas.bin");
                printf("Lista actualizada correctamente!\n");
                printf("\n");
                }       
            break;
        case 4:
                escribir_tareas_bin(head, "tareas.bin");
                printf("SALIENDO...\n");
            break;
        default:
                printf("OPCION INVALIDA, INTENTE DE NUEVO.!\n");
                printf("\n");
            break;
        }
    } while (opc != 4);
  
    liberar_tareas(&head);

    return 0;
}

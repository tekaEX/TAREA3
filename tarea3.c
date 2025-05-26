#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura de un ítem
typedef struct Item {
    char nombre[64];
    int puntos;
    int peso;
} Item;

// Estructura de un escenario
typedef struct Escenario {
    int id;
    char nombre[128];
    char descripcion[256];
    List *items;
    int es_final;
    struct Escenario *arriba, *abajo, *izquierda, *derecha;
    int arriba_id, abajo_id, izquierda_id, derecha_id;
} Escenario;

Map *grafo;
List *inventario;
int puntos = 0, peso = 0, tiempo = 10;

int is_equal_int(void *a, void *b) {
    return *((int*)a) == *((int*)b);
}

// Crea un Item a partir de "nombre,puntos,peso"
Item* crear_item(const char *str) {
    List *valores = split_string(str, ",");
    if (!valores || list_size(valores) < 3) return NULL;
    Item *item = malloc(sizeof(Item));
    char *n = list_first(valores);
    char *pts = list_next(valores);
    char *ps = list_next(valores);
    strcpy(item->nombre, n ? n : "");
    item->puntos = pts ? atoi(pts) : 0;
    item->peso   = ps  ? atoi(ps)  : 0;
    list_clean(valores);
    free(valores);
    return item;
}

// Crea un Escenario desde campos CSV
Escenario* crear_escenario(char **campos) {
    Escenario *e = malloc(sizeof(Escenario));
    e->id = atoi(campos[0]);
    strcpy(e->nombre, campos[1]);
    strcpy(e->descripcion, campos[2]);

    e->items = list_create();
    List *raw = split_string(campos[3], ";");
    for (char *it = list_first(raw); it; it = list_next(raw)) {
        Item *it_p = crear_item(it);
        if (it_p) list_pushBack(e->items, it_p);
    }
    list_clean(raw);
    free(raw);

    e->arriba_id    = atoi(campos[4]);
    e->abajo_id     = atoi(campos[5]);
    e->izquierda_id = atoi(campos[6]);
    e->derecha_id   = atoi(campos[7]);
    e->es_final     = atoi(campos[8]);
    e->arriba = e->abajo = e->izquierda = e->derecha = NULL;
    return e;
}

// Lee CSV y almacena escenarios en el grafo
void leer_escenarios() {
    grafo = map_create(is_equal_int);
    FILE *f = fopen("data/graphquest.csv", "r");
    if (!f) { perror("Error al abrir archivo"); return; }

    char **campos = leer_linea_csv(f, ','); // encabezado
    if (campos) free(campos);
    while ((campos = leer_linea_csv(f, ','))) {
        Escenario *e = crear_escenario(campos);
        if (e) {
            int *key = malloc(sizeof(int)); *key = e->id;
            map_insert(grafo, key, e);
        }
        free(campos);
    }
    fclose(f);
}

// Conecta punteros de escenarios según IDs
void conectar_escenarios() {
  MapPair *p = map_first(grafo);
  while (p) {
    Escenario *e = p->value;
    // Arriba
    if (e->arriba_id != -1) {
      int *key = malloc(sizeof(int));
      *key = e->arriba_id;
      MapPair *mp = map_search(grafo, key);
      if (mp) e->arriba = mp->value;
      else e->arriba = NULL;
      free(key);
    } else {
      e->arriba = NULL;
    }
    // Abajo
    if (e->abajo_id != -1) {
      int *key = malloc(sizeof(int));
      *key = e->abajo_id;
      MapPair *mp = map_search(grafo, key);
      if (mp) e->abajo = mp->value;
      else e->abajo = NULL;
      free(key);
    } else {
      e->abajo = NULL;
    }
    // Izquierda
    if (e->izquierda_id != -1) {
      int *key = malloc(sizeof(int));
      *key = e->izquierda_id;
      MapPair *mp = map_search(grafo, key);
      if (mp) e->izquierda = mp->value;
      else e->izquierda = NULL;
      free(key);
    } else {
      e->izquierda = NULL;
    }
    // Derecha
    if (e->derecha_id != -1) {
      int *key = malloc(sizeof(int));
      *key = e->derecha_id;
      MapPair *mp = map_search(grafo, key);
      if (mp) e->derecha = mp->value;
      else e->derecha = NULL;
      free(key);
    } else {
      e->derecha = NULL;
    }
    p = map_next(grafo);
  }
}

// Muestra inventario\ n
void imprimir_inventario() {
    printf("\n--- Inventario del Jugador ---\n");
    for (Item *it = list_first(inventario); it; it = list_next(inventario)) {
        printf("- %s (%d pts, %d kg)\n", it->nombre, it->puntos, it->peso);
    }
    printf("Total Puntos: %d, Total Peso: %d\n", puntos, peso);
    printf("Tiempo restante: %d\n", tiempo);
    puts("------------------------------\n");
}

// Imprime escenario actual
void imprimirEscenario(Escenario *e) {
    if (!e) { printf("Escenario nulo\n"); return; }
    puts("========================================");
    puts("             ESTADO ACTUAL");
    puts("========================================");
    printf("ID: %d\nNombre: %s\nDescripcion: %s\n", e->id, e->nombre, e->descripcion);
    if (e->es_final) printf("[Escenario Final]\n");
    printf("Items:");
    if (!list_first(e->items)) printf(" vacio.\n");
    else {
        for (Item *it = list_first(e->items); it; it = list_next(e->items)) {
            printf("  - %s (%d pts, %d kg)\n", it->nombre, it->puntos, it->peso);
        }
        puts("Inventario actual:");
        imprimir_inventario();
    }
}

// Imprime conexiones disponibles
void imprimirConexiones(Escenario *e) {
    printf("Conexiones:\n");
    if (e->arriba)    printf("1) Arriba: %d\n", e->arriba->id);
    if (e->abajo)     printf("2) Abajo: %d\n", e->abajo->id);
    if (e->izquierda) printf("3) Izquierda: %d\n", e->izquierda->id);
    if (e->derecha)   printf("4) Derecha: %d\n", e->derecha->id);
}

// Recoge items del escenario
void recoger_items(Escenario *e) {
    for (Item *it = list_first(e->items); it; it = list_next(e->items)) {
        puntos += it->puntos;
        peso   += it->peso;
        list_pushBack(inventario, it);
    }
    list_clean(e->items);
    tiempo--;
    printf("Items recogidos.\n");
}

// Descarta items del escenario
void descartar_items(Escenario *e) {
    list_clean(e->items);
    tiempo--;
    printf("Items descartados.\n");
}

int main() {
    printf("---------------------------\nLECTURA DE ESCENARIOS\n---------------------------\n");
    leer_escenarios();
    conectar_escenarios();
    MapPair *inicio = map_first(grafo);
    Escenario *actual = inicio->value;
    inventario = list_create();
    char opcion;
    do {
        imprimirEscenario(actual);
        puts("\n========================================");
        puts("     Que accion quieres realizar?");
        puts("========================================");
        puts("1) Recoger Item(s)");
        puts("2) Descartar Item(s)");
        puts("3) Avanzar a otra direccion");
        puts("4) Reiniciar partida");
        puts("5) Salir");
        printf("Ingrese su opcion: ");
        scanf(" %c", &opcion);
        switch (opcion) {
            case '1': recoger_items(actual); break;
            case '2': descartar_items(actual); break;
            case '3':
                imprimirConexiones(actual);
                printf("Ingrese dirección (1-4): ");
                scanf(" %c", &opcion);
                if (opcion=='1' && actual->arriba)    actual = actual->arriba;
                if (opcion=='2' && actual->abajo)     actual = actual->abajo;
                if (opcion=='3' && actual->izquierda) actual = actual->izquierda;
                if (opcion=='4' && actual->derecha)   actual = actual->derecha;
                break;
            case '4':
                inicio = map_first(grafo);
                actual = inicio->value;
                puntos = peso = 0;
                tiempo = 10;
                list_clean(inventario);
                break;
        }
        presioneTeclaParaContinuar(); limpiarPantalla();
    } while (opcion != '5');
    return 0;
}

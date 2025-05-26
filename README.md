# Tarea 3 - Juego de Aventuras por Escenarios

## a. ¿Cómo compilar y ejecutar la tarea?

1. **Compilar:**
   Abre una terminal en la raíz del proyecto y ejecuta:
   ```sh
   gcc tarea3.c tdas/list.c tdas/map.c tdas/extra.c -o tarea3.exe
   ```
   > Asegúrate de tener los archivos fuente y la carpeta `tdas/` en el mismo directorio.

2. **Ejecutar:**
   ```sh
   ./tarea3.exe
   ```
   El programa leerá los escenarios desde `data/graphquest.csv`.

---

## b. Opciones que funcionan correctamente y las que no lo hacen (indicando posibles causas)

**Opciones que funcionan correctamente:**
- 1) Recoger Item(s): Recoge todos los ítems del escenario y los agrega al inventario.
- 2) Descartar Item(s): Descarta todos los ítems del escenario actual.
- 3) Avanzar a otra dirección: Permite moverse entre escenarios conectados (arriba, abajo, izquierda, derecha).
- 4) Reiniciar partida: Reinicia el juego desde el primer escenario, vaciando el inventario y reiniciando puntos, peso y tiempo.
- 5) Salir: Termina la aplicación.

**Posibles problemas:**
- Si el archivo `data/graphquest.csv` no existe o está mal formateado, el programa no podrá cargar los escenarios.
- Si se ingresa una dirección inválida al avanzar, no se realiza ningún movimiento.
- El tiempo se calcula como `(peso + 1) / 10`, lo que puede no reflejar correctamente el avance si el peso es bajo.
- Calculo con variables que estén vacías, en casos en que sea null y recoges igualmente el ítem


---

## c. Ejemplo de interacción con la aplicación

```
---------------------------
LECTURA DE ESCENARIOS
---------------------------
========================================
             ESTADO ACTUAL
========================================
ID: 1
Nombre: Entrada
Descripcion: Comienzo de la aventura
Items:  - Espada (10 pts, 5 kg)
Inventario actual:

--- Inventario del Jugador ---
Total Puntos: 0, Total Peso: 0
Tiempo restante: 10
------------------------------

========================================
     Que accion quieres realizar?
========================================
1) Recoger Item(s)
2) Descartar Item(s)
3) Avanzar a otra direccion
4) Reiniciar partida
5) Salir
Ingrese su opcion: 1
Items recogidos.

Presione una tecla para continuar...

========================================
             ESTADO ACTUAL
========================================
ID: 1
Nombre: Entrada
Descripcion: Comienzo de la aventura
Items: vacio.
Inventario actual:

--- Inventario del Jugador ---
- Espada (10 pts, 5 kg)
Total Puntos: 10, Total Peso: 5
Tiempo restante: 10
------------------------------
```

---

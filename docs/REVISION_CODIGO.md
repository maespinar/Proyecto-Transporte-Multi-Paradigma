# Revisión del código del grupo

Revisión del repositorio tomado desde `master`, commit `5cd0824`, y del aporte de Diego implementado en la rama local `codex/diego-chuquiure`. Los siguientes hallazgos indican qué falta integrar o corregir en el proyecto grupal.

## Hallazgos que conviene resolver primero

### 1. La declaración y definición de asignarPedidoUnidad no coinciden

En `cpp/Gestion-Estados/asignar-pedido-unidad.h:7` la unidad se recibe como `const Unidad&`, pero en `asignar-pedido-unidad.cpp:3` se define con `Unidad&`. Son funciones distintas para C++.

Se comprobó compilando un programa que incluye la cabecera y llama a la función: el enlazador devuelve `undefined reference to asignarPedidoUnidad(Pedido&, Unidad const&, ...)`. La cabecera debe declarar una referencia modificable, porque la función cambia el estado del bus. El menú actual no la llama y por eso este error no aparece al compilar solamente el menú.

Además, las tres cabeceras de esa carpeta incluyen `"structures.h"`, aunque el archivo está en `cpp/data/`. Compilar directamente esos `.cpp` sin `-Icpp/data` falla con `structures.h: No such file or directory`. Se puede corregir la ruta relativa o incluir esa carpeta al compilar.

### 2. Falta conectar la gestión de estados al menú y a los CSV

Las funciones para habilitar/cerrar conexiones, asignar normalmente un pedido y marcarlo como entregado existen en `cpp/Gestion-Estados/`, pero todavía no se invocan desde el menú. Esas funciones tampoco guardan archivos.

El soporte agregado por Diego guarda el registro y los transbordos. Para terminar 4.1.4 y 4.1.5, el responsable debe conectar las otras operaciones al menú y persistir los vectores afectados después de validar sus cambios.

En `asignar-pedido-unidad.cpp:7` se copia la unidad dentro del pedido **antes** de cambiar el bus a `OCUPADA`, por lo que esa copia queda con un estado distinto al de la flota. Conviene copiar después de actualizarlo. También se reemplaza `horaSolicitud` al asignar: ese campo representa la hora de creación, según la propuesta de demanda por franja horaria, y debería conservarse.

### 3. La entrega actual solo libera el primer bus

`cpp/Gestion-Estados/marcar-pedido.cpp:7` y `:8` liberan la unidad principal. Cuando se conecte esta operación al menú, debe liberar también `unidadTransbordo` y su unidad correspondiente en el vector de flota. De lo contrario, un viaje de dos etapas dejaría el segundo bus ocupado permanentemente.

Esta extensión corresponde a la integración entre la gestión de estados y el nuevo transbordo. La opción 9 reserva y guarda correctamente ambos buses, pero no agrega una segunda operación de entrega que compita con la del responsable de 4.1.4.

### 4. El cálculo existente de distancia no garantiza el recorrido más corto en kilómetros

`cpp/emisiones.hpp`, en la funcion `calcularDistanciaRuta`, utiliza una cola BFS y devuelve el primer destino alcanzado. BFS minimiza cantidad de tramos en una red sin pesos; aquí cada tramo tiene una distancia diferente.

Se probó con A–B de 100 km, A–C de 1 km y C–B de 1 km: devuelve `100`, aunque existe un recorrido de 2 km. Si se quiere evaluar el recorrido de menor distancia, debe usarse una búsqueda que compare kilómetros, como Dijkstra con pesos positivos, o calcular la distancia de una ruta elegida explícitamente. El cálculo de CO2 depende de esa decisión.

### 5. Los datos de prueba contienen casos que necesitan explicación o ajuste

Los archivos originales tienen 47 puntos, 46 conexiones, 12 unidades y 14 pedidos. Cumplen las cantidades mínimas, incluyen un cierre y representan los tres estados de pedido.

Sin embargo:

- P14–P15 está cerrado y es el único enlace entre dos partes de la red. No existe una alternativa física entre ellas. Es necesario agregar algún desvío válido si el grupo quiere demostrar un transbordo exitoso sobre los datos reales. La demostración separada de `tests/data/` ya cubre ese caso sin alterar los originales.
- S002 y S003 figuran asignados y atraviesan ese corte. Podrían representar un cierre ocurrido después del despacho, pero el sistema todavía no documenta ni gestiona esa situación.
- S008 contiene 45 pasajeros y U06 como segunda unidad, cuya capacidad es 36. Un transbordo secuencial transporta al grupo completo en cada etapa y ese dato no cumple la capacidad necesaria. La nueva asignación rechaza este caso.

## Funcionalidades base pendientes del grupo

Esta lista se basa en las funciones y consultas encontradas, no en una evaluación de la participación de cada compañero.

| Apartados | Situación del código revisado |
| --- | --- |
| C++ 4.1.1 y 4.1.2 | Menú y carga inicial presentes; el menú necesita incorporar el resto de operaciones grupales. |
| C++ 4.1.3 | Implementado en el aporte de Diego, con guardado de los registros. |
| C++ 4.1.4 y 4.1.5 | Gestión existente sin conexión al menú; resolver las firmas y conectar persistencia para esas operaciones. |
| C++ 4.1.6 y 4.1.7 | Hay listados generales y conteos; faltan consultas por punto, estado y unidad y el reporte completo pedido. |
| Haskell 4.2.1 y 4.2.2 | Lectura y tipos presentes. |
| Haskell 4.2.3 y 4.2.4 | Implementados en el aporte de Diego. |
| Haskell 4.2.5, 4.2.6 y 4.2.7 | Faltan costos de rutas, reporte de pedidos y resumen completo exigido. |
| Prolog 4.3.1 | Lectura presente. |
| Prolog 4.3.2 y 4.3.3 | Implementados en el aporte de Diego. |
| Prolog 4.3.4, 4.3.5 y 4.3.6 | Faltan `ruta_evitando`, `puede_atender` y `pedidos_inaccesibles`. |

Los lectores originales en C++ y Haskell asumen CSV bien formados y pueden fallar si se editan manualmente con columnas incompletas o números inválidos. El nuevo registro produce el formato esperado; un tratamiento de archivos dañados sería una mejora adicional del grupo.

## Comprobaciones del aporte de Diego

- Pruebas C++ de registro y asignación: duplicados, puntos desconocidos, tramos inversos, valores inválidos, capacidades, estados y tiempos del plan.
- Prueba de integración: menú real, registro de las cuatro entidades, conservación de campos adicionales, guardado, recarga, entradas inválidas, EOF y un fallo de lectura previa sin cambiar originales.
- Integración C++/Prolog real: ruta de 14 + 10 minutos y espera de 5; reserva de U01/U02, persistencia de ambas unidades y rechazos sin cambios cuando no hay ruta o capacidad.
- 10 pruebas Haskell: vecinos bidireccionales, cierres, duplicados, rutas alternativas, ciclos, puntos desconocidos y red vacía.
- 16 pruebas Prolog: conexión, alcanzabilidad, cierre, aislamiento, transbordo óptimo, recorrido inverso, ausencia de infraestructura de intercambio y selección del mínimo aunque la ruta lenta se encuentre primero.
- Verificación en Windows con MinGW y SWI-Prolog, y en Linux/WSL con G++, GHC y SWI-Prolog.
- `git diff --check` y comprobación de que `data/` conserva los archivos del repositorio original.

Las instrucciones para repetir las pruebas están en el README. Los casos artificiales son independientes de los CSV del grupo. La entrega se organiza en un commit por funcionalidad del aporte de Diego.

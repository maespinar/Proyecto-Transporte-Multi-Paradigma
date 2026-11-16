# Sistema de Transporte Urbano del Corredor Rojo

Proyecto de Paradigmas de Programación: C++ registra y gestiona datos, Haskell trabaja con funciones puras sobre la red y Prolog deduce su conectividad. Los módulos comparten los archivos de `data/`.

## Aporte de Diego Alejandro Chuquiure Vargas

| Requisito de la propuesta | Implementación |
| --- | --- |
| C++ 4.1.3: registro con validaciones | `cpp/Registro/registro.h` y `cpp/Registro/registro.cpp`, opción 8 del menú |
| Haskell 4.2.3: vecinos habilitados en ambos sentidos | `vecinos` en `haskell/Main.hs` |
| Haskell 4.2.4: todas las rutas sin repetir puntos | `rutasPosibles` en `haskell/Main.hs` |
| Prolog 4.3.2: conexión directa habilitada | `conectado/3` en `prolog/rutas.pl` |
| Prolog 4.3.3: alcanzabilidad | `alcanzable/2` en `prolog/rutas.pl` |
| Adicional: transbordos ante interrupciones viales | `cpp/Transbordos/transbordos.h` y `cpp/Transbordos/transbordos.cpp`, `prolog/rutas.pl`, `prolog/consulta_transbordo.pl`, opción 9 |

Los registros y la asignación de transbordos guardan sus cambios en CSV mediante `cpp/data/persistencia.h` y `cpp/data/persistencia.cpp`. Este soporte permite utilizar los datos desde los otros lenguajes; las restantes operaciones de gestión de estados del grupo todavía deben conectarse al menú y al guardado. El detalle de la revisión está en [docs/REVISION_CODIGO.md](docs/REVISION_CODIGO.md).

## Requisitos y ejecución

- Compilador C++11 para el programa. Las pruebas de integracion usan C++17 para preparar sus carpetas de datos.
- GHC para ejecutar o compilar Haskell. Solo se utilizan módulos de `base`.
- [SWI-Prolog](https://www.swi-prolog.org/download/stable) para Prolog y para la opción de transbordos de C++.

Ejecutar los siguientes comandos desde la raíz del repositorio. Las bibliotecas usadas en Prolog (`csv`, `plunit`) vienen con SWI-Prolog.

### C++

En este equipo se puede iniciar directamente con `./ejecutar.ps1`. El script busca el compilador y SWI-Prolog, compila el menú y configura sus rutas solo durante esa ejecución. Acepta `-CarpetaDatos build/demo`, `-Compilador <ruta>` y `-Prolog <ruta>`. No cambia la configuración permanente de Windows.

```powershell
g++ -std=c++11 cpp/main.cpp cpp/Registro/registro.cpp cpp/Transbordos/transbordos.cpp cpp/data/structures.cpp cpp/data/persistencia.cpp -o transporte.exe
.\transporte.exe
```

En Linux:

```bash
mkdir -p build
g++ -std=c++11 cpp/main.cpp cpp/Registro/registro.cpp cpp/Transbordos/transbordos.cpp cpp/data/structures.cpp cpp/data/persistencia.cpp -o build/transporte
./build/transporte
```

Crear previamente `build/` si no existe. Estos comandos compilan el menú integrado; los archivos de `cpp/Gestion-Estados/` tienen observaciones pendientes indicadas en la revisión.

Para los transbordos, `swipl` debe estar en `PATH`. Si está instalado en otra carpeta, agregar su carpeta a `PATH`, por ejemplo en PowerShell. El iniciador `ejecutar.ps1` hace este paso al recibir `-Prolog` o la variable `SWIPL`:

```powershell
$env:PATH = 'C:\Program Files\swipl\bin;' + $env:PATH
```

El ejecutable también acepta una carpeta alternativa de datos:

```powershell
.\transporte.exe build/demo
```

### Haskell

```bash
runghc haskell/Main.hs
```

El programa conserva el reporte ambiental existente y agrega vecinos de P14 y rutas habilitadas de las solicitudes. Para consultar otras rutas desde GHCi:

```text
ghci haskell/Main.hs
conexiones <- cargarConexiones "data/conexiones.csv"
vecinos "P14" conexiones
rutasPosibles "P01" "P14" conexiones
rutasPosibles "P01" "P20" conexiones
```

El cierre P14–P15 hace que la última consulta devuelva `[]` con los datos originales. Una ruta de un punto a sí mismo se representa como `[[punto]]` y no recorre tramos.

### Prolog

```bash
swipl -q -s prolog/main.pl
```

Consultas:

```prolog
?- cargar_todo.
?- conectado('P01', 'P02', Distancia).
?- conectado('P02', 'P01', Distancia).
?- conectado('P14', 'P15', _).        % false: tramo cerrado
?- alcanzable('P01', 'P14').          % true
?- alcanzable('P01', 'P20').          % false con data/ original
?- ruta_habilitada('P01', 'P14', Ruta, Km, Minutos).
?- main.                            % resumen y reporte ambiental existente
```

Los códigos se conservan como aparecen en los CSV. En Prolog deben escribirse entre comillas simples porque empiezan con mayúscula.

## Organizacion sencilla del codigo

En C++ las cabeceras `.h` contienen las declaraciones y los `.cpp` contienen las funciones. Se usa `using namespace std`, `struct`, `vector`, `for`, `if` y archivos de texto. Las busquedas recorren vectores sin lambdas ni plantillas. El archivo `cpp/emisiones.hpp` del compañero se conserva sin cambios.

Las funciones nuevas de Haskell tienen auxiliares con nombre, sin definiciones locales con `where` ni funciones anonimas. El lector CSV existente se conserva sin cambios. En Prolog la busqueda usa recursion y comparaciones, sin `append` ni `setof`. Los ejemplos de listas y del recorrido del codigo estan en `docs/APORTE_DIEGO.md`.

## Registro con validaciones

La opción 8 permite registrar paraderos, conexiones, buses y solicitudes. Usa estructuras y vectores existentes y valida antes de modificar los datos:

- Códigos únicos, escritos con mayúsculas, dígitos o `_`.
- Puntos de origen y destino existentes y distintos.
- Tramos únicos en cualquier sentido: P01–P02 equivale a P02–P01.
- Distancia, minutos enteros, capacidad, carga y costo por km positivos; valores reales finitos.
- Tipos, prioridades, estados, hora `HH:MM` y campos adicionales válidos.
- Nombre, zona y tipo de bus sin comas ni saltos de línea, porque los lectores del grupo utilizan CSV simples sin campos entrecomillados.

Los buses nuevos comienzan disponibles y las solicitudes nuevas comienzan pendientes, con ambas columnas de unidad vacías. Para números decimales se usa punto. Los campos de emisiones, mantenimiento, horario y transbordo se conservan al guardar.

Si la validación o el guardado fallan, no se incorpora el registro en memoria. Antes de guardar se lee el contenido original de todos los archivos afectados. Si falla una escritura, se intenta restaurar lo que habia antes y se informa cualquier error de restauracion. Ejecutar una sola instancia de la aplicación por carpeta de datos; no es una base de datos con transacciones ante cortes de energía.

## Transbordos ante contingencias

La opción 9 solicita el código de una solicitud pendiente, de la cual obtiene el origen y el destino. Evita pedirlos otra vez para no contradecir sus datos.

1. C++ escribe `consulta_transbordo.csv`, `puntos_transbordo.csv` y `conexiones_transbordo.csv` en la carpeta de ejecucion. Ejecuta un comando fijo de SWI-Prolog; el origen y destino viajan como datos en el CSV.
2. Prolog determina si el recorrido habitual está bloqueado y genera rutas habilitadas sin repetir puntos. Solo admite un intercambio en un punto con `permite_transbordo=true`.
3. Entre las alternativas, Prolog escoge la de menor tiempo de recorrido. En un empate conserva la primera alternativa encontrada.
4. C++ valida el plan recibido, suma 5 minutos de espera y busca dos buses distintos disponibles. **Cada bus debe poder transportar a todo el grupo**; sus capacidades no se suman.
5. Lee `plan_transbordo.txt` y borra los cuatro archivos de consulta. Guarda la solicitud como `asignado`, registra `unidad_asignada` y `unidad_transbordo` y marca ambos buses como `ocupada` en `unidades.csv`.

No hay un campo de recorrido habitual ni líneas de servicio en los datos existentes. Por eso se define como habitual la ruta de menor tiempo suponiendo todos los tramos abiertos. Si ese recorrido contiene un cierre, se buscan alternativas. La congestión debe representarse mediante el estado `cerrada` para activar esta contingencia. La espera es constante, por lo que minimizar el recorrido también minimiza el tiempo total.

Un transbordo no puede salvar una red físicamente desconectada. Los CSV originales forman una cadena y el cierre P14–P15 la separa: se informa que no existe alternativa, sin modificar solicitudes ni unidades. Los buses se eligen por disponibilidad y capacidad; no se modela su ubicación, ni se sustituye la funcionalidad de mantenimiento de otro integrante.

### Demostración con una alternativa real dentro de la red de prueba

`tests/data/` contiene una red artificial de pruebas. Copiarla antes de utilizar el menú para mantener intactos esos casos:

```powershell
New-Item -ItemType Directory -Force build/demo
Copy-Item tests/data/*.csv build/demo/
.\transporte.exe build/demo
```

Elegir la opción 9 e ingresar `S001`. Resultado esperado:

```text
Solicitud S001 | Ruta habitual bloqueada en tramo P01-P03.
P01 -> P02 (Unidad U01, 14 min)
Transbordo en P02 | Espera: 5 min
P02 -> P03 (Unidad U02, 10 min)
Tiempo total estimado: 29 min | Estado: asignado.
```

Repetir `S001` se rechaza porque ya está asignado. `S002` requiere 70 pasajeros y no dispone de dos buses con capacidad suficiente. `S004` tiene un destino aislado. Para repetir la demostración, volver a copiar los CSV de prueba a `build/demo/`.

Para probar la inferencia directamente en Prolog, sin modificar los datos reales:

```prolog
?- consult('tests/red_prueba.pl').
?- preparar_red.
?- mejor_transbordo('P01', 'P03', Punto, Ruta1, Ruta2, Min1, Min2).
% Punto = 'P02', Ruta1 = ['P01','P02'], Ruta2 = ['P02','P03'], Min1 = 14, Min2 = 10.
```

La función adicional tiene comentarios `FA - Autor: Diego Alejandro Chuquiure Vargas` en C++ y Prolog. Su explicación para la sustentación está en [docs/APORTE_DIEGO.md](docs/APORTE_DIEGO.md).

## Pruebas

Desde la raíz, con compilador, SWI-Prolog y GHC disponibles:

```powershell
New-Item -ItemType Directory -Force build
g++ -std=c++11 -Wall -Wextra -Werror tests/registro_test.cpp cpp/Registro/registro.cpp cpp/data/structures.cpp cpp/data/persistencia.cpp -o build/registro_test.exe
.\build\registro_test.exe

g++ -std=c++11 -Wall -Wextra -Werror tests/transbordos_test.cpp cpp/Registro/registro.cpp cpp/Transbordos/transbordos.cpp cpp/data/structures.cpp cpp/data/persistencia.cpp -o build/transbordos_test.exe
.\build\transbordos_test.exe
g++ -std=c++17 tests/integracion_test.cpp cpp/Registro/registro.cpp cpp/Transbordos/transbordos.cpp cpp/data/structures.cpp cpp/data/persistencia.cpp -o build/integracion_test.exe
.\build\integracion_test.exe
ghc -i./haskell -outputdir build -main-is Pruebas tests/rutas_test.hs haskell/Main.hs -o build/rutas_test.exe
.\build\rutas_test.exe
swipl -q -s tests/rutas_test.pl -g run_tests -t halt
```

En Linux usar las mismas compilaciones con nombres sin `.exe` y ejecutar `./build/<nombre>`. La prueba de integración invoca Prolog de verdad, usa CSV en una carpeta temporal y comprueba registro de las cuatro entidades, rechazos, persistencia, reinicio y asignación de dos buses. No cambia `data/` ni `tests/data/`.



---

## Aporte del Integrante 4 - Renzo Muñoz Guevara

### Funcionalidades implementadas

| Código | Lenguaje | Funcionalidad |
|---|---|---|
| 4.1.6 | C++ | Consultas básicas de puntos, conexiones y pedidos |
| 4.1.7 | C++ | Reporte general del sistema |
| 4.2.7 | Haskell | Resumen de pedidos, rutas posibles y costos de transporte |
| FA | Haskell | Detección de puntos con alta demanda de pasajeros |
| 4.3.5 | Prolog | Verificar si una unidad puede atender un pedido |
| 4.3.6 | Prolog | Identificar pedidos pendientes inaccesibles |

### Ejecutar C++

Desde PowerShell, ubicarse en la carpeta principal del proyecto y ejecutar:

```powershell
.\ejecutar.ps1
```

El script compila los archivos C++ y ejecuta el menú principal.

#### 4.1.6 - Consultas básicas

Seleccionar la opción `10` del menú principal.

Permite realizar las siguientes consultas:

1. Mostrar puntos registrados.
2. Consultar conexiones de un punto.
3. Consultar pedidos por estado.
4. Consultar pedidos asignados a una unidad.

#### 4.1.7 - Reporte general

Seleccionar la opción `11` del menú principal.

Muestra los indicadores generales del sistema.

**Resultados obtenidos durante las pruebas:**

| Indicador | Resultado |
|---|---:|
| Puntos registrados | 47 |
| Conexiones habilitadas | 45 |
| Conexiones cerradas | 1 |
| Unidades disponibles | 8 |
| Pedidos pendientes | 6 |
| Pedidos asignados | 4 |
| Pedidos entregados | 4 |
| Unidad con más pedidos | U01 |
| Cantidad de pedidos de U01 | 3 |

Los resultados pueden cambiar si se modifican los archivos CSV.

### Ejecutar Haskell

Desde PowerShell, ubicarse en la carpeta principal del proyecto y ejecutar:

```powershell
runghc -ihaskell haskell/Main.hs
```

Se mostrarán los siguientes resultados:

- Resumen de datos cargados.
- Conectividad y rutas habilitadas.
- Reporte ambiental de los buses.
- Resumen de pedidos, rutas posibles y costos de transporte.
- Demanda de pasajeros por punto entre las 18:00 y las 19:00.

#### 4.2.7 - Resumen de resultados

El módulo `ReporteResultados.hs` permite:

- Mostrar la información de cada pedido.
- Calcular la cantidad de rutas posibles.
- Calcular la distancia de la primera ruta habilitada encontrada.
- Estimar el costo de esa ruta para cada unidad.
- Contar los pedidos pendientes según su prioridad.

#### Funcionalidad adicional - Detección de puntos con alta demanda

El módulo `DemandaParaderos.hs` analiza los pedidos pendientes según el punto de origen y la franja horaria.

Utiliza `filter`, `map` y `sum` para calcular la demanda de pasajeros.

**Ejemplo con los datos de prueba:**

| Punto | Pasajeros pendientes | Estado |
|---|---:|---|
| P01 | 75 | Demanda alta |
| P14 | 95 | Demanda alta |

Se considera demanda alta cuando la cantidad de pasajeros pendientes supera el límite configurado de 70 pasajeros.

### Ejecutar Prolog

Desde PowerShell, ubicarse en la carpeta principal del proyecto y abrir SWI-Prolog:

```powershell
& "C:\Program Files\swipl\bin\swipl.exe" -q -s prolog/main.pl
```

Si `swipl` está configurado en el PATH de Windows, también se puede ejecutar:

```powershell
swipl -q -s prolog/main.pl
```

Dentro de Prolog, cargar los datos:

```prolog
cargar_todo.
```

Importar las funcionalidades del integrante 4:

```prolog
use_module('prolog/GestionPedidos/gestion-pedidos.pl').
```

#### 4.3.5 - Unidades que pueden atender un pedido

Verifica que el pedido esté pendiente, la unidad esté disponible y su capacidad sea suficiente.

Consultar una unidad compatible con el pedido S009:

```prolog
puede_atender(Unidad, 'S009').
```

Mostrar todas las unidades compatibles:

```prolog
findall(Unidad, puede_atender(Unidad, 'S009'), Lista).
```

#### 4.3.6 - Pedidos inaccesibles

Identifica los pedidos pendientes que no tienen una ruta habilitada entre su origen y destino.

Consultar los pedidos inaccesibles:

```prolog
pedido_inaccesible(Pedido).
```

Mostrar todos los pedidos inaccesibles:

```prolog
findall(Pedido, pedido_inaccesible(Pedido), Lista).
```

**Resultados obtenidos durante las pruebas:**

```text
Lista = ['S009', 'S010', 'S013'].
```

#### Comandos adicionales de Prolog

| Acción | Comando |
|---|---|
| Recargar archivos modificados | `make.` |
| Mostrar resumen de datos | `resumen_datos.` |
| Verificar si S009 es inaccesible | `pedido_inaccesible('S009').` |
| Salir de Prolog | `halt.` |

### Archivos del integrante 4

**C++**

- `cpp/Consultas/consultas.h`
- `cpp/Consultas/consultas.cpp`
- `cpp/ReporteGeneral/reporte-general.h`
- `cpp/ReporteGeneral/reporte-general.cpp`

**Haskell**

- `haskell/Reportes/ReporteResultados.hs`
- `haskell/DemandaParaderos/DemandaParaderos.hs`

**Prolog**

- `prolog/GestionPedidos/gestion-pedidos.pl`

Las funcionalidades de C++ están integradas en `cpp/MenuPrincipal/menu-principal.cpp` mediante las opciones 10 y 11.

Las funcionalidades de Haskell se ejecutan desde `haskell/Main.hs`.

Las funcionalidades de Prolog utilizan los datos cargados mediante `prolog/LecturaDeArchivosCSV/lectura-csv.pl`.

Los tres lenguajes trabajan con los archivos CSV compartidos de la carpeta `data/`.
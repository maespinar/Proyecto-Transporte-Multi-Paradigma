# Aporte de Diego Alejandro Chuquiure Vargas

Esta version usa funciones pequenas y pasos explicitos. C++ esta separado en archivos .h y .cpp, Haskell usa funciones auxiliares con nombre y Prolog usa reglas recursivas para recorrer y comparar rutas.

## C++ archivos .h y .cpp

| Archivo | Que contiene |
| --- | --- |
| cpp/Registro/registro.h | Declaraciones de las funciones de registro. |
| cpp/Registro/registro.cpp | Validaciones, registro de las cuatro entidades y preguntas por consola. |
| cpp/Transbordos/transbordos.h | La estructura PlanTransbordo y declaraciones del transbordo. |
| cpp/Transbordos/transbordos.cpp | Consulta a Prolog, validacion de rutas y asignacion de dos buses. |
| cpp/data/persistencia.h | La estructura CambioCSV y declaraciones para guardar CSV. |
| cpp/data/persistencia.cpp | Escritura de los archivos conservando las columnas del grupo. |
| cpp/main.cpp | Carga inicial y menu; opcion 8 registra y opcion 9 planifica transbordos. |

En una cabecera se indica como se llama una funcion, que recibe y que devuelve:

```cpp
bool existePunto(string codigo, const vector<Punto>& puntos);
```

En el .cpp se escribe lo que hace. Por ejemplo, existePunto recorre el vector con un for y devuelve true cuando encuentra el codigo. Si termina el recorrido sin encontrarlo, devuelve false.

Se usa using namespace std para escribir string, vector y cout. No se usan lambdas, plantillas, auto, filesystem ni librerias de procesos de Windows/Linux en el programa.

## C++ 4.1.3 registro con validaciones

registrarPunto comprueba codigo unico, nombre, zona y tipo. registrarConexion revisa puntos existentes, extremos distintos, distancia y tiempo positivos y que el tramo no exista en ninguno de los dos sentidos. registrarUnidad revisa codigo, capacidad, costo y los campos adicionales. registrarPedido revisa codigo, puntos, pasajeros, prioridad y hora.

Las cuatro funciones siguen este orden: limpiar el mensaje de error, comprobar condiciones, devolver false si falla algo y agregar al vector si todos los datos son validos.

procesarRegistro hace las preguntas de la opcion 8. Trabaja sobre copias de los vectores, guarda el CSV correspondiente y despues actualiza los vectores del programa. Un registro rechazado no cambia los datos.

## Haskell 4.2.3 vecinos

El archivo es haskell/Main.hs. Las funciones nuevas se leen por separado, sin definiciones locales con where. Se conserva el lector original del compañero, que contiene sus propias definiciones con where; no forma parte del aporte de Diego. El where de module Main where pertenece a la declaracion del modulo.

1. conexionHabilitada comprueba el estado de una conexion.
2. filter descarta las conexiones cerradas.
3. buscarVecinos recorre una conexion a la vez. Si el punto coincide con un extremo, agrega el otro extremo.
4. foldr aplica agregarSinRepetir para formar la lista sin duplicados.

Ejemplo con los datos originales:

```haskell
vecinos "P02" conexiones
-- ["P01","P03"]
```

agregarSinRepetir comprueba si el punto ya esta en la lista acumulada. Si esta, devuelve esa lista; si no esta, lo agrega al inicio. Se mantienen map, filter y foldr para cumplir el uso de funciones de orden superior pedido por el enunciado.

## Haskell 4.2.4 rutas posibles

rutasPosibles inicia la busqueda con el origen como primer visitado. buscarRutas comprueba si ya llegamos al destino. Si llegamos, devuelve el recorrido; de lo contrario, llama a continuarRutas.

continuarRutas toma un vecino de la lista. Si ya esta visitado, lo salta. Si no esta visitado, busca rutas desde ese vecino. El operador ++ junta las rutas de ese vecino con las del resto.

```haskell
rutasPosibles "P01" "P03" conexiones
-- [["P01","P02","P03"]]
```

Cada alternativa recibe su propia lista de visitados. Asi se evitan ciclos sin perder rutas distintas que comparten un paradero. Las funciones son puras: reciben listas y devuelven resultados sin modificar los CSV.

## Prolog listas basicas

Las listas representan los puntos de un recorrido. Su notacion necesaria es:

```prolog
[]                   % lista vacia
['P01','P02','P03']   % tres puntos
[Primero|Resto]      % primer elemento separado del resto
```

Por ejemplo, al comparar ['P01','P02','P03'] con [Primero|Resto], Primero queda como 'P01' y Resto como ['P02','P03']. Se usa esta separacion para revisar un punto y continuar con los siguientes. Una lista de una sola ruta ya basta para entender el recorrido: no se usan append, setof ni listas de candidatos ordenadas.

## Prolog 4.3.2 y 4.3.3

El archivo es prolog/rutas.pl. conectado tiene dos reglas: busca la conexion habilitada con los extremos en un orden y en el contrario. alcanzable pregunta si existe una ruta habilitada entre los puntos.

recorrer_red toma un tramo, verifica con visitado que el siguiente punto no se haya usado y continua desde ese punto. Al llegar al destino, termina. Prolog prueba conexiones alternativas mediante backtracking.

```prolog
conectado('P01', 'P02', Distancia).
alcanzable('P01', 'P14').
```

## Funcionalidad adicional transbordos

En prolog/rutas.pl, ruta_habitual busca un recorrido y comprueba que no exista otro de menor tiempo. El simbolo \+ significa que la consulta que sigue no debe cumplirse. Asi se compara sin setof: una alternativa es la mejor si no existe otra mas rapida. Ante empate se conserva la primera encontrada; ! indica que se detiene esa seleccion.

tramo_bloqueado revisa dos puntos consecutivos. separar_ruta avanza por la lista hasta el punto de intercambio y construye las dos etapas por recursion. ruta_con_transbordo comprueba que el punto permita el cambio de bus. mejor_transbordo compara los tiempos de las alternativas.

En cpp/Transbordos/transbordos.cpp, consultarTransbordo escribe tres CSV de consulta y ejecuta un comando fijo de Prolog con system. Los codigos del usuario se escriben en el CSV, nunca se agregan al comando. prolog/consulta_transbordo.pl lee esos archivos y devuelve plan_transbordo.txt. C++ lee la respuesta y elimina los cuatro archivos de consulta.

asignarTransbordo recorre la flota con un for y busca dos buses disponibles. Cada uno debe tener capacidad para todos los pasajeros: dos buses de 30 plazas no sirven para hacer este transbordo de 45 pasajeros. Los dos buses transportan al mismo grupo en etapas distintas.

procesarTransbordo suma los 5 minutos de espera, guarda las dos unidades en la solicitud y marca ambos buses ocupados. La prueba de ejemplo suma 14 + 5 + 10 = 29 minutos.

La ruta habitual se define como la de menor tiempo suponiendo todos los tramos abiertos, porque los CSV no guardan un recorrido habitual. Un transbordo requiere una alternativa fisica; no permite atravesar un tramo cerrado.

## Orden para estudiar los archivos

Primero revisar registro.h y despues sus funciones en registro.cpp. Continuar con vecinos, buscarVecinos, buscarRutas y continuarRutas en Haskell. Luego revisar conectado, alcanzable y recorrer_red en Prolog. Finalmente seguir consultarTransbordo, mejor_transbordo, asignarTransbordo y procesarTransbordo.

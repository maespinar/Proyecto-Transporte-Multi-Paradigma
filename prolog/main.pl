:- use_module(library(csv)).

:- dynamic punto/5.
:- dynamic conexion/5.
:- dynamic unidad/9.
:- dynamic pedido/9.

ruta_archivo(Rel, Ruta) :-
    (   exists_file(Rel) -> Ruta = Rel
    ;   atom_concat('../', Rel, Sub), exists_file(Sub) -> Ruta = Sub
    ;   Ruta = Rel
    ).

guardar_puntos([]).
guardar_puntos([row(A, B, C, D, E)|Resto]) :-
    assertz(punto(A, B, C, D, E)),
    guardar_puntos(Resto).

cargar_puntos :-
    retractall(punto(_, _, _, _, _)),
    ruta_archivo('data/puntos.csv', Ruta),
    csv_read_file(Ruta, [_|Filas], [strip(true)]),
    guardar_puntos(Filas).

guardar_conexiones([]).
guardar_conexiones([row(A, B, C, D, E)|Resto]) :-
    assertz(conexion(A, B, C, D, E)),
    guardar_conexiones(Resto).

cargar_conexiones :-
    retractall(conexion(_, _, _, _, _)),
    ruta_archivo('data/conexiones.csv', Ruta),
    csv_read_file(Ruta, [_|Filas], [strip(true)]),
    guardar_conexiones(Filas).

guardar_unidades([]).
guardar_unidades([row(A, B, C, D, E, F, G, H, I)|Resto]) :-
    assertz(unidad(A, B, C, D, E, F, G, H, I)),
    guardar_unidades(Resto).

cargar_unidades :-
    retractall(unidad(_, _, _, _, _, _, _, _, _)),
    ruta_archivo('data/unidades.csv', Ruta),
    csv_read_file(Ruta, [_|Filas], [strip(true)]),
    guardar_unidades(Filas).

guardar_pedidos([]).
guardar_pedidos([Fila|Resto]) :-
    Fila =.. [row, A, B, C, D, E, F, G, H, I],
    assertz(pedido(A, B, C, D, E, F, G, H, I)),
    guardar_pedidos(Resto).
guardar_pedidos([Fila|Resto]) :-
    Fila =.. [row, A, B, C, D, E, F, G, H],
    assertz(pedido(A, B, C, D, E, F, G, H, '')),
    guardar_pedidos(Resto).

cargar_pedidos :-
    retractall(pedido(_, _, _, _, _, _, _, _, _)),
    ruta_archivo('data/pedidos.csv', Ruta),
    csv_read_file(Ruta, [_|Filas], [strip(true)]),
    guardar_pedidos(Filas).

cargar_todo :-
    cargar_puntos,
    cargar_conexiones,
    cargar_unidades,
    cargar_pedidos.

resumen_datos :-
    findall(P, punto(P, _, _, _, _), L1), length(L1, N1),
    findall(C, conexion(C, _, _, _, _), L2), length(L2, N2),
    findall(U, unidad(U, _, _, _, _, _, _, _, _), L3), length(L3, N3),
    findall(D, pedido(D, _, _, _, _, _, _, _, _), L4), length(L4, N4),
    format('Puntos cargados: ~w~n', [N1]),
    format('Conexiones cargadas: ~w~n', [N2]),
    format('Unidades en flota: ~w~n', [N3]),
    format('Pedidos cargados: ~w~n', [N4]).

calcular_emision(Distancia, CodUnidad, Emision, Clasificacion) :-
    unidad(CodUnidad, _, _, _, _, _, Factor, _, _),
    Emision is Distancia * Factor,
    clasificar_emision(Emision, Clasificacion).

clasificar_emision(E, 'Nivel bueno') :- E =< 8.0, !.
clasificar_emision(E, 'Nivel moderado') :- E =< 15.0, !.
clasificar_emision(_, 'Nivel alto').

reporte_unidad(Distancia, CodUnidad) :-
    unidad(CodUnidad, Tipo, _, _, _, Combustible, _, _, _),
    calcular_emision(Distancia, CodUnidad, Emision, Clasificacion),
    format('~w | ~w | Combustible: ~w | Emision: ~2f kg CO2 | ~w~n', [CodUnidad, Tipo, Combustible, Emision, Clasificacion]).

reporte_flota(Distancia) :-
    forall(unidad(Cod, _, _, _, _, _, _, _, _), reporte_unidad(Distancia, Cod)).

main :-
    writeln('================================================'),
    writeln('  MODULO PROLOG: BASE DE CONOCIMIENTO Y LOGICA  '),
    writeln('================================================'),
    cargar_todo,
    nl,
    writeln('--- Resumen de Carga en Base de Conocimiento ---'),
    resumen_datos,
    nl,
    writeln('========================================================'),
    writeln(' REPORTE AMBIENTAL DE TODA LA FLOTA (Ruta base: 14.5 km)'),
    writeln('========================================================'),
    reporte_flota(14.5).
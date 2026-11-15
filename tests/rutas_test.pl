:- ensure_loaded('../prolog/main').
:- ensure_loaded('red_prueba').
:- use_module(library(plunit)).
:- begin_tests(rutas_diego, [setup(preparar_red)]).

test(conexion_directa, [nondet]) :- conectado('P01', 'P02', 2.0).
test(doble_sentido, [nondet]) :- conectado('P02', 'P01', 2.0).
test(cerrada, [fail]) :- conectado('P01', 'P03', _).
test(alcance_indirecto, [nondet]) :- alcanzable('P01', 'P03').
test(alcance_inverso, [nondet]) :- alcanzable('P03', 'P01').
test(aislado, [fail]) :- alcanzable('P01', 'P05').
test(desconocido, [fail]) :- alcanzable('X', 'X').
test(mismo_punto, [nondet]) :- alcanzable('P01', 'P01').
test(optimo, [true((T == 'P02', R1 == ['P01','P02'], R2 == ['P02','P03'], A == 14, B == 10))]) :-
    mejor_transbordo('P01', 'P03', T, R1, R2, A, B).
test(inverso) :- mejor_transbordo('P03', 'P01', 'P02', _, _, 10, 14).
test(minimo_aunque_el_lento_aparezca_primero,
     [setup((retract(user:conexion('P01','P02',2.0,14,habilitada)),
             assertz(user:conexion('P01','P02',2.0,14,habilitada)))),
      cleanup(preparar_red)]) :-
    mejor_transbordo('P01', 'P03', 'P02', ['P01','P02'], ['P02','P03'], 14, 10).
test(sin_contingencia, [fail]) :- mejor_transbordo('P01', 'P04', _, _, _, _, _).
test(sin_ruta, [fail]) :- mejor_transbordo('P01', 'P05', _, _, _, _, _).
test(extremos_no_son_transbordo, [fail]) :- mejor_transbordo('P02', 'P02', _, _, _, _, _).
test(sin_infraestructura, [setup(retract(user:punto('P02', intercambio, intercambio, zona, true))), cleanup(assertz(user:punto('P02', intercambio, intercambio, zona, true))), fail]) :-
    mejor_transbordo('P01', 'P03', _, _, _, _, _).
test(sin_ciclos) :-
    forall(ruta_habilitada('P01', 'P03', R, _, _), (sort(R, U), length(R, N), length(U, N))).

:- end_tests(rutas_diego).

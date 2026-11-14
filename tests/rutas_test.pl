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
:- end_tests(rutas_diego).

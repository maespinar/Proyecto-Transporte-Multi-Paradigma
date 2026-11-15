:- use_module(library(csv)).
:- use_module('LecturaDeArchivosCSV/lectura-csv.pl').
:- use_module('rutas.pl').

% FA - Autor: Diego Alejandro Chuquiure Vargas.
% C++ deja tres CSV de consulta. Prolog responde en plan_transbordo.txt.
consulta_transbordo :-
    csv_read_file('consulta_transbordo.csv', [_, row(Origen, Destino)], [strip(true)]),
    csv_read_file('puntos_transbordo.csv', [_|Puntos], [strip(true)]),
    csv_read_file('conexiones_transbordo.csv', [_|Conexiones], [strip(true)]),
    retractall(lectura_csv:punto(_, _, _, _, _)),
    retractall(lectura_csv:conexion(_, _, _, _, _)),
    guardar_puntos(Puntos),
    guardar_conexiones(Conexiones),
    open('plan_transbordo.txt', write, Archivo),
    escribir_plan(Archivo, Origen, Destino),
    close(Archivo).

escribir_plan(Archivo, Origen, Destino) :-
    \+ ruta_red(todas, Origen, Destino, _, _, _),
    !,
    writeln(Archivo, sin_ruta).

escribir_plan(Archivo, Origen, Destino) :-
    \+ ruta_habitual_bloqueada(Origen, Destino, _),
    !,
    writeln(Archivo, sin_contingencia).

escribir_plan(Archivo, Origen, Destino) :-
    mejor_transbordo(Origen, Destino, Punto, Ruta1, Ruta2, Tiempo1, Tiempo2),
    ruta_habitual_bloqueada(Origen, Destino, Habitual),
    tramo_bloqueado(Habitual, A, B),
    !,
    writeln(Archivo, ok),
    format(Archivo, '~w ~d ~d~n', [Punto, Tiempo1, Tiempo2]),
    escribir_ruta(Archivo, Ruta1), nl(Archivo),
    escribir_ruta(Archivo, Ruta2), nl(Archivo),
    format(Archivo, '~w ~w~n', [A, B]).

escribir_plan(Archivo, _, _) :-
    writeln(Archivo, sin_ruta).

escribir_ruta(_, []).
escribir_ruta(Archivo, [Punto|Resto]) :-
    write(Archivo, Punto),
    write(Archivo, ' '),
    escribir_ruta(Archivo, Resto).

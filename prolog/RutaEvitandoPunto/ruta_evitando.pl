:- module(ruta_evitando_punto, [ruta_evitando/4]).

:- use_module('../LecturaDeArchivosCSV/lectura-csv.pl', [punto/5]).
:- use_module('../rutas.pl', [ruta_habilitada/5]).

ruta_evitando(Origen, Destino, Evitado, Ruta) :-
    punto(Evitado, _, _, _, _),
    ruta_habilitada(Origen, Destino, Ruta, _, _),
    \+ member(Evitado, Ruta).

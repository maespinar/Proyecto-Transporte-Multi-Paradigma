:- module(emisiones, [
    calcular_emision/4, clasificar_emision/2,
    reporte_unidad/2, reporte_flota/1
]).

:- use_module('../LecturaDeArchivosCSV/lectura-csv.pl', [unidad/9]).

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


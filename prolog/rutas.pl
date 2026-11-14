% 4.3.2 - Autor: Diego Alejandro Chuquiure Vargas.
% Dos reglas porque cada tramo se puede recorrer en ambos sentidos.
conectado(Origen, Destino, Distancia) :-
    conexion(Origen, Destino, Distancia, _, habilitada).
conectado(Origen, Destino, Distancia) :-
    conexion(Destino, Origen, Distancia, _, habilitada).

tramo_red(habilitada, Origen, Destino, Distancia, Tiempo) :-
    conexion(Origen, Destino, Distancia, Tiempo, habilitada).
tramo_red(habilitada, Origen, Destino, Distancia, Tiempo) :-
    conexion(Destino, Origen, Distancia, Tiempo, habilitada).
tramo_red(todas, Origen, Destino, Distancia, Tiempo) :-
    conexion(Origen, Destino, Distancia, Tiempo, _).
tramo_red(todas, Origen, Destino, Distancia, Tiempo) :-
    conexion(Destino, Origen, Distancia, Tiempo, _).

% [Punto|Resto] separa el primer punto del resto de una lista.
visitado(Punto, [Punto|_]).
visitado(Punto, [_|Resto]) :-
    visitado(Punto, Resto).

ruta_red(Modo, Origen, Destino, Ruta, Distancia, Tiempo) :-
    punto(Origen, _, _, _, _),
    punto(Destino, _, _, _, _),
    recorrer_red(Modo, Origen, Destino, [Origen], Ruta, Distancia, Tiempo).

recorrer_red(_, Destino, Destino, _, [Destino], 0, 0).
recorrer_red(Modo, Actual, Destino, Visitados, [Actual|Resto], Distancia, Tiempo) :-
    Actual \== Destino,
    tramo_red(Modo, Actual, Siguiente, D, T),
    \+ visitado(Siguiente, Visitados),
    recorrer_red(Modo, Siguiente, Destino, [Siguiente|Visitados], Resto, DistanciaRestante, TiempoRestante),
    Distancia is D + DistanciaRestante,
    Tiempo is T + TiempoRestante.

ruta_habilitada(Origen, Destino, Ruta, Distancia, Tiempo) :-
    ruta_red(habilitada, Origen, Destino, Ruta, Distancia, Tiempo).

% 4.3.3 - Autor: Diego Alejandro Chuquiure Vargas.
alcanzable(Origen, Destino) :-
    ruta_habilitada(Origen, Destino, _, _, _).

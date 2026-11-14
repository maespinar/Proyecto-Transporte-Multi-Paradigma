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

% FA - Autor: Diego Alejandro Chuquiure Vargas.
% Una ruta es la mas rapida si no existe otra que tarde menos.
hay_ruta_mas_rapida(Origen, Destino, Tiempo) :-
    ruta_red(todas, Origen, Destino, _, _, OtroTiempo),
    OtroTiempo < Tiempo.

ruta_habitual(Origen, Destino, Ruta) :-
    ruta_red(todas, Origen, Destino, Ruta, _, Tiempo),
    \+ hay_ruta_mas_rapida(Origen, Destino, Tiempo),
    !. % Ante un empate se conserva la primera ruta encontrada.

conexion_cerrada(A, B) :- conexion(A, B, _, _, cerrada).
conexion_cerrada(A, B) :- conexion(B, A, _, _, cerrada).

% Se revisan dos puntos consecutivos y luego se avanza al siguiente par.
tramo_bloqueado([A, B|_], A, B) :- conexion_cerrada(A, B).
tramo_bloqueado([_|Resto], A, B) :- tramo_bloqueado(Resto, A, B).

ruta_habitual_bloqueada(Origen, Destino, Ruta) :-
    ruta_habitual(Origen, Destino, Ruta),
    tramo_bloqueado(Ruta, _, _).

% El intercambio termina la primera etapa y empieza la segunda.
% Se exige un punto posterior para que la segunda etapa no sea vacia.
separar_ruta([Punto, Siguiente|Resto], Punto, [Punto], [Punto, Siguiente|Resto]).
separar_ruta([Actual|Resto], Punto, [Actual|Primera], Segunda) :-
    separar_ruta(Resto, Punto, Primera, Segunda).

tiempo_ruta([_], 0).
tiempo_ruta([A, B|Resto], Tiempo) :-
    tramo_red(habilitada, A, B, _, Minutos),
    tiempo_ruta([B|Resto], TiempoRestante),
    Tiempo is Minutos + TiempoRestante.

ruta_con_transbordo(Origen, Destino, Punto, [Origen|Primera], Segunda, Tiempo1, Tiempo2) :-
    ruta_habilitada(Origen, Destino, [Origen|Resto], _, _),
    separar_ruta(Resto, Punto, Primera, Segunda),
    punto(Punto, _, _, _, true),
    tiempo_ruta([Origen|Primera], Tiempo1),
    tiempo_ruta(Segunda, Tiempo2).

hay_transbordo_mas_rapido(Origen, Destino, Tiempo) :-
    ruta_con_transbordo(Origen, Destino, _, _, _, Tiempo1, Tiempo2),
    Tiempo1 + Tiempo2 < Tiempo.

mejor_transbordo(Origen, Destino, Punto, Ruta1, Ruta2, Tiempo1, Tiempo2) :-
    ruta_habitual_bloqueada(Origen, Destino, _),
    ruta_con_transbordo(Origen, Destino, Punto, Ruta1, Ruta2, Tiempo1, Tiempo2),
    Tiempo is Tiempo1 + Tiempo2,
    \+ hay_transbordo_mas_rapido(Origen, Destino, Tiempo),
    !.

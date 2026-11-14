% 4.3.2 - Autor: Diego Alejandro Chuquiure Vargas.
% Dos reglas porque cada tramo se puede recorrer en ambos sentidos.
conectado(Origen, Destino, Distancia) :-
    conexion(Origen, Destino, Distancia, _, habilitada).
conectado(Origen, Destino, Distancia) :-
    conexion(Destino, Origen, Distancia, _, habilitada).


:- module(gestion_pedidos, [puede_atender/2, pedido_inaccesible/1]).

:- use_module('../LecturaDeArchivosCSV/lectura-csv.pl').
:- use_module('../rutas.pl').

puede_atender(Unidad, Pedido) :-
    pedido(Pedido, _, _, Carga, _, pendiente, _, _, _),
    unidad(Unidad, _, Capacidad, _, disponible, _, _, _, _),
    Capacidad >= Carga.

pedido_inaccesible(Pedido) :-
    pedido(Pedido, Origen, Destino, _, _, pendiente, _, _, _),
    \+ alcanzable(Origen, Destino).

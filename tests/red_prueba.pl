% Red artificial exclusiva de pruebas; no altera data/.
preparar_red :-
    retractall(punto(_, _, _, _, _)),
    retractall(conexion(_, _, _, _, _)),
    assertz(punto('P01', inicio, terminal, zona, false)),
    assertz(punto('P02', intercambio, intercambio, zona, true)),
    assertz(punto('P03', destino, paradero, zona, false)),
    assertz(punto('P04', desvio, paradero, zona, false)),
    assertz(punto('P05', aislado, paradero, zona, true)),
    assertz(conexion('P01', 'P03', 1.0, 2, cerrada)),
    assertz(conexion('P01', 'P02', 2.0, 14, habilitada)),
    assertz(conexion('P02', 'P03', 2.0, 10, habilitada)),
    assertz(conexion('P01', 'P04', 1.0, 20, habilitada)),
    assertz(conexion('P04', 'P02', 1.0, 20, habilitada)).

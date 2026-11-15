:- use_module('LecturaDeArchivosCSV/lectura-csv.pl').
:- use_module('CalculoDeEmisionesCO2/emisiones.pl').
:- use_module('rutas.pl').

main :-
    writeln('================================================'),
    writeln('  MODULO PROLOG: BASE DE CONOCIMIENTO Y LOGICA  '),
    writeln('================================================'),
    cargar_todo,
    nl,
    writeln('--- Resumen de Carga en Base de Conocimiento ---'),
    resumen_datos,
    nl,
    writeln('========================================================'),
    writeln(' REPORTE AMBIENTAL DE TODA LA FLOTA (Ruta base: 14.5 km)'),
    writeln('========================================================'),
    reporte_flota(14.5).
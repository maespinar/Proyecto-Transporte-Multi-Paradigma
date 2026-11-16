module Main where

import LecturaDeArchivosCSV.LecturaCSV
import Reportes.ReporteInicial
import Reportes.ReporteResultados
import DemandaParaderos.DemandaParaderos

main :: IO ()
main = do
    mostrarReporteInicial

    puntos <- cargarPuntos "data/puntos.csv"
    conexiones <- cargarConexiones "data/conexiones.csv"
    unidades <- cargarUnidades "data/unidades.csv"
    pedidos <- cargarPedidos "data/pedidos.csv"

    mostrarReporte pedidos conexiones unidades
    mostrarTodos puntos pedidos
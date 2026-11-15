module Reportes.ReporteInicial where

import LecturaDeArchivosCSV.LecturaCSV
import CalculoDeEmisionesCO2.Emisiones
import CalculoDeRutas.Rutas

mostrarReporteInicial :: IO ()
mostrarReporteInicial = do
    putStrLn "================================================"
    putStrLn " MODULO HASKELL: CARGA Y PARADIGMA FUNCIONAL"
    putStrLn "================================================"
    puntos     <- cargarPuntos "data/puntos.csv"
    conexiones <- cargarConexiones "data/conexiones.csv"
    unidades   <- cargarUnidades "data/unidades.csv"
    pedidos    <- cargarPedidos "data/pedidos.csv"

    putStrLn "\n--- Resumen de Carga en Memoria ---"
    putStrLn ("Puntos cargados: " ++ show (length puntos))
    putStrLn ("Conexiones cargadas: " ++ show (length conexiones))
    putStrLn ("Unidades en flota: " ++ show (length unidades))
    putStrLn ("Pedidos cargados: " ++ show (length pedidos))

    putStrLn "\n--- CONECTIVIDAD Y RUTAS HABILITADAS ---"
    putStrLn ("Vecinos de P14: " ++ show (vecinos "P14" conexiones))
    mapM_ (mostrarRutasPedido conexiones) pedidos

    putStrLn "\n========================================================"
    putStrLn " REPORTE AMBIENTAL DE TODA LA FLOTA (Ruta base: 14.5 km)"
    putStrLn "========================================================"
    mapM_ (mostrarReporteBus 14.5) unidades

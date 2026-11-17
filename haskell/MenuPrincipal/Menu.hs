module MenuPrincipal.Menu (ejecutarMenuHaskell) where

import ControlDeAutonomia.Autonomia (reporteAutonomia)
import DefinicionDeTipos.Tipos
import LecturaDeArchivosCSV.LecturaCSV
import Reportes.ReporteInicial (mostrarReporteInicial)
import Reportes.ReporteResultados (mostrarReporte)
import DemandaParaderos.DemandaParaderos (mostrarTodos)

ejecutarMenuHaskell :: IO ()
ejecutarMenuHaskell = do
    mostrarReporteInicial
    puntos <- cargarPuntos "data/puntos.csv"
    conexiones <- cargarConexiones "data/conexiones.csv"
    unidades <- cargarUnidades "data/unidades.csv"
    pedidos <- cargarPedidos "data/pedidos.csv"
    mostrarReporte pedidos conexiones unidades
    mostrarTodos puntos pedidos
    entrada <- getContents
    mostrarMenu (lines entrada) conexiones unidades pedidos

mostrarMenu :: [String] -> [Conexion] -> [Unidad] -> [Pedido] -> IO ()
mostrarMenu entradas conexiones unidades pedidos = do
    putStrLn "\n--- MODULO HASKELL ---"
    putStrLn "1. Mostrar reporte inicial"
    putStrLn "2. Verificar autonomia y mantenimiento de un bus"
    putStrLn "3. Salir"
    (opcion, restantes) <- leerEntrada "Seleccione una opcion: " entradas
    case opcion of
        "1" -> do
            mostrarReporteInicial
            mostrarMenu restantes conexiones unidades pedidos
        "2" -> do
            (codigoPedido, siguientes) <- leerEntrada "Codigo de pedido: " restantes
            (codigoUnidad, pendientes) <- leerEntrada "Codigo de bus: " siguientes
            putStrLn (reporteAutonomia codigoPedido codigoUnidad pedidos unidades conexiones)
            mostrarMenu pendientes conexiones unidades pedidos
        "3" -> putStrLn "Cerrando modulo Haskell."
        "" -> return ()
        _ -> do
            putStrLn "Seleccione una opcion entre 1 y 3."
            mostrarMenu restantes conexiones unidades pedidos

leerEntrada :: String -> [String] -> IO (String, [String])
leerEntrada mensaje entradas = do
    putStr mensaje
    case entradas of
        [] -> return ("", [])
        entrada:restantes -> return (entrada, restantes)

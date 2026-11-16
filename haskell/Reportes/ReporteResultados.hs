
module Reportes.ReporteResultados where

import DefinicionDeTipos.Tipos
import CalculoDeRutas.Rutas

contarPendientes :: PrioridadPedido -> [Pedido] -> Int
contarPendientes prioridad pedidos =
    length (filter (\p ->
        estadoPedido p == Pendiente &&
        prioridadPedido p == prioridad) pedidos)

mostrarResumenPedido :: [Conexion] -> [Unidad] -> Pedido -> IO ()
mostrarResumenPedido conexiones unidades pedido = do
    let rutas = rutasPosibles (origPedido pedido) (destPedido pedido) conexiones

    putStrLn ("Pedido: " ++ codPedido pedido)
    putStrLn ("Origen: " ++ origPedido pedido)
    putStrLn ("Destino: " ++ destPedido pedido)
    putStrLn ("Pasajeros: " ++ show (cargaPedido pedido))
    putStrLn ("Estado: " ++ show (estadoPedido pedido))
    putStrLn ("Rutas posibles: " ++ show (length rutas))

    if null rutas
        then putStrLn "No existe una ruta habilitada"
        else do
            let ruta = head rutas
            let distancia = distanciaRuta ruta conexiones

            putStrLn ("Ruta: " ++ show ruta)
            putStrLn ("Distancia: " ++ show distancia ++ " km")
            mapM_ (mostrarCosto distancia) unidades

    putStrLn "-----------------------"

mostrarReporte :: [Pedido] -> [Conexion] -> [Unidad] -> IO ()
mostrarReporte pedidos conexiones unidades = do
    putStrLn "\n--- RESUMEN DE RESULTADOS ---"

    mapM_ (mostrarResumenPedido conexiones unidades) pedidos

    putStrLn "--- PEDIDOS PENDIENTES ---"
    putStrLn ("Prioridad alta: " ++ show (contarPendientes Alta pedidos))
    putStrLn ("Prioridad media: " ++ show (contarPendientes Media pedidos))
    putStrLn ("Prioridad baja: " ++ show (contarPendientes Baja pedidos))

distanciaTramo :: String -> String -> [Conexion] -> Double
distanciaTramo origen destino conexiones =
    sum (map distanciaKm (filter (\c ->
        estadoConexion c == Habilitada &&
        ((origConexion c == origen && destConexion c == destino) ||
        (origConexion c == destino && destConexion c == origen))) conexiones))

distanciaRuta :: [String] -> [Conexion] -> Double
distanciaRuta (a:b:resto) conexiones =
    distanciaTramo a b conexiones + distanciaRuta (b:resto) conexiones
distanciaRuta _ _ = 0

mostrarCosto :: Double -> Unidad -> IO ()
mostrarCosto distancia unidad =
    putStrLn (codUnidad unidad ++ " | Costo: S/ " ++
        show (distancia * costoKm unidad))

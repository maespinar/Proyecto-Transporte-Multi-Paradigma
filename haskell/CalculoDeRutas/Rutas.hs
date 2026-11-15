module CalculoDeRutas.Rutas where

import DefinicionDeTipos.Tipos

vecinos :: String -> [Conexion] -> [String]
vecinos punto conexiones = foldr agregarSinRepetir [] (buscarVecinos punto (filter conexionHabilitada conexiones))

conexionHabilitada :: Conexion -> Bool
conexionHabilitada conexion = estadoConexion conexion == Habilitada

buscarVecinos :: String -> [Conexion] -> [String]
buscarVecinos _ [] = []
buscarVecinos punto (conexion:resto)
    | origConexion conexion == punto = destConexion conexion : buscarVecinos punto resto
    | destConexion conexion == punto = origConexion conexion : buscarVecinos punto resto
    | otherwise = buscarVecinos punto resto

agregarSinRepetir :: String -> [String] -> [String]
agregarSinRepetir punto acumulados
    | punto `elem` acumulados = acumulados
    | otherwise = punto : acumulados

rutasPosibles :: String -> String -> [Conexion] -> [[String]]
rutasPosibles origen destino conexiones = buscarRutas origen destino [origen] conexiones

buscarRutas :: String -> String -> [String] -> [Conexion] -> [[String]]
buscarRutas actual destino visitados conexiones
    | actual == destino = [reverse visitados]
    | otherwise = continuarRutas (vecinos actual conexiones) destino visitados conexiones

continuarRutas :: [String] -> String -> [String] -> [Conexion] -> [[String]]
continuarRutas [] _ _ _ = []
continuarRutas (siguiente:resto) destino visitados conexiones
    | siguiente `elem` visitados = continuarRutas resto destino visitados conexiones
    | otherwise = buscarRutas siguiente destino (siguiente:visitados) conexiones ++
                  continuarRutas resto destino visitados conexiones

mostrarRutasPedido :: [Conexion] -> Pedido -> IO ()
mostrarRutasPedido conexiones pedido =
    putStrLn (codPedido pedido ++ " (" ++ origPedido pedido ++ " -> " ++ destPedido pedido ++
              "): " ++ show (rutasPosibles (origPedido pedido) (destPedido pedido) conexiones))

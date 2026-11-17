module CalculoDeRutas.Auxiliares where

import DefinicionDeTipos.Tipos

vecinosHabilitados :: String -> [Conexion] -> [String]
vecinosHabilitados punto conexiones = foldr agregarSinRepetir [] (buscarVecinos punto (filter conexionHabilitada conexiones))

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
    | puntoVisitado punto acumulados = acumulados
    | otherwise = punto : acumulados

puntoVisitado :: String -> [String] -> Bool
puntoVisitado _ [] = False
puntoVisitado punto (actual:resto)
    | punto == actual = True
    | otherwise = puntoVisitado punto resto

buscarRutas :: String -> String -> [String] -> [Conexion] -> [[String]]
buscarRutas actual destino visitados conexiones
    | actual == destino = [reverse visitados]
    | otherwise = continuarRutas (vecinosHabilitados actual conexiones) destino visitados conexiones

continuarRutas :: [String] -> String -> [String] -> [Conexion] -> [[String]]
continuarRutas [] _ _ _ = []
continuarRutas (siguiente:resto) destino visitados conexiones
    | puntoVisitado siguiente visitados = continuarRutas resto destino visitados conexiones
    | otherwise = buscarRutas siguiente destino (siguiente:visitados) conexiones ++
                  continuarRutas resto destino visitados conexiones


module CalculoDeRutas.Rutas
    ( vecinos, rutasPosibles, mostrarRutasPedido
    , conexionHabilitada, buscarVecinos, agregarSinRepetir
    , puntoVisitado, buscarRutas, continuarRutas
    ) where

import DefinicionDeTipos.Tipos
import CalculoDeRutas.Auxiliares

vecinos :: String -> [Conexion] -> [String]
vecinos punto conexiones = vecinosHabilitados punto conexiones

rutasPosibles :: String -> String -> [Conexion] -> [[String]]
rutasPosibles origen destino conexiones = buscarRutas origen destino [origen] conexiones

mostrarRutasPedido :: [Conexion] -> Pedido -> IO ()
mostrarRutasPedido conexiones pedido =
    putStrLn (codPedido pedido ++ " (" ++ codPunto (origPedido pedido) ++ " -> " ++
              codPunto (destPedido pedido) ++ "): " ++
              show (rutasPosibles (codPunto (origPedido pedido)) (codPunto (destPedido pedido)) conexiones))

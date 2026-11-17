module ControlDeAutonomia.Autonomia
    ( EvaluacionAutonomia(..), verificarAutonomia, reporteAutonomia ) where

import DefinicionDeTipos.Tipos
import ControlDeAutonomia.Auxiliares

verificarAutonomia :: Pedido -> Unidad -> [Conexion] -> [Unidad] -> (String, [EvaluacionAutonomia])
verificarAutonomia pedido unidad conexiones unidades
    | null (codPunto (origPedido pedido)) || null (codPunto (destPedido pedido)) || cargaPedido pedido <= 0 =
        ("El pedido debe tener origen, destino y una cantidad positiva de pasajeros.", [])
    | kilometrajeValido (kmAcumulados unidad) == False ||
      kilometrajeValido (kmLimiteMant unidad) == False || kmLimiteMant unidad == 0 =
        ("El kilometraje del bus no es valido.", [])
    | null (filter conexionInvalida conexiones) == False = ("Hay una conexion con datos invalidos.", [])
    | otherwise = evaluarRecorrido unidad unidades (cargaPedido pedido) (distanciaMasCorta pedido conexiones)

reporteAutonomia :: String -> String -> [Pedido] -> [Unidad] -> [Conexion] -> String
reporteAutonomia codigoPedido codigoUnidad pedidos unidades conexiones =
    reporteDeCoincidencias
        [(pedido, unidad, verificarAutonomia pedido unidad conexiones unidades) |
         pedido <- filter ((== codigoPedido) . codPedido) pedidos,
         unidad <- filter ((== codigoUnidad) . codUnidad) unidades]

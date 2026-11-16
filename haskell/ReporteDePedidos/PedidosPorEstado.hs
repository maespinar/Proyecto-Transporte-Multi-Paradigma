module ReporteDePedidos.PedidosPorEstado (contarPedidosPorEstado, resumenEstados) where

contarPedidosPorEstado :: String -> [(String, String, String, Int, String, String, String, String, String)] -> Int
contarPedidosPorEstado _ [] = 0
contarPedidosPorEstado estadoBuscado ((_, _, _, _, _, estado, _, _, _):resto)
    | estado == estadoBuscado = 1 + contarPedidosPorEstado estadoBuscado resto
    | otherwise = contarPedidosPorEstado estadoBuscado resto

resumenEstados :: [(String, String, String, Int, String, String, String, String, String)] -> [(String, Int)]
resumenEstados pedidos =
    [("pendiente", contarPedidosPorEstado "pendiente" pedidos),
     ("asignado", contarPedidosPorEstado "asignado" pedidos),
     ("entregado", contarPedidosPorEstado "entregado" pedidos)]

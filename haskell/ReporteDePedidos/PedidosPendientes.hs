module ReporteDePedidos.PedidosPendientes (pendientesPorPrioridad) where

pendientesPorPrioridad :: [(String, String, String, Int, String, String, String, String, String)] -> [(String, String, String, Int, String, String, String, String, String)]
pendientesPorPrioridad pedidos =
    pendientesDePrioridad "alta" pedidos ++
    pendientesDePrioridad "media" pedidos ++
    pendientesDePrioridad "baja" pedidos

pendientesDePrioridad :: String -> [(String, String, String, Int, String, String, String, String, String)] -> [(String, String, String, Int, String, String, String, String, String)]
pendientesDePrioridad _ [] = []
pendientesDePrioridad prioridadBuscada ((codigo, origen, destino, carga, prioridad, estado, unidad, hora, transbordo):resto)
    | estado == "pendiente" && prioridad == prioridadBuscada =
        (codigo, origen, destino, carga, prioridad, estado, unidad, hora, transbordo) :
        pendientesDePrioridad prioridadBuscada resto
    | otherwise = pendientesDePrioridad prioridadBuscada resto

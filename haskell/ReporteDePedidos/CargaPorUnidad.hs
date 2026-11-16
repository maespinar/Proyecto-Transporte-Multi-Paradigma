module ReporteDePedidos.CargaPorUnidad (cargaTotalAsignada, cargasPorUnidad) where

cargaTotalAsignada :: String -> [(String, String, String, Int, String, String, String, String, String)] -> Int
cargaTotalAsignada _ [] = 0
cargaTotalAsignada "" _ = 0
cargaTotalAsignada unidad ((_, _, _, carga, _, estado, asignada, _, transbordo):resto)
    | estado == "asignado" && (unidad == asignada || unidad == transbordo) =
        carga + cargaTotalAsignada unidad resto
    | otherwise = cargaTotalAsignada unidad resto

cargasPorUnidad :: [String] -> [(String, String, String, Int, String, String, String, String, String)] -> [(String, Int)]
cargasPorUnidad [] _ = []
cargasPorUnidad (unidad:resto) pedidos =
    (unidad, cargaTotalAsignada unidad pedidos) : cargasPorUnidad resto pedidos

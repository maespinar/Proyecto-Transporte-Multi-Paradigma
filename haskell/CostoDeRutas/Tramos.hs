module CostoDeRutas.Tramos (buscarTramo) where

buscarTramo :: String -> String -> [(String, String, Double, Int, String)] -> (String, Double, Int)
buscarTramo origen destino [] =
    ("El tramo " ++ origen ++ " -> " ++ destino ++ " no existe.", 0, 0)
buscarTramo origen destino ((inicio, fin, distancia, tiempo, estado):resto)
    | (inicio == origen && fin == destino) || (inicio == destino && fin == origen) =
        validarTramo origen destino distancia tiempo estado
    | otherwise = buscarTramo origen destino resto

validarTramo :: String -> String -> Double -> Int -> String -> (String, Double, Int)
validarTramo origen destino distancia tiempo estado
    | estado == "cerrada" =
        ("El tramo " ++ origen ++ " -> " ++ destino ++ " esta cerrado.", 0, 0)
    | estado /= "habilitada" =
        ("El tramo " ++ origen ++ " -> " ++ destino ++ " tiene un estado invalido.", 0, 0)
    | distancia <= 0 || tiempo <= 0 =
        ("El tramo " ++ origen ++ " -> " ++ destino ++ " tiene distancia o tiempo invalidos.", 0, 0)
    | otherwise = ("", distancia, tiempo)

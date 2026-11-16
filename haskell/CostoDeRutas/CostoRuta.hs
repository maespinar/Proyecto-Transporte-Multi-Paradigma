module CostoDeRutas.CostoRuta (calcularCostoRuta, calcularCostoConUnidad) where

calcularCostoRuta :: [String] -> [(String, String, Double, Int, String)] -> (String, Double, Int)
calcularCostoRuta [] _ = ("La ruta no puede estar vacia.", 0, 0)
calcularCostoRuta ruta conexiones = calcularTramos ruta conexiones

calcularTramos :: [String] -> [(String, String, Double, Int, String)] -> (String, Double, Int)
calcularTramos [] _ = ("", 0, 0)
calcularTramos [_] _ = ("", 0, 0)
calcularTramos (origen:destino:resto) conexiones =
    sumarTramos (buscarTramo origen destino conexiones)
                (calcularTramos (destino:resto) conexiones)

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

sumarTramos :: (String, Double, Int) -> (String, Double, Int) -> (String, Double, Int)
sumarTramos (mensaje, distancia, tiempo) (mensajeRestante, distanciaRestante, tiempoRestante)
    | mensaje /= "" = (mensaje, 0, 0)
    | mensajeRestante /= "" = (mensajeRestante, 0, 0)
    | otherwise = ("", distancia + distanciaRestante, tiempo + tiempoRestante)

calcularCostoConUnidad :: [String] -> [(String, String, Double, Int, String)] -> Double -> (String, Double, Int, Double)
calcularCostoConUnidad ruta conexiones costoPorKm
    | costoPorKm < 0 =
        ("El costo por kilometro de la unidad es invalido.", 0, 0, 0)
    | otherwise = agregarCosto costoPorKm (calcularCostoRuta ruta conexiones)

agregarCosto :: Double -> (String, Double, Int) -> (String, Double, Int, Double)
agregarCosto costoPorKm (mensaje, distancia, tiempo)
    | mensaje /= "" = (mensaje, 0, 0, 0)
    | otherwise = ("", distancia, tiempo, distancia * costoPorKm)

module CostoDeRutas.DistanciaRuta (calcularDistanciaTotal) where

import CostoDeRutas.Tramos (buscarTramo)

calcularDistanciaTotal :: [String] -> [(String, String, Double, Int, String)] -> (String, Double)
calcularDistanciaTotal [] _ = ("La ruta no puede estar vacia.", 0)
calcularDistanciaTotal [_] _ = ("", 0)
calcularDistanciaTotal (origen:destino:resto) conexiones =
    sumarDistancia (buscarTramo origen destino conexiones)
                  (calcularDistanciaTotal (destino:resto) conexiones)

sumarDistancia :: (String, Double, Int) -> (String, Double) -> (String, Double)
sumarDistancia (mensaje, distancia, _) (mensajeRestante, distanciaRestante)
    | mensaje /= "" = (mensaje, 0)
    | mensajeRestante /= "" = (mensajeRestante, 0)
    | otherwise = ("", distancia + distanciaRestante)

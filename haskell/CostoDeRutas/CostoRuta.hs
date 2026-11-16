module CostoDeRutas.CostoRuta (calcularCostoRuta, calcularCostoConUnidad, calcularDistanciaTotal, calcularTiempoTotal) where

import CostoDeRutas.DistanciaRuta (calcularDistanciaTotal)
import CostoDeRutas.TiempoRuta (calcularTiempoTotal)

calcularCostoRuta :: [String] -> [(String, String, Double, Int, String)] -> (String, Double, Int)
calcularCostoRuta ruta conexiones =
    combinarResultados (calcularDistanciaTotal ruta conexiones) (calcularTiempoTotal ruta conexiones)

combinarResultados :: (String, Double) -> (String, Int) -> (String, Double, Int)
combinarResultados (mensajeDistancia, distancia) (mensajeTiempo, tiempo)
    | mensajeDistancia /= "" = (mensajeDistancia, 0, 0)
    | mensajeTiempo /= "" = (mensajeTiempo, 0, 0)
    | otherwise = ("", distancia, tiempo)

calcularCostoConUnidad :: [String] -> [(String, String, Double, Int, String)] -> Double -> (String, Double, Int, Double)
calcularCostoConUnidad ruta conexiones costoPorKm
    | costoPorKm < 0 =
        ("El costo por kilometro de la unidad es invalido.", 0, 0, 0)
    | otherwise = agregarCosto costoPorKm (calcularCostoRuta ruta conexiones)

agregarCosto :: Double -> (String, Double, Int) -> (String, Double, Int, Double)
agregarCosto costoPorKm (mensaje, distancia, tiempo)
    | mensaje /= "" = (mensaje, 0, 0, 0)
    | otherwise = ("", distancia, tiempo, distancia * costoPorKm)

module CostoDeRutas.TiempoRuta (calcularTiempoTotal) where

import CostoDeRutas.Tramos (buscarTramo)

calcularTiempoTotal :: [String] -> [(String, String, Double, Int, String)] -> (String, Int)
calcularTiempoTotal [] _ = ("La ruta no puede estar vacia.", 0)
calcularTiempoTotal [_] _ = ("", 0)
calcularTiempoTotal (origen:destino:resto) conexiones =
    sumarTiempo (buscarTramo origen destino conexiones)
                (calcularTiempoTotal (destino:resto) conexiones)

sumarTiempo :: (String, Double, Int) -> (String, Int) -> (String, Int)
sumarTiempo (mensaje, _, tiempo) (mensajeRestante, tiempoRestante)
    | mensaje /= "" = (mensaje, 0)
    | mensajeRestante /= "" = (mensajeRestante, 0)
    | otherwise = ("", tiempo + tiempoRestante)

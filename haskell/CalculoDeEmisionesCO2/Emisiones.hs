module CalculoDeEmisionesCO2.Emisiones where

import DefinicionDeTipos.Tipos

calcularEmisionCO2 :: Double -> Unidad -> Double
calcularEmisionCO2 distancia u = distancia * factorEmision u

clasificarEmision :: Double -> String
clasificarEmision e
    | e <= 8.0  = "Nivel bueno"
    | e <= 15.0 = "Nivel moderado"
    | otherwise = "Nivel alto"

mostrarReporteBus :: Double -> Unidad -> IO ()
mostrarReporteBus dist u = do
    let emision = calcularEmisionCO2 dist u
    putStrLn (codUnidad u ++ " | " ++ tipoUnidad u ++ " | Combustible: " ++ show (combustibleUnidad u) ++ " | Emision: " ++ show emision ++ " kg CO2 | " ++ clasificarEmision emision)


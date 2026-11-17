module ControlDeAutonomia.Auxiliares
    ( EvaluacionAutonomia(..), kilometrajeValido, conexionInvalida
    , distanciaMasCorta, evaluarRecorrido, reporteDeCoincidencias
    ) where

import DefinicionDeTipos.Tipos
import CalculoDeRutas.Rutas (rutasPosibles)
import CostoDeRutas.DistanciaRuta (calcularDistanciaTotal)

data EvaluacionAutonomia = EvaluacionAutonomia {
    autonomiaRestante :: Double,
    distanciaRequerida :: Double,
    aptaPorAutonomia :: Bool,
    alternativasAptas :: [(String, Double)]
} deriving (Show, Eq)

kilometrajeValido :: Double -> Bool
kilometrajeValido valor = valor >= 0 && valor - valor == 0

autonomiaSuficiente :: Double -> Double -> Bool
autonomiaSuficiente restante requerida = restante > 0 && requerida <= restante + 0.000000001

conexionInvalida :: Conexion -> Bool
conexionInvalida conexion = kilometrajeValido (distanciaKm conexion) == False ||
                           distanciaKm conexion == 0 || tiempoMin conexion <= 0

unidadApta :: Double -> Int -> Unidad -> Bool
unidadApta requerida pasajeros unidad =
    estadoUnidad unidad == Disponible && capacidadUnidad unidad >= pasajeros &&
    kilometrajeValido (kmAcumulados unidad) && kilometrajeValido (kmLimiteMant unidad) &&
    autonomiaSuficiente (autonomiaDeUnidad unidad) requerida

autonomiaDeUnidad :: Unidad -> Double
autonomiaDeUnidad unidad = kmLimiteMant unidad - kmAcumulados unidad

distanciasDeRutas :: [[String]] -> [(String, String, Double, Int, String)] -> (String, [Double])
distanciasDeRutas [] _ = ("", [])
distanciasDeRutas (ruta:rutas) conexiones =
    combinarDistancias (calcularDistanciaTotal ruta conexiones) (distanciasDeRutas rutas conexiones)

combinarDistancias :: (String, Double) -> (String, [Double]) -> (String, [Double])
combinarDistancias (mensaje, distancia) (mensajeRestante, restantes)
    | mensaje /= "" = (mensaje, [])
    | kilometrajeValido distancia == False = ("La distancia de la ruta excede el rango permitido.", [])
    | mensajeRestante /= "" = (mensajeRestante, [])
    | otherwise = ("", distancia:restantes)

distanciaMasCorta :: Pedido -> [Conexion] -> (String, Double)
distanciaMasCorta pedido conexiones =
    elegirDistancia (distanciasDeRutas
        (rutasPosibles (codPunto (origPedido pedido)) (codPunto (destPedido pedido)) conexiones)
        (tramosHabilitados conexiones))

tramosHabilitados :: [Conexion] -> [(String, String, Double, Int, String)]
tramosHabilitados conexiones =
    [(origConexion conexion, destConexion conexion, distanciaKm conexion, tiempoMin conexion, "habilitada") |
     conexion <- conexiones, estadoConexion conexion == Habilitada]

elegirDistancia :: (String, [Double]) -> (String, Double)
elegirDistancia ("", []) = ("No existe una ruta habilitada para el pedido.", 0)
elegirDistancia ("", primera:resto) = ("", foldr min primera resto)
elegirDistancia (mensaje, _) = (mensaje, 0)

evaluarRecorrido :: Unidad -> [Unidad] -> Int -> (String, Double) -> (String, [EvaluacionAutonomia])
evaluarRecorrido unidad unidades pasajeros ("", requerida) =
    ("", [EvaluacionAutonomia (autonomiaDeUnidad unidad) requerida
          (autonomiaSuficiente (autonomiaDeUnidad unidad) requerida)
          (alternativasPara unidad requerida pasajeros unidades)])
evaluarRecorrido _ _ _ (mensaje, _) = (mensaje, [])

alternativasPara :: Unidad -> Double -> Int -> [Unidad] -> [(String, Double)]
alternativasPara unidad requerida pasajeros unidades =
    [(codUnidad otra, autonomiaDeUnidad otra) |
     otra <- filter (unidadApta requerida pasajeros) unidades, codUnidad otra /= codUnidad unidad]

reporteDeCoincidencias :: [(Pedido, Unidad, (String, [EvaluacionAutonomia]))] -> String
reporteDeCoincidencias ((pedido, unidad, resultado):_) = mostrarEvaluacion pedido unidad resultado
reporteDeCoincidencias [] = "No se pudo consultar la autonomia: el pedido o el bus indicado no existe."

mostrarEvaluacion :: Pedido -> Unidad -> (String, [EvaluacionAutonomia]) -> String
mostrarEvaluacion pedido unidad ("", evaluacion:_) = mostrarResultado pedido unidad evaluacion
mostrarEvaluacion _ _ (mensaje, _) = "No se pudo consultar la autonomia: " ++ mensaje

mostrarKilometros :: Double -> String
mostrarKilometros valor = show (fromInteger (round (valor * 100)) / 100 :: Double)

mostrarResultado :: Pedido -> Unidad -> EvaluacionAutonomia -> String
mostrarResultado pedido unidad evaluacion =
    unlines (["Bus " ++ codUnidad unidad ++ " | Autonomia restante: " ++
              mostrarKilometros (autonomiaRestante evaluacion) ++ " km | Distancia requerida: " ++
              mostrarKilometros (distanciaRequerida evaluacion) ++ " km", estadoEvaluacion evaluacion] ++
             ["Alerta: mantenimiento o recarga al terminar el recorrido." |
              aptaPorAutonomia evaluacion && autonomiaRestante evaluacion <= distanciaRequerida evaluacion] ++
             ["El bus esta ocupado." | estadoUnidad unidad /= Disponible] ++
             ["El bus no tiene capacidad suficiente." | capacidadUnidad unidad < cargaPedido pedido] ++
             recomendaciones pedido unidad evaluacion)

estadoEvaluacion :: EvaluacionAutonomia -> String
estadoEvaluacion evaluacion
    | aptaPorAutonomia evaluacion = "Estado: Apto por autonomia."
    | otherwise = "Estado: No apto (requiere mantenimiento preventivo o recarga, faltan " ++
                  mostrarKilometros (distanciaRequerida evaluacion - autonomiaRestante evaluacion) ++ " km)."

recomendaciones :: Pedido -> Unidad -> EvaluacionAutonomia -> [String]
recomendaciones pedido unidad evaluacion
    | aptaPorAutonomia evaluacion && estadoUnidad unidad == Disponible && capacidadUnidad unidad >= cargaPedido pedido = []
    | otherwise = mostrarAlternativas (alternativasAptas evaluacion)

mostrarAlternativas :: [(String, Double)] -> [String]
mostrarAlternativas [] = ["No hay otras unidades disponibles con capacidad y autonomia suficientes."]
mostrarAlternativas alternativas =
    ["Recomendacion: Asignar unidad " ++ codigo ++ " (autonomia restante: " ++ mostrarKilometros restante ++ " km)." |
     (codigo, restante) <- alternativas]

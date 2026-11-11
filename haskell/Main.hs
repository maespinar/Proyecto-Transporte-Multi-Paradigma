module Main where

import System.IO

data TipoPunto = Paradero | Terminal | Estacion | Intercambio
    deriving (Show, Eq)

data EstadoConexion = Habilitada | Cerrada
    deriving (Show, Eq)

data EstadoUnidad = Disponible | Ocupada
    deriving (Show, Eq)

data TipoCombustible = GNV | Diesel | Electrico
    deriving (Show, Eq)

data PrioridadPedido = Alta | Media | Baja
    deriving (Show, Eq)

data EstadoPedido = Pendiente | Asignado | Entregado
    deriving (Show, Eq)

data Punto = Punto {
    codPunto :: String,
    nomPunto :: String,
    tipoPunto :: TipoPunto,
    zonaPunto :: String,
    permiteTransbordo :: Bool
} deriving (Show, Eq)

data Conexion = Conexion {
    origConexion :: String,
    destConexion :: String,
    distanciaKm :: Double,
    tiempoMin :: Int,
    estadoConexion :: EstadoConexion
} deriving (Show, Eq)

data Unidad = Unidad {
    codUnidad :: String,
    tipoUnidad :: String,
    capacidadUnidad :: Int,
    costoKm :: Double,
    estadoUnidad :: EstadoUnidad,
    combustibleUnidad :: TipoCombustible,
    factorEmision :: Double,
    kmAcumulados :: Double,
    kmLimiteMant :: Double
} deriving (Show, Eq)

data Pedido = Pedido {
    codPedido :: String,
    origPedido :: String,
    destPedido :: String,
    cargaPedido :: Int,
    prioridadPedido :: PrioridadPedido,
    estadoPedido :: EstadoPedido,
    unidadAsignada :: String,
    horaSolicitud :: String,
    unidadTransbordo :: String
} deriving (Show, Eq)

trim :: String -> String
trim = reverse . dropWhile esBlanco . reverse . dropWhile esBlanco
  where esBlanco c = c == ' ' || c == '\t' || c == '\r' || c == '\n'

splitPorComa :: String -> [String]
splitPorComa "" = [""]
splitPorComa (c:cs)
    | c == ','  = "" : resto
    | otherwise = (c : head resto) : tail resto
  where resto = splitPorComa cs

parseTipoPunto :: String -> TipoPunto
parseTipoPunto s = case trim s of
    "Terminal"     -> Terminal
    "Estacion"     -> Estacion
    "Intercambio"  -> Intercambio
    _              -> Paradero

parseEstadoConexion :: String -> EstadoConexion
parseEstadoConexion s = case trim s of
    "cerrada" -> Cerrada
    _         -> Habilitada

parseEstadoUnidad :: String -> EstadoUnidad
parseEstadoUnidad s = case trim s of
    "ocupada" -> Ocupada
    _         -> Disponible

parseCombustible :: String -> TipoCombustible
parseCombustible s = case trim s of
    "DIESEL"    -> Diesel
    "Diesel"    -> Diesel
    "ELECTRICO" -> Electrico
    "Electrico" -> Electrico
    _           -> GNV

parsePrioridad :: String -> PrioridadPedido
parsePrioridad s = case trim s of
    "alta"  -> Alta
    "media" -> Media
    _       -> Baja

parseEstadoPedido :: String -> EstadoPedido
parseEstadoPedido s = case trim s of
    "asignado"  -> Asignado
    "entregado" -> Entregado
    _           -> Pendiente

parseBool :: String -> Bool
parseBool s = trim s == "true"

parsePunto :: String -> Punto
parsePunto linea =
    let cols = map trim (splitPorComa linea)
    in Punto (cols !! 0) (cols !! 1) (parseTipoPunto (cols !! 2)) (cols !! 3) (parseBool (cols !! 4))

parseConexion :: String -> Conexion
parseConexion linea =
    let cols = map trim (splitPorComa linea)
    in Conexion (cols !! 0) (cols !! 1) (read (cols !! 2)) (read (cols !! 3)) (parseEstadoConexion (cols !! 4))

parseUnidad :: String -> Unidad
parseUnidad linea =
    let cols = map trim (splitPorComa linea)
    in Unidad (cols !! 0) (cols !! 1) (read (cols !! 2)) (read (cols !! 3)) (parseEstadoUnidad (cols !! 4)) (parseCombustible (cols !! 5)) (read (cols !! 6)) (read (cols !! 7)) (read (cols !! 8))

parsePedido :: String -> Pedido
parsePedido linea =
    let cols = map trim (splitPorComa linea)
    in Pedido (cols !! 0) (cols !! 1) (cols !! 2) (read (cols !! 3)) (parsePrioridad (cols !! 4)) (parseEstadoPedido (cols !! 5)) (cols !! 6) (cols !! 7) (if length cols > 8 then cols !! 8 else "")

cargarPuntos :: FilePath -> IO [Punto]
cargarPuntos ruta = do
    contenido <- readFile ruta
    let filas = drop 1 (lines contenido)
    return [parsePunto f | f <- filas, not (null (trim f))]

cargarConexiones :: FilePath -> IO [Conexion]
cargarConexiones ruta = do
    contenido <- readFile ruta
    let filas = drop 1 (lines contenido)
    return [parseConexion f | f <- filas, not (null (trim f))]

cargarUnidades :: FilePath -> IO [Unidad]
cargarUnidades ruta = do
    contenido <- readFile ruta
    let filas = drop 1 (lines contenido)
    return [parseUnidad f | f <- filas, not (null (trim f))]

cargarPedidos :: FilePath -> IO [Pedido]
cargarPedidos ruta = do
    contenido <- readFile ruta
    let filas = drop 1 (lines contenido)
    return [parsePedido f | f <- filas, not (null (trim f))]

calcularEmisionCO2 :: Double -> Unidad -> Double
calcularEmisionCO2 distancia u = distancia * factorEmision u

clasificarEmision :: Double -> String
clasificarEmision e
    | e <= 8.0  = "Nivel bueno"
    | e <= 15.0 = "Nivel moderado"
    | otherwise = "Nivel alto"

main :: IO ()
main = do
    putStrLn "================================================"
    putStrLn " MODULO HASKELL: CARGA Y PARADIGMA FUNCIONAL"
    putStrLn "================================================"
    puntos     <- cargarPuntos "data/puntos.csv"
    conexiones <- cargarConexiones "data/conexiones.csv"
    unidades   <- cargarUnidades "data/unidades.csv"
    pedidos    <- cargarPedidos "data/pedidos.csv"

    putStrLn "\n--- Resumen de Carga en Memoria ---"
    putStrLn ("Puntos cargados: " ++ show (length puntos))
    putStrLn ("Conexiones cargadas: " ++ show (length conexiones))
    putStrLn ("Unidades en flota: " ++ show (length unidades))
    putStrLn ("Pedidos cargados: " ++ show (length pedidos))

    putStrLn "\n--- Primeros 3 Puntos ---"
    mapM_ (putStrLn . show) (take 3 puntos)

    putStrLn "\n--- Primeras 3 Unidades ---"
    mapM_ (putStrLn . show) (take 3 unidades)

    putStrLn "\n--- Prueba Funcionalidad de Contaminacion ---"
    let bus = head unidades
    let dist = 14.5
    let emision = calcularEmisionCO2 dist bus
    putStrLn ("Bus: " ++ codUnidad bus ++ " | Distancia: " ++ show dist ++ " km | Emision: " ++ show emision ++ " kg CO2 | " ++ clasificarEmision emision)
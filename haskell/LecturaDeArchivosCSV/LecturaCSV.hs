module LecturaDeArchivosCSV.LecturaCSV where

import DefinicionDeTipos.Tipos

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


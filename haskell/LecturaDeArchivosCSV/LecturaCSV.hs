module LecturaDeArchivosCSV.LecturaCSV where

import DefinicionDeTipos.Tipos
import System.FilePath ((</>), takeDirectory)

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

parsePedido :: [Punto] -> String -> Either String Pedido
parsePedido puntos linea =
    let columnas = map trim (splitPorComa linea)
        completas = if length columnas == 8 then columnas ++ [""] else columnas
    in case completas of
        [codigo, origen, destino, carga, prioridad, estado, unidad, hora, transbordo] -> do
            inicio <- case filter ((== origen) . codPunto) puntos of
                punto:_ -> Right punto
                [] -> Left ("Punto de origen inexistente: " ++ origen)
            fin <- case filter ((== destino) . codPunto) puntos of
                punto:_ -> Right punto
                [] -> Left ("Punto de destino inexistente: " ++ destino)
            pasajeros <- case reads carga of
                [(numero, "")] -> Right numero
                _ -> Left ("Carga invalida en " ++ codigo)
            if codigo == "" || pasajeros <= 0 ||
               (prioridad `elem` ["alta", "media", "baja"]) == False ||
               (estado `elem` ["pendiente", "asignado", "entregado"]) == False
                then Left ("Datos invalidos en la solicitud " ++ codigo)
                else Right (Pedido codigo inicio fin pasajeros (parsePrioridad prioridad)
                            (parseEstadoPedido estado) unidad hora transbordo)
        _ -> Left "Columnas invalidas en una solicitud."

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
    puntos <- cargarPuntos (takeDirectory ruta </> "puntos.csv")
    contenido <- readFile ruta
    let filas = drop 1 (lines contenido)
        resultados = [parsePedido puntos f | f <- filas, trim f /= ""]
    mapM_ (putStrLn . ("Aviso: se omite una solicitud. " ++)) [mensaje | Left mensaje <- resultados]
    return [pedido | Right pedido <- resultados]


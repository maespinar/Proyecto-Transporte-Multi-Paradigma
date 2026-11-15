module DefinicionDeTipos.Tipos where

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


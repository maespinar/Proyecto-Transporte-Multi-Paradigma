module Pruebas where

import qualified Main as S
import Data.List (sort, nub)
import Control.Monad (unless)

comprobar :: String -> Bool -> IO ()
comprobar nombre condicion = unless condicion (error nombre)

main :: IO ()
main = do
    let red = [ S.Conexion "P01" "P02" 1 2 S.Habilitada
              , S.Conexion "P02" "P03" 1 2 S.Habilitada
              , S.Conexion "P01" "P03" 3 8 S.Habilitada
              , S.Conexion "P03" "P04" 1 1 S.Cerrada ]
    comprobar "vecinos en ambos sentidos" (sort (S.vecinos "P02" red) == ["P01", "P03"])
    comprobar "excluir cierres" (S.vecinos "P04" red == [])
    comprobar "sin vecinos duplicados" (S.vecinos "P01" (red ++ red) == S.vecinos "P01" red)
    comprobar "todas las rutas" (sort (S.rutasPosibles "P01" "P03" red) == [["P01", "P02", "P03"], ["P01", "P03"]])
    comprobar "rutas inversas" (length (S.rutasPosibles "P03" "P01" red) == 2)
    comprobar "no repetir puntos" (all (\r -> length r == length (nub r)) (S.rutasPosibles "P01" "P03" red))
    comprobar "destino inaccesible" (null (S.rutasPosibles "P01" "P04" red))
    comprobar "punto desconocido" (null (S.rutasPosibles "X" "P03" red))
    comprobar "ruta trivial" (S.rutasPosibles "P01" "P01" red == [["P01"]])
    comprobar "red vacia" (null (S.rutasPosibles "P01" "P02" []))
    putStrLn "Haskell: 10 pruebas correctas."

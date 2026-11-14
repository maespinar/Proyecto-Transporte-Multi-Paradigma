module Pruebas where

import qualified Main as S
import Data.List (sort)
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
    putStrLn "Haskell: 3 pruebas de vecinos correctas."

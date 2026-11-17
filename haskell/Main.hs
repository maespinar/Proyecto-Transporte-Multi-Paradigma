module Main (main, module Tipos, module LecturaCSV, module Emisiones, module Rutas) where

import DefinicionDeTipos.Tipos as Tipos
import LecturaDeArchivosCSV.LecturaCSV as LecturaCSV
import CalculoDeEmisionesCO2.Emisiones as Emisiones
import CalculoDeRutas.Rutas as Rutas
import MenuPrincipal.Menu (ejecutarMenuHaskell)

main :: IO ()
main = ejecutarMenuHaskell


module DemandaParaderos.DemandaParaderos where
--adicional renzo
import DefinicionDeTipos.Tipos

calcularDemanda :: String -> String -> String -> [Pedido] -> Int
calcularDemanda paradero inicio fin pedidos =
    sum (map cargaPedido (filter (\p ->
        codPunto (origPedido p) == paradero &&
        estadoPedido p == Pendiente &&
        horaSolicitud p >= inicio &&
        horaSolicitud p < fin) pedidos))

mostrarDemanda :: String -> String -> String -> Int -> [Pedido] -> IO ()
mostrarDemanda paradero inicio fin limite pedidos = do
    let total = calcularDemanda paradero inicio fin pedidos

    if total > limite
        then putStrLn (paradero ++ " | " ++ show total ++ " pasajeros | DEMANDA ALTA")
        else putStrLn (paradero ++ " | " ++ show total ++ " pasajeros | Normal")

mostrarTodos :: [Punto] -> [Pedido] -> IO ()
mostrarTodos puntos pedidos = do
    putStrLn "\n--- DEMANDA DE PASAJEROS DE 18:00 A 19:00 ---"
    mapM_ (\p -> mostrarDemanda (codPunto p) "18:00" "19:00" 70 pedidos) puntos

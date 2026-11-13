#pragma once

#include <vector>

struct Punto;
struct Conexion;
struct Unidad;
struct Pedido;

void mostrarMenu();
void ejecutarMenuPrincipal(const std::vector<Punto> &puntos,
                          const std::vector<Conexion> &conexiones,
                          const std::vector<Unidad> &unidades,
                          const std::vector<Pedido> &pedidos);

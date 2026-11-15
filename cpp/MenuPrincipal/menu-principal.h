#pragma once

#include <vector>
#include <string>

struct Punto;
struct Conexion;
struct Unidad;
struct Pedido;

void mostrarMenu();
void ejecutarMenuPrincipal(std::vector<Punto> &puntos,
                          std::vector<Conexion> &conexiones,
                          std::vector<Unidad> &unidades,
                          std::vector<Pedido> &pedidos, const std::string& carpetaDatos = "data");

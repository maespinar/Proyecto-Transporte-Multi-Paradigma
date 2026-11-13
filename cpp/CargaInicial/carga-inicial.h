#pragma once

#include <string>
#include <vector>

struct Punto;
struct Conexion;
struct Unidad;
struct Pedido;

bool cargarPuntos(std::string ruta, std::vector<Punto> &puntos);
bool cargarConexiones(std::string ruta, std::vector<Conexion> &conexiones);
bool cargarUnidades(std::string ruta, std::vector<Unidad> &unidades);
bool cargarPedidos(std::string ruta, std::vector<Pedido> &pedidos,
                   const std::vector<Punto> &puntos,
                   const std::vector<Unidad> &unidades);
bool cargarDatosIniciales(std::vector<Punto> &puntos,
                         std::vector<Conexion> &conexiones,
                         std::vector<Unidad> &unidades,
                         std::vector<Pedido> &pedidos);

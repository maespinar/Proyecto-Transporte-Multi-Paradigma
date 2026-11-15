#pragma once

#include <string>
#include <vector>

struct Conexion;
struct Unidad;
struct Pedido;

double calcularDistanciaRuta(std::string orig, std::string dest,
                             const std::vector<Conexion> &conexiones);
std::string clasificarNivelEmision(double emision);
void procesarCalculoEmision(const std::vector<Pedido> &pedidos,
                            const std::vector<Unidad> &unidades,
                            const std::vector<Conexion> &conexiones);

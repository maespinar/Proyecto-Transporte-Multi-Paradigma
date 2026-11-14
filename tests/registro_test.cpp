#include "../cpp/Registro/registro.h"
#include <cassert>
#include <iostream>
#include <limits>

int main() {
    std::string error;
    std::vector<Punto> puntos;
    Punto a{"P01", "Inicio", TERMINAL, "Ate", false};
    Punto b{"P02", "Intercambio", INTERCAMBIO, "Surco", true};
    Punto c{"P03", "Destino", PARADERO, "Surco", false};
    assert(registrarPunto(a, puntos, error));
    assert(!registrarPunto(a, puntos, error));
    Punto invalido = a;
    invalido.codigo = "";
    assert(!registrarPunto(invalido, puntos, error));
    invalido.codigo = "p04";
    assert(!registrarPunto(invalido, puntos, error));
    invalido.codigo = "P04";
    invalido.nombre = "nombre,con,comas";
    assert(!registrarPunto(invalido, puntos, error));
    assert(registrarPunto(b, puntos, error));
    assert(registrarPunto(c, puntos, error));
    std::vector<Conexion> conexiones;
    Conexion tramo{"P01", "P02", 2, 14, HABILITADA};
    assert(registrarConexion(tramo, puntos, conexiones, error));
    assert(!registrarConexion(tramo, puntos, conexiones, error));
    tramo.origen = "P03";
    tramo.destino = "P03";
    assert(!registrarConexion(tramo, puntos, conexiones, error));
    tramo.origen = "P01";
    tramo.distanciaKm = std::numeric_limits<double>::infinity();
    tramo.tiempoMin = 1;
    assert(!registrarConexion(tramo, puntos, conexiones, error));
    Conexion inverso{"P02", "P01", 2, 14, HABILITADA};
    assert(!registrarConexion(inverso, puntos, conexiones, error));
    tramo.destino = "X";
    assert(!registrarConexion(tramo, puntos, conexiones, error));
    tramo.destino = "P03";
    tramo.distanciaKm = 0;
    assert(!registrarConexion(tramo, puntos, conexiones, error));
    tramo.distanciaKm = 1;
    tramo.tiempoMin = -1;
    assert(!registrarConexion(tramo, puntos, conexiones, error));
    std::vector<Unidad> unidades;
    Unidad u{"U01", "Bus", 60, 5, DISPONIBLE, GNV, 0.4, 0, 500};
    assert(registrarUnidad(u, unidades, error));
    assert(!registrarUnidad(u, unidades, error));
    u.codigo = "U02";
    u.costoKm = -1;
    assert(!registrarUnidad(u, unidades, error));
    u.costoKm = 5;
    u.factorEmision = -1;
    assert(!registrarUnidad(u, unidades, error));
    u.factorEmision = 0.4;
    u.codigo = "U02";
    u.capacidad = 0;
    assert(!registrarUnidad(u, unidades, error));
    u.capacidad = 60;
    assert(registrarUnidad(u, unidades, error));
    std::vector<Pedido> pedidos;
    Pedido p{"S001", a, c, 45, ALTA, PENDIENTE, {}, "18:00", {}};
    assert(registrarPedido(p, puntos, pedidos, error));
    assert(!registrarPedido(p, puntos, pedidos, error));
    p.codigo = "S002";
    p.carga = 0;
    assert(!registrarPedido(p, puntos, pedidos, error));
    p.carga = 45;
    p.destino.codigo = "X";
    assert(!registrarPedido(p, puntos, pedidos, error));
    p.destino = c;
    p.horaSolicitud = "25:70";
    assert(!registrarPedido(p, puntos, pedidos, error));

    cout << "C++: registro con validaciones correcto.\n";
}

#include "../cpp/Registro/registro.h"
#include "../cpp/Transbordos/transbordos.h"
#include <cassert>
#include <iostream>

using namespace std;

int main() {
    string error;
    vector<Punto> puntos;
    Punto a{"P01", "Inicio", TERMINAL, "Ate", false};
    Punto b{"P02", "Intercambio", INTERCAMBIO, "Surco", true};
    Punto c{"P03", "Destino", PARADERO, "Surco", false};
    assert(registrarPunto(a, puntos, error));
    assert(registrarPunto(b, puntos, error));
    assert(registrarPunto(c, puntos, error));
    vector<Conexion> conexiones{{"P01", "P02", 2, 14, HABILITADA}};
    Unidad u{"U01", "Bus", 60, 5, DISPONIBLE, GNV, 0.4, 0, 500};
    vector<Unidad> unidades{u};
    u.codigo = "U02";
    unidades.push_back(u);
    Pedido p{"S001", a, c, 45, ALTA, PENDIENTE, {}, "18:00", {}};
    vector<Pedido> pedidos{p};
    PlanTransbordo plan{"P02", {"P01", "P02"}, {"P02", "P03"}, 14, 10, "P01", "P03"};
    assert(asignarTransbordo(pedidos[0], unidades, puntos, conexiones, plan, 5, error) == false); // falta segundo tramo
    conexiones.push_back({"P02", "P03", 2, 10, HABILITADA});
    auto planInvalido = plan;
    planInvalido.punto = "P01";
    assert(!asignarTransbordo(pedidos[0], unidades, puntos, conexiones, planInvalido, 5, error));
    planInvalido = plan;
    planInvalido.tiempoPrimero = 13;
    assert(!asignarTransbordo(pedidos[0], unidades, puntos, conexiones, planInvalido, 5, error));
    assert(!asignarTransbordo(pedidos[0], unidades, puntos, conexiones, plan, -1, error));
    unidades[1].estado = OCUPADA;
    assert(!asignarTransbordo(pedidos[0], unidades, puntos, conexiones, plan, 5, error));
    unidades[1].estado = DISPONIBLE;
    unidades[1].capacidad = 44;
    assert(!asignarTransbordo(pedidos[0], unidades, puntos, conexiones, plan, 5, error));
    assert(pedidos[0].estado == PENDIENTE && unidades[0].estado == DISPONIBLE);
    unidades[1].capacidad = 60;
    assert(asignarTransbordo(pedidos[0], unidades, puntos, conexiones, plan, 5, error));
    assert(pedidos[0].estado == ASIGNADO);
    assert(pedidos[0].unidadAsignada.codigo == "U01");
    assert(pedidos[0].unidadTransbordo.codigo == "U02");
    assert(unidades[0].estado == OCUPADA && unidades[1].estado == OCUPADA);
    assert(!asignarTransbordo(pedidos[0], unidades, puntos, conexiones, plan, 5, error));
    assert(plan.tiempoPrimero + plan.tiempoSegundo + 5 == 29);
    cout << "C++: transbordo y asignacion correctos.\n";
}

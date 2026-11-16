
#ifndef REPORTE_GENERAL_H
#define REPORTE_GENERAL_H

#include <vector>
#include "../data/structures.h"
using namespace std;

void mostrarReporteGeneral(vector<Punto> &puntos,
                          vector<Conexion> &conexiones,
                          vector<Unidad> &unidades,
                          vector<Pedido> &pedidos);

#endif

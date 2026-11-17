#pragma once

#include "../data/structures.h"
#include <vector>

using namespace std;

bool asignarPedidoUnidad(Pedido& pedido, Unidad& unidadAsignada, const string& horaSolicitud,
                        const vector<Conexion>& conexiones);

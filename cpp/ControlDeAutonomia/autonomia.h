#pragma once

#include "../data/structures.h"
#include <vector>

struct EvaluacionAutonomia {
    double restante;
    double requerida;
    bool apta;
};

bool verificarAutonomia(const Pedido& pedido, const Unidad& unidad,
                        const vector<Conexion>& conexiones,
                        EvaluacionAutonomia& evaluacion);
void mostrarAutonomia(const Pedido& pedido, const Unidad& unidad,
                      const EvaluacionAutonomia& evaluacion, const vector<Unidad>& unidades);
void procesarAutonomia(vector<Pedido>& pedidos, vector<Unidad>& unidades,
                       const vector<Conexion>& conexiones, const string& carpetaDatos, bool asignar);

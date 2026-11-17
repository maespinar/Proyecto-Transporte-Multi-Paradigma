#pragma once

#include "../data/structures.h"
#include <vector>

bool kilometrajeValido(double valor);
bool autonomiaSuficiente(double restante, double requerida);
bool buscarDistancia(const string& actual, const string& destino,
                     const vector<Conexion>& conexiones, vector<string>& visitados,
                     double acumulada, double& menor);

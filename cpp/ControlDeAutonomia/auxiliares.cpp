#include "auxiliares.h"
#include <iostream>

bool kilometrajeValido(double valor) {
    return valor >= 0 && valor - valor == 0;
}

bool autonomiaSuficiente(double restante, double requerida) {
    double toleranciaRedondeo = 0.000000001;
    return restante > 0 && requerida <= restante + toleranciaRedondeo;
}

bool buscarDistancia(const string& actual, const string& destino,
                     const vector<Conexion>& conexiones, vector<string>& visitados,
                     double acumulada, double& menor) {
    if (actual == destino) {
        if (menor < 0 || acumulada < menor) menor = acumulada;
        return true;
    }
    if (menor >= 0 && acumulada >= menor) return true;
    for (const Conexion& conexion : conexiones) {
        if (conexion.estado != HABILITADA) continue;
        string siguiente;
        if (conexion.origen == actual) siguiente = conexion.destino;
        else if (conexion.destino == actual) siguiente = conexion.origen;
        if (siguiente.empty()) continue;
        bool visitado = false;
        for (const string& codigo : visitados) {
            if (codigo == siguiente) visitado = true;
        }
        if (visitado) continue;
        double distancia = acumulada + conexion.distanciaKm;
        if (!kilometrajeValido(distancia)) {
            cout << "La distancia de la ruta excede el rango permitido.\n";
            return false;
        }
        visitados.push_back(siguiente);
        bool resultado = buscarDistancia(siguiente, destino, conexiones, visitados, distancia, menor);
        visitados.pop_back();
        if (!resultado) return false;
    }
    return true;
}

#ifndef TRANSBORDOS_H
#define TRANSBORDOS_H

#include "../data/structures.h"
#include <vector>

using namespace std;

struct PlanTransbordo {
    string punto;
    vector<string> rutaPrimera;
    vector<string> rutaSegunda;
    int tiempoPrimero;
    int tiempoSegundo;
    string bloqueoOrigen;
    string bloqueoDestino;
};

bool consultarTransbordo(const Pedido& pedido, const vector<Punto>& puntos,
                        const vector<Conexion>& conexiones, string script,
                        PlanTransbordo& plan, string& error);
bool asignarTransbordo(Pedido& pedido, vector<Unidad>& unidades,
                      const vector<Punto>& puntos, const vector<Conexion>& conexiones,
                      const PlanTransbordo& plan, int espera, string& error);
void procesarTransbordo(vector<Pedido>& pedidos, vector<Unidad>& unidades,
                       const vector<Punto>& puntos, const vector<Conexion>& conexiones,
                       string carpetaDatos, string script);

#endif

#ifndef REGISTRO_H
#define REGISTRO_H

#include "../data/structures.h"
#include <vector>

using namespace std;

bool codigoValido(string codigo);
bool textoCSVValido(string texto);
bool existePunto(string codigo, const vector<Punto>& puntos);
bool horaValida(string hora);
bool registrarPunto(const Punto& nuevo, vector<Punto>& puntos, string& error);
bool registrarConexion(const Conexion& nueva, const vector<Punto>& puntos, vector<Conexion>& conexiones, string& error);
bool registrarUnidad(const Unidad& nueva, vector<Unidad>& unidades, string& error);
bool registrarPedido(const Pedido& nuevo, const vector<Punto>& puntos, vector<Pedido>& pedidos, string& error);
void procesarRegistro(vector<Punto>& puntos, vector<Conexion>& conexiones,
                     vector<Unidad>& unidades, vector<Pedido>& pedidos, string carpetaDatos);

#endif

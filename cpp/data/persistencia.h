#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "structures.h"
#include <vector>

using namespace std;

struct CambioCSV {
    string ruta;
    string contenido;
};

string csvPuntos(const vector<Punto>& puntos);
string csvConexiones(const vector<Conexion>& conexiones);
string csvUnidades(const vector<Unidad>& unidades);
string csvPedidos(const vector<Pedido>& pedidos);
bool existeArchivo(string ruta);
bool escribirCSV(string ruta, string contenido);
bool guardarCambiosCSV(const vector<CambioCSV>& cambios, string& error);

#endif

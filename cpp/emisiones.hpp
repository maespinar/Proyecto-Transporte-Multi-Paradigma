#ifndef EMISIONES_HPP
#define EMISIONES_HPP

#include "data/structures.h"
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

double calcularDistanciaRuta(string orig, string dest,
                             const vector<Conexion> &conexiones) {
  if (orig == dest)
    return 0.0;
  vector<string> visitados;
  queue<pair<string, double>> q;
  q.push({orig, 0.0});
  visitados.push_back(orig);

  while (!q.empty()) {
    string actual = q.front().first;
    double distActual = q.front().second;
    q.pop();

    if (actual == dest) {
      return distActual;
    }

    for (int i = 0; i < conexiones.size(); i++) {
      if (conexiones[i].estado == CERRADA)
        continue;

      string vecino = "";
      if (conexiones[i].origen == actual) {
        vecino = conexiones[i].destino;
      } else if (conexiones[i].destino == actual) {
        vecino = conexiones[i].origen;
      }

      if (!vecino.empty()) {
        bool yaVisitado = false;
        for (int j = 0; j < visitados.size(); j++) {
          if (visitados[j] == vecino) {
            yaVisitado = true;
            break;
          }
        }
        if (!yaVisitado) {
          visitados.push_back(vecino);
          q.push({vecino, distActual + conexiones[i].distanciaKm});
        }
      }
    }
  }
  return -1.0;
}

string clasificarNivelEmision(double emision) {
  if (emision <= 8.0)
    return "Nivel bueno";
  if (emision <= 15.0)
    return "Nivel moderado";
  return "Nivel alto";
}

void procesarCalculoEmision(const vector<Pedido> &pedidos,
                            const vector<Unidad> &unidades,
                            const vector<Conexion> &conexiones) {
  string codPedido, codUnidad;
  cout << "\n--- CALCULAR EMISION DE CO2 POR RUTA ---" << endl;
  cout << "Ingrese codigo de pedido (ej. S001): ";
  cin >> codPedido;

  int posPedido = -1;
  for (int i = 0; i < pedidos.size(); i++) {
    if (pedidos[i].codigo == codPedido) {
      posPedido = i;
      break;
    }
  }

  if (posPedido == -1) {
    cout << "Error: El pedido " << codPedido << " no existe." << endl;
    return;
  }

  Pedido p = pedidos[posPedido];
  cout << "Pedido encontrado: " << p.origen.codigo << " -> " << p.destino.codigo
       << " (" << p.carga << " pasajeros)" << endl;
  cout << "Ingrese codigo de bus a evaluar (ej. U01, o '-' para usar el "
          "asignado "
       << p.unidadAsignada.codigo << "): ";
  cin >> codUnidad;

  Unidad u;
  u.codigo = "";
  if (codUnidad == "-" || codUnidad.empty()) {
    u = p.unidadAsignada;
  } else {
    for (int i = 0; i < unidades.size(); i++) {
      if (unidades[i].codigo == codUnidad) {
        u = unidades[i];
        break;
      }
    }
  }

  if (u.codigo.empty()) {
    cout << "Error: La unidad ingresada no es valida." << endl;
    return;
  }

  double distancia =
      calcularDistanciaRuta(p.origen.codigo, p.destino.codigo, conexiones);
  if (distancia < 0) {
    cout << "No se encontro una ruta habilitada entre " << p.origen.codigo
         << " y " << p.destino.codigo << endl;
    return;
  }

  double emisionTotal = distancia * u.factorEmision;
  string nivel = clasificarNivelEmision(emisionTotal);
  string comb = "GNV";
  if (u.tipoCombustible == DIESEL)
    comb = "Diesel";
  else if (u.tipoCombustible == ELECTRICO)
    comb = "Electrico";

  cout << "\n>>> RESULTADO DE EVALUACION AMBIENTAL <<<" << endl;
  cout << fixed << setprecision(2);
  cout << "Bus " << u.codigo << ", Combustible " << comb << ", Distancia "
       << distancia << " km, Emision " << emisionTotal << " kg CO2, " << nivel
       << endl;
}

#endif
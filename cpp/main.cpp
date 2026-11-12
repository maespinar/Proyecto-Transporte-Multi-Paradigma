#include "data/structures.h"
#include "emisiones.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string limpiarEspacios(string texto) {
  int inicio = 0;
  while (inicio < texto.length() &&
         (texto[inicio] == ' ' || texto[inicio] == '\t' ||
          texto[inicio] == '\r')) {
    inicio++;
  }
  int fin = texto.length() - 1;
  while (fin >= 0 && (texto[fin] == ' ' || texto[fin] == '\t' ||
                      texto[fin] == '\r' || texto[fin] == '\n')) {
    fin--;
  }
  if (inicio > fin)
    return "";
  return texto.substr(inicio, fin - inicio + 1);
}

TipoPunto stringToTipoPunto(string s) {
  if (s == "Terminal" || s == "TERMINAL")
    return TERMINAL;
  if (s == "Estacion" || s == "ESTACION")
    return ESTACION;
  if (s == "Intercambio" || s == "INTERCAMBIO")
    return INTERCAMBIO;
  return PARADERO;
}

EstadoConexion stringToEstadoConexion(string s) {
  if (s == "cerrada" || s == "CERRADA")
    return CERRADA;
  return HABILITADA;
}

EstadoUnidad stringToEstadoUnidad(string s) {
  if (s == "ocupada" || s == "OCUPADA")
    return OCUPADA;
  return DISPONIBLE;
}

TipoCombustible stringToTipoCombustible(string s) {
  if (s == "DIESEL" || s == "Diesel")
    return DIESEL;
  if (s == "ELECTRICO" || s == "Electrico")
    return ELECTRICO;
  return GNV;
}

string combustibleToString(TipoCombustible c) {
  if (c == DIESEL)
    return "Diesel";
  if (c == ELECTRICO)
    return "Electrico";
  return "GNV";
}

PrioridadPedido stringToPrioridad(string s) {
  if (s == "alta" || s == "ALTA")
    return ALTA;
  if (s == "media" || s == "MEDIA")
    return MEDIA;
  return BAJA;
}

EstadoPedido stringToEstadoPedido(string s) {
  if (s == "asignado" || s == "ASIGNADO")
    return ASIGNADO;
  if (s == "entregado" || s == "ENTREGADO")
    return ENTREGADO;
  return PENDIENTE;
}

Punto buscarPunto(string cod, const vector<Punto> &puntos) {
  for (int i = 0; i < puntos.size(); i++) {
    if (puntos[i].codigo == cod)
      return puntos[i];
  }
  Punto vacio;
  vacio.codigo = cod;
  vacio.nombre = "Desconocido";
  return vacio;
}

Unidad buscarUnidad(string cod, const vector<Unidad> &unidades) {
  for (int i = 0; i < unidades.size(); i++) {
    if (unidades[i].codigo == cod)
      return unidades[i];
  }
  Unidad vacia;
  vacia.codigo = "";
  vacia.tipo = "Sin asignar";
  vacia.factorEmision = 0.0;
  return vacia;
}

bool cargarPuntos(string ruta, vector<Punto> &puntos) {
  ifstream archivo(ruta);
  if (!archivo.is_open())
    return false;
  string linea;
  getline(archivo, linea);
  while (getline(archivo, linea)) {
    if (linea.empty())
      continue;
    stringstream ss(linea);
    string cod, nom, tipoStr, zona, transbordoStr;
    getline(ss, cod, ',');
    getline(ss, nom, ',');
    getline(ss, tipoStr, ',');
    getline(ss, zona, ',');
    getline(ss, transbordoStr, ',');

    Punto p;
    p.codigo = limpiarEspacios(cod);
    p.nombre = limpiarEspacios(nom);
    p.tipo = stringToTipoPunto(limpiarEspacios(tipoStr));
    p.zona = limpiarEspacios(zona);
    p.permiteTransbordo = (limpiarEspacios(transbordoStr) == "true");
    puntos.push_back(p);
  }
  archivo.close();
  return true;
}

bool cargarConexiones(string ruta, vector<Conexion> &conexiones) {
  ifstream archivo(ruta);
  if (!archivo.is_open())
    return false;
  string linea;
  getline(archivo, linea);
  while (getline(archivo, linea)) {
    if (linea.empty())
      continue;
    stringstream ss(linea);
    string orig, dest, distStr, tiempoStr, estStr;
    getline(ss, orig, ',');
    getline(ss, dest, ',');
    getline(ss, distStr, ',');
    getline(ss, tiempoStr, ',');
    getline(ss, estStr, ',');

    Conexion c;
    c.origen = limpiarEspacios(orig);
    c.destino = limpiarEspacios(dest);
    c.distanciaKm = stod(limpiarEspacios(distStr));
    c.tiempoMin = stoi(limpiarEspacios(tiempoStr));
    c.estado = stringToEstadoConexion(limpiarEspacios(estStr));
    conexiones.push_back(c);
  }
  archivo.close();
  return true;
}

bool cargarUnidades(string ruta, vector<Unidad> &unidades) {
  ifstream archivo(ruta);
  if (!archivo.is_open())
    return false;
  string linea;
  getline(archivo, linea);
  while (getline(archivo, linea)) {
    if (linea.empty())
      continue;
    stringstream ss(linea);
    string cod, tipo, capStr, costoStr, estStr, combStr, emisionStr, kmAcumStr,
        kmLimStr;
    getline(ss, cod, ',');
    getline(ss, tipo, ',');
    getline(ss, capStr, ',');
    getline(ss, costoStr, ',');
    getline(ss, estStr, ',');
    getline(ss, combStr, ',');
    getline(ss, emisionStr, ',');
    getline(ss, kmAcumStr, ',');
    getline(ss, kmLimStr, ',');

    Unidad u;
    u.codigo = limpiarEspacios(cod);
    u.tipo = limpiarEspacios(tipo);
    u.capacidad = stoi(limpiarEspacios(capStr));
    u.costoKm = stod(limpiarEspacios(costoStr));
    u.estado = stringToEstadoUnidad(limpiarEspacios(estStr));
    u.tipoCombustible = stringToTipoCombustible(limpiarEspacios(combStr));
    u.factorEmision = stod(limpiarEspacios(emisionStr));
    u.kmAcumulados = stod(limpiarEspacios(kmAcumStr));
    u.kmLimiteMant = stod(limpiarEspacios(kmLimStr));
    unidades.push_back(u);
  }
  archivo.close();
  return true;
}

bool cargarPedidos(string ruta, vector<Pedido> &pedidos,
                   const vector<Punto> &puntos,
                   const vector<Unidad> &unidades) {
  ifstream archivo(ruta);
  if (!archivo.is_open())
    return false;
  string linea;
  getline(archivo, linea);
  while (getline(archivo, linea)) {
    if (linea.empty())
      continue;
    stringstream ss(linea);
    string cod, orig, dest, cargaStr, prioStr, estStr, uniAsig, hora, uniTrans;
    getline(ss, cod, ',');
    getline(ss, orig, ',');
    getline(ss, dest, ',');
    getline(ss, cargaStr, ',');
    getline(ss, prioStr, ',');
    getline(ss, estStr, ',');
    getline(ss, uniAsig, ',');
    getline(ss, hora, ',');
    getline(ss, uniTrans, ',');

    Pedido ped;
    ped.codigo = limpiarEspacios(cod);
    ped.origen = buscarPunto(limpiarEspacios(orig), puntos);
    ped.destino = buscarPunto(limpiarEspacios(dest), puntos);
    ped.carga = stoi(limpiarEspacios(cargaStr));
    ped.prioridad = stringToPrioridad(limpiarEspacios(prioStr));
    ped.estado = stringToEstadoPedido(limpiarEspacios(estStr));
    ped.unidadAsignada = buscarUnidad(limpiarEspacios(uniAsig), unidades);
    ped.horaSolicitud = limpiarEspacios(hora);
    ped.unidadTransbordo = buscarUnidad(limpiarEspacios(uniTrans), unidades);
    pedidos.push_back(ped);
  }
  archivo.close();
  return true;
}

void mostrarMenu() {
  cout << "\n==========================================" << endl;
  cout << " SISTEMA DE TRANSPORTE - CORREDOR ROJO    " << endl;
  cout << "==========================================" << endl;
  cout << "1. Ver resumen de datos cargados" << endl;
  cout << "2. Listar paraderos y terminales" << endl;
  cout << "3. Listar flota de buses (unidades)" << endl;
  cout << "4. Listar conexiones de ruta" << endl;
  cout << "5. Listar pedidos de transporte" << endl;
  cout << "6. Calcular emision de CO2 de bus por ruta" << endl;
  cout << "7. Salir" << endl;
  cout << "Seleccione una opcion: ";
}

int main() {
  vector<Punto> puntos;
  vector<Conexion> conexiones;
  vector<Unidad> unidades;
  vector<Pedido> pedidos;

  cout << "Iniciando carga de datos desde archivos CSV..." << endl;

  if (!cargarPuntos("../data/puntos.csv", puntos)) {
    cargarPuntos("data/puntos.csv", puntos);
  }
  if (!cargarConexiones("../data/conexiones.csv", conexiones)) {
    cargarConexiones("data/conexiones.csv", conexiones);
  }
  if (!cargarUnidades("../data/unidades.csv", unidades)) {
    cargarUnidades("data/unidades.csv", unidades);
  }
  if (!cargarPedidos("../data/pedidos.csv", pedidos, puntos, unidades)) {
    cargarPedidos("data/pedidos.csv", pedidos, puntos, unidades);
  }

  if (puntos.empty() || conexiones.empty() || unidades.empty()) {
    cout << "Error critico: No se pudieron cargar los datos de la carpeta data/"
         << endl;
    return 1;
  }

  cout << "Carga completada exitosamente." << endl;

  int opcion = 0;
  while (opcion != 7) {
    mostrarMenu();
    cin >> opcion;

    if (opcion == 1) {
      cout << "\n--- RESUMEN DEL SISTEMA ---" << endl;
      cout << "Puntos registrados: " << puntos.size() << endl;
      cout << "Conexiones activas: " << conexiones.size() << endl;
      cout << "Unidades en flota: " << unidades.size() << endl;
      cout << "Pedidos registrados: " << pedidos.size() << endl;
    } else if (opcion == 2) {
      cout << "\n--- LISTA DE PUNTOS ---" << endl;
      for (int i = 0; i < puntos.size(); i++) {
        cout << puntos[i].codigo << " - " << puntos[i].nombre << " ("
             << puntos[i].zona << ")" << endl;
      }
    } else if (opcion == 3) {
      cout << "\n--- FLOTA DE BUSES ---" << endl;
      for (int i = 0; i < unidades.size(); i++) {
        cout << unidades[i].codigo << " | " << unidades[i].tipo
             << " | Capacidad: " << unidades[i].capacidad << " | Costo/km: S/"
             << unidades[i].costoKm << " | Combustible: "
             << combustibleToString(unidades[i].tipoCombustible)
             << " | Emision: " << unidades[i].factorEmision << " kg/km" << endl;
      }
    } else if (opcion == 4) {
      cout << "\n--- CONEXIONES REGISTRADAS ---" << endl;
      for (int i = 0; i < conexiones.size(); i++) {
        cout << conexiones[i].origen << " -> " << conexiones[i].destino
             << " | Distancia: " << conexiones[i].distanciaKm
             << " km | Tiempo: " << conexiones[i].tiempoMin << " min" << endl;
      }
    } else if (opcion == 5) {
      cout << "\n--- PEDIDOS DE TRANSPORTE ---" << endl;
      for (int i = 0; i < pedidos.size(); i++) {
        cout << pedidos[i].codigo << " | Ruta: " << pedidos[i].origen.codigo
             << " a " << pedidos[i].destino.codigo
             << " | Carga: " << pedidos[i].carga
             << " pasajeros | Hora: " << pedidos[i].horaSolicitud << endl;
      }
    } else if (opcion == 6) {
      procesarCalculoEmision(pedidos, unidades, conexiones);
    } else if (opcion == 7) {
      cout << "Cerrando sistema..." << endl;
    } else {
      cout << "Opcion no valida, intente nuevamente." << endl;
    }
  }

  return 0;
}
#include "menu-principal.h"
#include "../data/structures.h"
#include "../CalculoDeEmisionesCO2/emisiones.h"
#include "../Registro/registro.h"
#include "../Transbordos/transbordos.h"
#include "../ControlDeAutonomia/autonomia.h"
#include <iostream>
#include <string>
#include "../Consultas/consultas.h"
#include "../ReporteGeneral/reporte-general.h"

using namespace std;

string combustibleToString(TipoCombustible c) {
  if (c == DIESEL)
    return "Diesel";
  if (c == ELECTRICO)
    return "Electrico";
  return "GNV";
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
  cout << "8. Registrar punto, conexion, unidad o solicitud" << endl;
  cout << "9. Planificar y asignar transbordo ante contingencia" << endl;
  cout << "10. Consultas basicas" << endl;//renzo
  cout << "11. Reporte general" << endl;//renzo
  cout << "12. Verificar autonomia y mantenimiento de un bus" << endl;
  cout << "13. Asignar pedido con control de autonomia" << endl;
  cout << "Seleccione una opcion: ";
}

void ejecutarMenuPrincipal(vector<Punto> &puntos,
                          vector<Conexion> &conexiones,
                          vector<Unidad> &unidades,
                          vector<Pedido> &pedidos, const string& carpetaDatos) {
  int opcion = 0;
  while (opcion != 7) {
    mostrarMenu();
    cin >> opcion;
    if (!cin) {
      if (cin.eof()) break;
      cin.clear();
      string restoLinea;
      getline(cin, restoLinea);
      cout << "Ingrese una opcion numerica." << endl;
      continue;
    }

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
    } else if (opcion == 8) {
      procesarRegistro(puntos, conexiones, unidades, pedidos, carpetaDatos);
    } else if (opcion == 9) {
      procesarTransbordo(pedidos, unidades, puntos, conexiones, carpetaDatos,
                         "prolog/consulta_transbordo.pl");
    } else if (opcion == 10) {
      realizarConsultas(puntos, conexiones, pedidos);
    } else if (opcion == 11) {
      mostrarReporteGeneral(puntos, conexiones, unidades, pedidos);
    } else if (opcion == 12 || opcion == 13) {
      procesarAutonomia(pedidos, unidades, conexiones, carpetaDatos, opcion == 13);
    } else {
      cout << "Opcion no valida, intente nuevamente." << endl;
    }
  }

}


#include <iostream>
#include "reporte-general.h"
using namespace std;

void mostrarReporteGeneral(vector<Punto> &puntos,
                          vector<Conexion> &conexiones,
                          vector<Unidad> &unidades,
                          vector<Pedido> &pedidos) {

    int habilitadas = 0;
    int cerradas = 0;
    int disponibles = 0;
    int pendientes = 0;
    int asignados = 0;
    int entregados = 0;

    for (int i = 0; i < conexiones.size(); i++) {
        if (conexiones[i].estado == HABILITADA) {
            habilitadas++;
        } else {
            cerradas++;
        }
    }

    for (int i = 0; i < unidades.size(); i++) {
        if (unidades[i].estado == DISPONIBLE) {
            disponibles++;
        }
    }

    for (int i = 0; i < pedidos.size(); i++) {
        if (pedidos[i].estado == PENDIENTE) {
            pendientes++;
        } else if (pedidos[i].estado == ASIGNADO) {
            asignados++;
        } else {
            entregados++;
        }
    }

    cout << "\n--- REPORTE GENERAL ---" << endl;
    cout << "Total de puntos: " << puntos.size() << endl;
    cout << "Conexiones habilitadas: " << habilitadas << endl;
    cout << "Conexiones cerradas: " << cerradas << endl;
    cout << "Unidades disponibles: " << disponibles << endl;
    cout << "Pedidos pendientes: " << pendientes << endl;
    cout << "Pedidos asignados: " << asignados << endl;
    cout << "Pedidos entregados: " << entregados << endl;

    int mayor = 0;
    string bus = "Ninguno";

    for (int i = 0; i < unidades.size(); i++) {
        int cantidad = 0;

        for (int j = 0; j < pedidos.size(); j++) {
            if (pedidos[j].unidadAsignada.codigo == unidades[i].codigo ||
                pedidos[j].unidadTransbordo.codigo == unidades[i].codigo) {
                cantidad++;
            }
        }

        if (cantidad > mayor) {
            mayor = cantidad;
            bus = unidades[i].codigo;
        }
    }

    cout << "Unidad con mas pedidos: " << bus << endl;
    cout << "Cantidad de pedidos: " << mayor << endl;
}

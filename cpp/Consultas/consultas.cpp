
#include <iostream>
#include "consultas.h"
using namespace std;

void realizarConsultas(vector<Punto> &puntos,
                      vector<Conexion> &conexiones,
                      vector<Pedido> &pedidos) {

    int opcion;
    string codigo;
    string estado;

    cout << "\n--- CONSULTAS BASICAS ---" << endl;
    cout << "1. Mostrar puntos" << endl;
    cout << "2. Conexiones de un punto" << endl;
    cout << "3. Pedidos por estado" << endl;
    cout << "4. Pedidos de una unidad" << endl;
    cout << "Seleccione: ";
    cin >> opcion;

    if (opcion == 1) {
        for (int i = 0; i < puntos.size(); i++) {
            cout << puntos[i].codigo << " - "
                 << puntos[i].nombre << endl;
        }
    }
    else if (opcion == 2) {
        cout << "Codigo del punto: ";
        cin >> codigo;

        for (int i = 0; i < conexiones.size(); i++) {
            if (conexiones[i].origen == codigo ||
                conexiones[i].destino == codigo) {

                cout << conexiones[i].origen << " - "
                     << conexiones[i].destino << " | "
                     << estadoConexionToString(conexiones[i].estado)
                     << endl;
            }
        }
    }
    else if (opcion == 3) {
        cout << "Estado (pendiente/asignado/entregado): ";
        cin >> estado;

        for (int i = 0; i < pedidos.size(); i++) {
            if (estadoPedidoToString(pedidos[i].estado) == estado) {
                cout << pedidos[i].codigo << " | "
                     << pedidos[i].origen.codigo << " -> "
                     << pedidos[i].destino.codigo << " | "
                     << pedidos[i].carga << " pasajeros" << endl;
            }
        }
    }
    else if (opcion == 4) {
        cout << "Codigo de unidad: ";
        cin >> codigo;

        for (int i = 0; i < pedidos.size(); i++) {
            if (pedidos[i].unidadAsignada.codigo == codigo ||
                pedidos[i].unidadTransbordo.codigo == codigo) {
                cout << pedidos[i].codigo << " | "
                     << estadoPedidoToString(pedidos[i].estado)
                     << endl;
            }
        }
    }
    else {
        cout << "Opcion no valida" << endl;
    }
}

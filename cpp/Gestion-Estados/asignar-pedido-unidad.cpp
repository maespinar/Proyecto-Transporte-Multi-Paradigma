#include "asignar-pedido-unidad.h"

void asignarPedidoUnidad(Pedido& pedido, Unidad& unidadAsignada, const string& horaSolicitud) {
    if (pedido.estado == PENDIENTE) {
        if (unidadAsignada.estado == DISPONIBLE) {
            if (unidadAsignada.capacidad >= pedido.carga) {
                pedido.unidadAsignada = unidadAsignada;
                pedido.horaSolicitud = horaSolicitud;
                pedido.estado = ASIGNADO;
                unidadAsignada.estado = OCUPADA;
            } else {
                cout << "Error: La unidad asignada no tiene suficiente capacidad para el pedido." << endl;
            }
        } else {
            cout << "Error: La unidad asignada no está disponible." << endl;
        }
    } else {
        cout << "Error: El pedido ya ha sido asignado o entregado." << endl;
    }
}
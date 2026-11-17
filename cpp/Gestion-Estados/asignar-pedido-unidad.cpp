#include "asignar-pedido-unidad.h"
#include "../ControlDeAutonomia/autonomia.h"
#include <iostream>

bool asignarPedidoUnidad(Pedido& pedido, Unidad& unidadAsignada, const string& horaSolicitud,
                        const vector<Conexion>& conexiones) {
    string mensaje;
    if (pedido.estado != PENDIENTE) mensaje = "El pedido no esta pendiente.";
    else if (unidadAsignada.estado != DISPONIBLE) mensaje = "El bus no esta disponible.";
    else if (unidadAsignada.capacidad < pedido.carga) mensaje = "El bus no tiene capacidad suficiente.";
    if (!mensaje.empty()) {
        cout << "Asignacion bloqueada: " << mensaje << '\n';
        return false;
    }
    EvaluacionAutonomia evaluacion;
    if (!verificarAutonomia(pedido, unidadAsignada, conexiones, evaluacion)) return false;
    if (!evaluacion.apta) {
        cout << "Asignacion bloqueada: autonomia insuficiente. El bus requiere mantenimiento preventivo o recarga.\n";
        return false;
    }
    unidadAsignada.estado = OCUPADA;
    pedido.unidadAsignada = unidadAsignada;
    pedido.unidadTransbordo = Unidad{};
    pedido.horaSolicitud = horaSolicitud;
    pedido.estado = ASIGNADO;
    return true;
}

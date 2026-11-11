#include "structures.h"

string tipoPuntoToString(TipoPunto t) {
    switch (t) {
        case TERMINAL:    return "Terminal";
        case ESTACION:    return "Estacion";
        case INTERCAMBIO: return "Intercambio";
        default:          return "Paradero";
    }
}

string estadoConexionToString(EstadoConexion e) {
    return (e == HABILITADA) ? "habilitada" : "cerrada";
}

string estadoUnidadToString(EstadoUnidad e) {
    return (e == DISPONIBLE) ? "disponible" : "ocupada";
}

string prioridadToString(PrioridadPedido p) {
    switch (p) {
        case ALTA:  return "alta";
        case MEDIA: return "media";
        default:    return "baja";
    }
}

string estadoPedidoToString(EstadoPedido e) {
    switch (e) {
        case ASIGNADO:  return "asignado";
        case ENTREGADO: return "entregado";
        default:        return "pendiente";
    }
}

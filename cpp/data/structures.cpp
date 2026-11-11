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

TipoPunto stringToTipoPunto(const string& s) {
    if (s == "Terminal")    return TERMINAL;
    if (s == "Estacion")    return ESTACION;
    if (s == "Intercambio") return INTERCAMBIO;
    return PARADERO;
}

EstadoConexion stringToEstadoConexion(const string& s) {
    return (s == "cerrada") ? CERRADA : HABILITADA;
}

EstadoUnidad stringToEstadoUnidad(const string& s) {
    return (s == "ocupada") ? OCUPADA : DISPONIBLE;
}

PrioridadPedido stringToPrioridad(const string& s) {
    if (s == "alta")  return ALTA;
    if (s == "media") return MEDIA;
    return BAJA;
}

EstadoPedido stringToEstadoPedido(const string& s) {
    if (s == "asignado")  return ASIGNADO;
    if (s == "entregado") return ENTREGADO;
    return PENDIENTE;
}
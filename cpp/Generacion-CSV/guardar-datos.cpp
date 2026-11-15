#include "guardar-datos.h"
#include "../data/structures.h"
#include <fstream>
#include <iostream>
#include <limits>

bool finalizarGuardado(ofstream& archivo, const string& ruta) {
    archivo.close();
    if (archivo.fail()) {
        cerr << "Error al guardar los datos en " << ruta << endl;
        return false;
    }
    return true;
}

bool guardarPuntos(const vector<Punto>& puntos, const string& ruta) {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir " << ruta << " para guardar." << endl;
        return false;
    }

    archivo << "codigo,nombre,tipo,zona,permite_transbordo\n";
    for (const Punto& punto : puntos) {
        archivo << punto.codigo << ',' << punto.nombre << ','
                << tipoPuntoToString(punto.tipo) << ',' << punto.zona << ','
                << (punto.permiteTransbordo ? "true" : "false") << '\n';
    }
    return finalizarGuardado(archivo, ruta);
}

bool guardarConexiones(const vector<Conexion>& conexiones, const string& ruta) {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir " << ruta << " para guardar." << endl;
        return false;
    }

    archivo.precision(numeric_limits<double>::max_digits10);
    archivo << "origen,destino,distancia_km,tiempo_min,estado\n";
    for (const Conexion& conexion : conexiones) {
        archivo << conexion.origen << ',' << conexion.destino << ','
                << conexion.distanciaKm << ',' << conexion.tiempoMin << ','
                << estadoConexionToString(conexion.estado) << '\n';
    }
    return finalizarGuardado(archivo, ruta);
}

bool guardarUnidades(const vector<Unidad>& unidades, const string& ruta) {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir " << ruta << " para guardar." << endl;
        return false;
    }

    archivo.precision(numeric_limits<double>::max_digits10);
    archivo << "codigo,tipo,capacidad,costo_km,estado,tipo_combustible,"
               "factor_emision,km_acumulados,km_limite_mantenimiento\n";
    for (const Unidad& unidad : unidades) {
        string combustible = "GNV";
        if (unidad.tipoCombustible == DIESEL) {
            combustible = "Diesel";
        } else if (unidad.tipoCombustible == ELECTRICO) {
            combustible = "Electrico";
        }

        archivo << unidad.codigo << ',' << unidad.tipo << ',' << unidad.capacidad << ','
                << unidad.costoKm << ',' << estadoUnidadToString(unidad.estado) << ','
                << combustible << ',' << unidad.factorEmision << ','
                << unidad.kmAcumulados << ',' << unidad.kmLimiteMant << '\n';
    }
    return finalizarGuardado(archivo, ruta);
}

bool guardarPedidos(const vector<Pedido>& pedidos, const string& ruta) {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir " << ruta << " para guardar." << endl;
        return false;
    }

    archivo << "codigo,origen,destino,carga,prioridad,estado,unidad_asignada,"
               "hora_solicitud,unidad_transbordo\n";
    for (const Pedido& pedido : pedidos) {
        archivo << pedido.codigo << ',' << pedido.origen.codigo << ','
                << pedido.destino.codigo << ',' << pedido.carga << ','
                << prioridadToString(pedido.prioridad) << ','
                << estadoPedidoToString(pedido.estado) << ','
                << pedido.unidadAsignada.codigo << ',' << pedido.horaSolicitud << ','
                << pedido.unidadTransbordo.codigo << '\n';
    }
    return finalizarGuardado(archivo, ruta);
}

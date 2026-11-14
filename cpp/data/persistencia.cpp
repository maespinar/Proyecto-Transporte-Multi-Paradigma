#include "persistencia.h"
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

string csvPuntos(const vector<Punto>& puntos) {
    ostringstream salida;
    salida << "codigo,nombre,tipo,zona,permite_transbordo\n";
    for (int i = 0; i < (int)puntos.size(); i++) {
        salida << puntos[i].codigo << ',' << puntos[i].nombre << ','
               << tipoPuntoToString(puntos[i].tipo) << ',' << puntos[i].zona << ',';
        if (puntos[i].permiteTransbordo) salida << "true\n";
        else salida << "false\n";
    }
    return salida.str();
}

string csvConexiones(const vector<Conexion>& conexiones) {
    ostringstream salida;
    salida << setprecision(17);
    salida << "origen,destino,distancia_km,tiempo_min,estado\n";
    for (int i = 0; i < (int)conexiones.size(); i++) {
        salida << conexiones[i].origen << ',' << conexiones[i].destino << ','
               << conexiones[i].distanciaKm << ',' << conexiones[i].tiempoMin << ','
               << estadoConexionToString(conexiones[i].estado) << '\n';
    }
    return salida.str();
}

string csvUnidades(const vector<Unidad>& unidades) {
    ostringstream salida;
    salida << setprecision(17);
    salida << "codigo,tipo,capacidad,costo_km,estado,tipo_combustible,factor_emision,km_acumulados,km_limite_mantenimiento\n";
    for (int i = 0; i < (int)unidades.size(); i++) {
        string combustible = "GNV";
        if (unidades[i].tipoCombustible == DIESEL) combustible = "Diesel";
        if (unidades[i].tipoCombustible == ELECTRICO) combustible = "Electrico";
        salida << unidades[i].codigo << ',' << unidades[i].tipo << ','
               << unidades[i].capacidad << ',' << unidades[i].costoKm << ','
               << estadoUnidadToString(unidades[i].estado) << ',' << combustible << ','
               << unidades[i].factorEmision << ',' << unidades[i].kmAcumulados << ','
               << unidades[i].kmLimiteMant << '\n';
    }
    return salida.str();
}

string csvPedidos(const vector<Pedido>& pedidos) {
    ostringstream salida;
    salida << "codigo,origen,destino,carga,prioridad,estado,unidad_asignada,hora_solicitud,unidad_transbordo\n";
    for (int i = 0; i < (int)pedidos.size(); i++) {
        salida << pedidos[i].codigo << ',' << pedidos[i].origen.codigo << ','
               << pedidos[i].destino.codigo << ',' << pedidos[i].carga << ','
               << prioridadToString(pedidos[i].prioridad) << ','
               << estadoPedidoToString(pedidos[i].estado) << ','
               << pedidos[i].unidadAsignada.codigo << ',' << pedidos[i].horaSolicitud
               << ',' << pedidos[i].unidadTransbordo.codigo << '\n';
    }
    return salida.str();
}

bool existeArchivo(string ruta) {
    ifstream archivo(ruta);
    return archivo.is_open();
}

bool escribirCSV(string ruta, string contenido) {
    ofstream archivo(ruta, ios::binary);
    if (!archivo.is_open()) return false;
    archivo << contenido;
    archivo.close();
    return !archivo.fail();
}

// Lee cada caracter para conservar tambien los saltos de linea originales.
bool leerOriginal(string ruta, string& contenido) {
    ifstream archivo(ruta, ios::binary);
    if (!archivo.is_open()) return false;
    contenido = "";
    char caracter;
    while (archivo.get(caracter)) contenido += caracter;
    return archivo.eof();
}

bool guardarCambiosCSV(const vector<CambioCSV>& cambios, string& error) {
    error = "";
    vector<string> originales;
    // Se comprueban todos los archivos antes de escribir alguno.
    for (int i = 0; i < (int)cambios.size(); i++) {
        string contenido;
        if (!leerOriginal(cambios[i].ruta, contenido)) {
            error = "No se pudo leer el CSV original: " + cambios[i].ruta;
            return false;
        }
        originales.push_back(contenido);
    }
    for (int i = 0; i < (int)cambios.size(); i++) {
        if (!escribirCSV(cambios[i].ruta, cambios[i].contenido)) {
            error = "No se pudo guardar: " + cambios[i].ruta;
            // Si una escritura falla, se intenta devolver lo que habia antes.
            for (int j = 0; j <= i; j++) {
                if (!escribirCSV(cambios[j].ruta, originales[j]))
                    error += " No se pudo restaurar: " + cambios[j].ruta;
            }
            return false;
        }
    }
    return true;
}

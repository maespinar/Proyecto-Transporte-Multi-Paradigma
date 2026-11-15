#include "registro.h"
#include "../data/persistencia.h"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

// 4.1.3 - Autor: Diego Alejandro Chuquiure Vargas.
bool codigoValido(string codigo) {
    if (codigo.empty()) return false;
    for (int i = 0; i < (int)codigo.size(); i++) {
        char c = codigo[i];
        if (!(c >= 'A' && c <= 'Z') && !(c >= '0' && c <= '9') && c != '_')
            return false;
    }
    return true;
}

bool textoCSVValido(string texto) {
    if (texto.find_first_of(",\r\n") != string::npos) return false;
    if (texto.find_first_not_of(" \t") == string::npos) return false;
    return true;
}

bool existePunto(string codigo, const vector<Punto>& puntos) {
    for (int i = 0; i < (int)puntos.size(); i++) {
        if (puntos[i].codigo == codigo) return true;
    }
    return false;
}

bool existeUnidad(string codigo, const vector<Unidad>& unidades) {
    for (int i = 0; i < (int)unidades.size(); i++) {
        if (unidades[i].codigo == codigo) return true;
    }
    return false;
}

bool existePedido(string codigo, const vector<Pedido>& pedidos) {
    for (int i = 0; i < (int)pedidos.size(); i++) {
        if (pedidos[i].codigo == codigo) return true;
    }
    return false;
}

bool horaValida(string hora) {
    if (hora.size() != 5 || hora[2] != ':') return false;
    for (int i = 0; i < 5; i++) {
        if (i != 2 && (hora[i] < '0' || hora[i] > '9')) return false;
    }
    int horas = (hora[0] - '0') * 10 + hora[1] - '0';
    int minutos = (hora[3] - '0') * 10 + hora[4] - '0';
    return horas < 24 && minutos < 60;
}

bool registrarPunto(const Punto& nuevo, vector<Punto>& puntos, string& error) {
    error = "";
    if (!codigoValido(nuevo.codigo) || existePunto(nuevo.codigo, puntos))
        error = "Codigo de punto invalido o repetido.";
    else if (!textoCSVValido(nuevo.nombre) || !textoCSVValido(nuevo.zona))
        error = "Nombre y zona son obligatorios, sin comas ni saltos de linea.";
    else if (nuevo.tipo < PARADERO || nuevo.tipo > INTERCAMBIO)
        error = "Tipo de punto invalido.";
    if (!error.empty()) return false;
    puntos.push_back(nuevo);
    return true;
}

bool registrarConexion(const Conexion& nueva, const vector<Punto>& puntos,
                      vector<Conexion>& conexiones, string& error) {
    error = "";
    if (!existePunto(nueva.origen, puntos) || !existePunto(nueva.destino, puntos))
        error = "Ambos puntos de la conexion deben existir.";
    else if (nueva.origen == nueva.destino)
        error = "Una conexion debe unir puntos distintos.";
    else if (!isfinite(nueva.distanciaKm) || nueva.distanciaKm <= 0 || nueva.tiempoMin <= 0)
        error = "La distancia y el tiempo deben ser mayores que cero.";
    else if (nueva.estado != HABILITADA && nueva.estado != CERRADA)
        error = "Estado de conexion invalido.";
    if (!error.empty()) return false;
    for (int i = 0; i < (int)conexiones.size(); i++) {
        if ((conexiones[i].origen == nueva.origen && conexiones[i].destino == nueva.destino) ||
            (conexiones[i].origen == nueva.destino && conexiones[i].destino == nueva.origen)) {
            error = "La conexion ya existe en uno de los dos sentidos.";
            return false;
        }
    }
    conexiones.push_back(nueva);
    return true;
}

bool registrarUnidad(const Unidad& nueva, vector<Unidad>& unidades, string& error) {
    error = "";
    if (!codigoValido(nueva.codigo) || existeUnidad(nueva.codigo, unidades))
        error = "Codigo de unidad invalido o repetido.";
    else if (!textoCSVValido(nueva.tipo) || nueva.capacidad <= 0 || !isfinite(nueva.costoKm) || nueva.costoKm <= 0)
        error = "Indique un tipo valido, capacidad y costo positivos.";
    else if (nueva.estado != DISPONIBLE || nueva.tipoCombustible < GNV || nueva.tipoCombustible > ELECTRICO)
        error = "Una unidad nueva debe estar disponible y tener un combustible valido.";
    else if (!isfinite(nueva.factorEmision) || nueva.factorEmision < 0 ||
             !isfinite(nueva.kmAcumulados) || nueva.kmAcumulados < 0 ||
             !isfinite(nueva.kmLimiteMant) || nueva.kmLimiteMant <= 0)
        error = "Emision y kilometraje no negativos; limite de mantenimiento positivo.";
    if (!error.empty()) return false;
    unidades.push_back(nueva);
    return true;
}

bool registrarPedido(const Pedido& nuevo, const vector<Punto>& puntos, vector<Pedido>& pedidos, string& error) {
    error = "";
    if (!codigoValido(nuevo.codigo) || existePedido(nuevo.codigo, pedidos))
        error = "Codigo de solicitud invalido o repetido.";
    else if (!existePunto(nuevo.origen.codigo, puntos) || !existePunto(nuevo.destino.codigo, puntos))
        error = "El origen y destino deben existir.";
    else if (nuevo.origen.codigo == nuevo.destino.codigo || nuevo.carga <= 0)
        error = "Los puntos deben ser distintos y la carga positiva.";
    else if (nuevo.prioridad < ALTA || nuevo.prioridad > BAJA || !horaValida(nuevo.horaSolicitud))
        error = "Prioridad invalida u hora fuera del formato HH:MM.";
    else if (nuevo.estado != PENDIENTE || !nuevo.unidadAsignada.codigo.empty() || !nuevo.unidadTransbordo.codigo.empty())
        error = "Una solicitud nueva debe estar pendiente y sin buses asignados.";
    if (!error.empty()) return false;
    pedidos.push_back(nuevo);
    return true;
}

bool leerDecimalRegistro(const string& mensaje, double& numero) {
    cout << mensaje;
    string linea;
    if (!getline(cin, linea)) return false;
    istringstream entrada(linea);
    if (!(entrada >> numero)) return false;
    entrada >> ws;
    return entrada.eof();
}

bool leerTextoRegistro(const string& mensaje, string& texto) {
    cout << mensaje;
    if (!getline(cin, texto)) return false;
    size_t inicio = texto.find_first_not_of(" \t\r");
    if (inicio == string::npos) texto.clear();
    else texto = texto.substr(inicio, texto.find_last_not_of(" \t\r") - inicio + 1);
    return true;
}

bool leerEnteroRegistro(const string& mensaje, int& numero) {
    string linea;
    if (!leerTextoRegistro(mensaje, linea)) return false;
    istringstream entrada(linea);
    if (!(entrada >> numero)) return false;
    entrada >> ws;
    return entrada.eof();
}

void procesarRegistro(vector<Punto>& puntos, vector<Conexion>& conexiones,
                            vector<Unidad>& unidades, vector<Pedido>& pedidos,
                            string carpetaDatos) {
    string restoLinea;
    getline(cin, restoLinea); // Consume el salto de linea dejado por el menu.
    int opcion = 0;
    cout << "\n--- REGISTRO DE INFORMACION ---\n1. Punto\n2. Conexion\n3. Unidad\n4. Solicitud\n";
    if (!leerEnteroRegistro("Seleccione: ", opcion)) {
        cout << "Entrada numerica invalida.\n";
        return;
    }
    string error;
    bool valido = false;
    vector<Punto> nuevosPuntos = puntos;
    vector<Conexion> nuevasConexiones = conexiones;
    vector<Unidad> nuevasUnidades = unidades;
    vector<Pedido> nuevosPedidos = pedidos;
    string nombre, contenido;
    if (opcion == 1) {
        Punto p{};
        int tipo = -1, transbordo = -1;
        valido = leerTextoRegistro("Codigo: ", p.codigo) && leerTextoRegistro("Nombre: ", p.nombre) &&
            leerEnteroRegistro("Tipo (0 paradero, 1 terminal, 2 estacion, 3 intercambio): ", tipo) &&
            leerTextoRegistro("Zona: ", p.zona) && leerEnteroRegistro("Permite transbordo (0 no, 1 si): ", transbordo);
        if (valido && tipo >= 0 && tipo <= 3 && (transbordo == 0 || transbordo == 1)) {
            p.tipo = (TipoPunto)tipo;
            p.permiteTransbordo = transbordo == 1;
            valido = registrarPunto(p, nuevosPuntos, error);
        } else valido = false;
        nombre = "puntos.csv";
        contenido = csvPuntos(nuevosPuntos);
    } else if (opcion == 2) {
        Conexion c{};
        int estado = -1;
        valido = leerTextoRegistro("Origen: ", c.origen) && leerTextoRegistro("Destino: ", c.destino) &&
            leerDecimalRegistro("Distancia (km): ", c.distanciaKm) && leerEnteroRegistro("Tiempo (minutos enteros): ", c.tiempoMin) &&
            leerEnteroRegistro("Estado (0 habilitada, 1 cerrada): ", estado);
        if (valido && estado >= 0 && estado <= 1) {
            c.estado = (EstadoConexion)estado;
            valido = registrarConexion(c, puntos, nuevasConexiones, error);
        } else valido = false;
        nombre = "conexiones.csv";
        contenido = csvConexiones(nuevasConexiones);
    } else if (opcion == 3) {
        Unidad u{};
        int combustible = -1;
        valido = leerTextoRegistro("Codigo: ", u.codigo) && leerTextoRegistro("Tipo de bus: ", u.tipo) &&
            leerEnteroRegistro("Capacidad: ", u.capacidad) && leerDecimalRegistro("Costo por km (S/): ", u.costoKm) &&
            leerEnteroRegistro("Combustible (0 GNV, 1 Diesel, 2 Electrico): ", combustible) &&
            leerDecimalRegistro("Factor de emision (kg CO2/km): ", u.factorEmision) &&
            leerDecimalRegistro("Kilometraje acumulado: ", u.kmAcumulados) &&
            leerDecimalRegistro("Limite de kilometraje para mantenimiento: ", u.kmLimiteMant);
        if (valido && combustible >= 0 && combustible <= 2) {
            u.estado = DISPONIBLE;
            u.tipoCombustible = (TipoCombustible)combustible;
            valido = registrarUnidad(u, nuevasUnidades, error);
        } else valido = false;
        nombre = "unidades.csv";
        contenido = csvUnidades(nuevasUnidades);
    } else if (opcion == 4) {
        Pedido p{};
        int prioridad = -1;
        valido = leerTextoRegistro("Codigo: ", p.codigo) && leerTextoRegistro("Origen: ", p.origen.codigo) &&
            leerTextoRegistro("Destino: ", p.destino.codigo) && leerEnteroRegistro("Pasajeros: ", p.carga) &&
            leerEnteroRegistro("Prioridad (0 alta, 1 media, 2 baja): ", prioridad) &&
            leerTextoRegistro("Hora de solicitud (HH:MM): ", p.horaSolicitud);
        if (valido && prioridad >= 0 && prioridad <= 2) {
            p.prioridad = (PrioridadPedido)prioridad;
            p.estado = PENDIENTE;
            for (int i = 0; i < (int)puntos.size(); i++) {
                if (puntos[i].codigo == p.origen.codigo) p.origen = puntos[i];
                if (puntos[i].codigo == p.destino.codigo) p.destino = puntos[i];
            }
            valido = registrarPedido(p, puntos, nuevosPedidos, error);
        } else valido = false;
        nombre = "pedidos.csv";
        contenido = csvPedidos(nuevosPedidos);
    }
    if (!valido) {
        if (error.empty()) error = "opcion o entrada invalida.";
        cout << "No se registro: " << error << '\n';
        return;
    }
    if (!guardarCambiosCSV({{carpetaDatos + "/" + nombre, contenido}}, error)) {
        cout << "No se guardo el registro: " << error << '\n';
        return;
    }
    puntos = nuevosPuntos;
    conexiones = nuevasConexiones;
    unidades = nuevasUnidades;
    pedidos = nuevosPedidos;
    cout << "Registro guardado en " << nombre << ".\n";
}

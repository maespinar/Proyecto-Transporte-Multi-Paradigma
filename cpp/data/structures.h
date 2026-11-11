#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>

using namespace std;

//Puntos
enum TipoPunto {
    PARADERO,
    TERMINAL,
    ESTACION,
    INTERCAMBIO
};

//Conexiones
enum EstadoConexion {
    HABILITADA,
    CERRADA
};

//Unidades
enum EstadoUnidad {
    DISPONIBLE,
    OCUPADA
};

enum TipoCombustible {
    GNV,
    DIESEL,
    ELECTRICO
};

//Pedidos
enum PrioridadPedido {
    ALTA,
    MEDIA,
    BAJA
};

enum EstadoPedido {
    PENDIENTE,
    ASIGNADO,
    ENTREGADO
};

struct Punto {
    string codigo;
    string nombre;
    TipoPunto tipo;
    string zona;
    bool permiteTransbordo;
};

struct Conexion {
    string origen;
    string destino;
    double distanciaKm;
    int tiempoMin;
    EstadoConexion estado;
};

struct Unidad {
    string codigo;
    string tipo;            
    int capacidad;
    double costoKm;
    EstadoUnidad estado;
    TipoCombustible tipoCombustible; 
    double factorEmision;   
    double kmAcumulados;    
    double kmLimiteMant;    
};

struct Pedido {
    string codigo;
    string origen;
    string destino;
    int carga;
    PrioridadPedido prioridad;
    EstadoPedido estado;
    string unidadAsignada;  
    string horaSolicitud;   
    string unidadTransbordo;
};

string tipoPuntoToString(TipoPunto t);
string estadoConexionToString(EstadoConexion e);
string estadoUnidadToString(EstadoUnidad e);
string prioridadToString(PrioridadPedido p);
string estadoPedidoToString(EstadoPedido e);

TipoPunto stringToTipoPunto(const string& s);
EstadoConexion stringToEstadoConexion(const string& s);
EstadoUnidad stringToEstadoUnidad(const string& s);
PrioridadPedido stringToPrioridad(const string& s);
EstadoPedido stringToEstadoPedido(const string& s);

#endif

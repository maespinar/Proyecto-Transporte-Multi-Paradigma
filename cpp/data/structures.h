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
    Punto origen;
    Punto destino;
    int carga;
    PrioridadPedido prioridad;
    EstadoPedido estado;
    Unidad unidadAsignada;  
    string horaSolicitud;   
    Unidad unidadTransbordo;
};

string tipoPuntoToString(TipoPunto t);
string estadoConexionToString(EstadoConexion e);
string estadoUnidadToString(EstadoUnidad e);
string prioridadToString(PrioridadPedido p);
string estadoPedidoToString(EstadoPedido e);

#endif

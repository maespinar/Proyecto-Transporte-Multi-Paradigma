#include "actualizar-conexion.h"

void actualizarConexion(Conexion& conexion, const EstadoConexion nuevoEstado) {
    conexion.estado = nuevoEstado;
    cout << "Estado de la conexión actualizada a: " << estadoConexionToString(nuevoEstado) << endl;
}
#include "autonomia.h"
#include "../Gestion-Estados/asignar-pedido-unidad.h"
#include "../data/persistencia.h"
#include <iostream>

static bool kilometrajeValido(double valor) {
    return valor >= 0 && valor - valor == 0;
}

static bool autonomiaSuficiente(double restante, double requerida) {
    double toleranciaRedondeo = 0.000000001;
    return restante > 0 && requerida <= restante + toleranciaRedondeo;
}

static bool buscarDistancia(const string& actual, const string& destino,
                            const vector<Conexion>& conexiones, vector<string>& visitados,
                            double acumulada, double& menor) {
    if (actual == destino) {
        if (menor < 0 || acumulada < menor) menor = acumulada;
        return true;
    }
    if (menor >= 0 && acumulada >= menor) return true;
    for (const Conexion& conexion : conexiones) {
        if (conexion.estado != HABILITADA) continue;
        string siguiente;
        if (conexion.origen == actual) siguiente = conexion.destino;
        else if (conexion.destino == actual) siguiente = conexion.origen;
        if (siguiente.empty()) continue;
        bool visitado = false;
        for (const string& codigo : visitados) {
            if (codigo == siguiente) visitado = true;
        }
        if (visitado) continue;
        double distancia = acumulada + conexion.distanciaKm;
        if (!kilometrajeValido(distancia)) {
            cout << "La distancia de la ruta excede el rango permitido.\n";
            return false;
        }
        visitados.push_back(siguiente);
        bool resultado = buscarDistancia(siguiente, destino, conexiones, visitados, distancia, menor);
        visitados.pop_back();
        if (!resultado) return false;
    }
    return true;
}

bool verificarAutonomia(const Pedido& pedido, const Unidad& unidad,
                        const vector<Conexion>& conexiones,
                        EvaluacionAutonomia& evaluacion) {
    if (pedido.origen.codigo.empty() || pedido.destino.codigo.empty() || pedido.carga <= 0) {
        cout << "El pedido debe tener origen, destino y una cantidad positiva de pasajeros.\n";
        return false;
    }
    if (!kilometrajeValido(unidad.kmAcumulados) || !kilometrajeValido(unidad.kmLimiteMant) ||
        unidad.kmLimiteMant == 0) {
        cout << "El kilometraje del bus no es valido.\n";
        return false;
    }
    for (const Conexion& conexion : conexiones) {
        if ((conexion.estado != HABILITADA && conexion.estado != CERRADA) ||
            !kilometrajeValido(conexion.distanciaKm) || conexion.distanciaKm == 0 || conexion.tiempoMin <= 0) {
            cout << "Hay una conexion con datos invalidos.\n";
            return false;
        }
    }
    vector<string> visitados = {pedido.origen.codigo};
    double distancia = -1;
    if (!buscarDistancia(pedido.origen.codigo, pedido.destino.codigo, conexiones, visitados, 0, distancia))
        return false;
    if (distancia < 0) {
        cout << "No existe una ruta habilitada para el pedido.\n";
        return false;
    }
    double restante = unidad.kmLimiteMant - unidad.kmAcumulados;
    evaluacion = {restante, distancia, autonomiaSuficiente(restante, distancia)};
    return true;
}

void mostrarAutonomia(const Pedido& pedido, const Unidad& unidad,
                      const EvaluacionAutonomia& evaluacion, const vector<Unidad>& unidades) {
    cout << "Bus " << unidad.codigo << " | Autonomia restante: " << evaluacion.restante
         << " km | Distancia requerida: " << evaluacion.requerida << " km\n";
    if (evaluacion.apta) {
        cout << "Estado: Apto por autonomia.\n";
        if (evaluacion.restante <= evaluacion.requerida)
            cout << "Alerta: mantenimiento o recarga al terminar el recorrido.\n";
    } else {
        cout << "Estado: No apto (requiere mantenimiento preventivo o recarga, faltan "
             << evaluacion.requerida - evaluacion.restante << " km).\n";
    }
    if (unidad.estado != DISPONIBLE) cout << "El bus esta ocupado.\n";
    if (unidad.capacidad < pedido.carga) cout << "El bus no tiene capacidad suficiente.\n";
    if (evaluacion.apta && unidad.estado == DISPONIBLE && unidad.capacidad >= pedido.carga) return;
    bool hayAlternativa = false;
    for (const Unidad& alternativa : unidades) {
        if (alternativa.codigo == unidad.codigo || alternativa.estado != DISPONIBLE ||
            alternativa.capacidad < pedido.carga) continue;
        if (!kilometrajeValido(alternativa.kmAcumulados) || !kilometrajeValido(alternativa.kmLimiteMant) ||
            alternativa.kmLimiteMant == 0) {
            cout << "Aviso: kilometraje invalido en " << alternativa.codigo << ".\n";
            continue;
        }
        double restante = alternativa.kmLimiteMant - alternativa.kmAcumulados;
        if (autonomiaSuficiente(restante, evaluacion.requerida)) {
            cout << "Recomendacion: Asignar unidad " << alternativa.codigo
                 << " (autonomia restante: " << restante << " km).\n";
            hayAlternativa = true;
        }
    }
    if (!hayAlternativa) cout << "No hay otras unidades disponibles con capacidad y autonomia suficientes.\n";
}

void procesarAutonomia(vector<Pedido>& pedidos, vector<Unidad>& unidades,
                       const vector<Conexion>& conexiones, const string& carpetaDatos, bool asignar) {
    string codigoPedido, codigoUnidad;
    cout << "\n--- CONTROL DE AUTONOMIA Y MANTENIMIENTO ---\nCodigo de pedido: ";
    if (!(cin >> codigoPedido)) return;
    cout << "Codigo de bus: ";
    if (!(cin >> codigoUnidad)) return;
    int posicionPedido = -1, posicionUnidad = -1;
    for (int i = 0; i < (int)pedidos.size(); i++) {
        if (pedidos[i].codigo == codigoPedido) posicionPedido = i;
    }
    for (int i = 0; i < (int)unidades.size(); i++) {
        if (unidades[i].codigo == codigoUnidad) posicionUnidad = i;
    }
    if (posicionPedido == -1 || posicionUnidad == -1) {
        cout << "El pedido o el bus indicado no existe.\n";
        return;
    }
    EvaluacionAutonomia evaluacion;
    if (!verificarAutonomia(pedidos[posicionPedido], unidades[posicionUnidad], conexiones, evaluacion)) return;
    mostrarAutonomia(pedidos[posicionPedido], unidades[posicionUnidad], evaluacion, unidades);
    if (!asignar) return;
    vector<Pedido> nuevosPedidos = pedidos;
    vector<Unidad> nuevasUnidades = unidades;
    Pedido& pedido = nuevosPedidos[posicionPedido];
    if (!asignarPedidoUnidad(pedido, nuevasUnidades[posicionUnidad], pedido.horaSolicitud, conexiones)) return;
    vector<CambioCSV> cambios = {
        {carpetaDatos + "/pedidos.csv", csvPedidos(nuevosPedidos)},
        {carpetaDatos + "/unidades.csv", csvUnidades(nuevasUnidades)}
    };
    string mensajeGuardado;
    if (!guardarCambiosCSV(cambios, mensajeGuardado)) {
        cout << "No se guardo la asignacion: " << mensajeGuardado << '\n';
        return;
    }
    pedidos = nuevosPedidos;
    unidades = nuevasUnidades;
    cout << "Pedido " << codigoPedido << " asignado al bus " << codigoUnidad << ".\n";
}

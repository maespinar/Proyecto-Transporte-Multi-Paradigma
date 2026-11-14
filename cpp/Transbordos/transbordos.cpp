#include "transbordos.h"
#include "../Registro/registro.h"
#include "../data/persistencia.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <climits>

using namespace std;

// FA - Autor: Diego Alejandro Chuquiure Vargas.
void borrarConsulta() {
    remove("consulta_transbordo.csv");
    remove("puntos_transbordo.csv");
    remove("conexiones_transbordo.csv");
    remove("plan_transbordo.txt");
}

bool ejecutarProlog(string script) {
    // Comandos fijos: los datos del usuario viajan por CSV, no por la consola.
    if (script == "prolog/consulta_transbordo.pl") {
        return system("swipl -q -s prolog/consulta_transbordo.pl -g consulta_transbordo -t halt") == 0;
    }
    if (script == "../prolog/consulta_transbordo.pl") {
        return system("swipl -q -s ../prolog/consulta_transbordo.pl -g consulta_transbordo -t halt") == 0;
    }
    return false;
}

bool consultarTransbordo(const Pedido& pedido, const vector<Punto>& puntos,
                        const vector<Conexion>& conexiones, string script,
                        PlanTransbordo& plan, string& error) {
    error = "";
    if (!codigoValido(pedido.origen.codigo) || !codigoValido(pedido.destino.codigo)) {
        error = "Codigos de origen o destino invalidos.";
        return false;
    }
    // Se evita sobrescribir archivos de una consulta anterior o de otra instancia.
    if (existeArchivo("consulta_transbordo.csv") || existeArchivo("puntos_transbordo.csv") ||
        existeArchivo("conexiones_transbordo.csv") || existeArchivo("plan_transbordo.txt")) {
        error = "Ya existen archivos de consulta. Cierre la otra instancia o retire los archivos anteriores.";
        return false;
    }
    string consulta = "origen,destino\n" + pedido.origen.codigo + "," + pedido.destino.codigo + "\n";
    if (!escribirCSV("consulta_transbordo.csv", consulta) ||
        !escribirCSV("puntos_transbordo.csv", csvPuntos(puntos)) ||
        !escribirCSV("conexiones_transbordo.csv", csvConexiones(conexiones))) {
        borrarConsulta();
        error = "No se pudieron preparar los CSV de consulta.";
        return false;
    }
    if (!ejecutarProlog(script)) {
        borrarConsulta();
        error = "No se pudo ejecutar Prolog. Agregue swipl al PATH o use ejecutar.ps1.";
        return false;
    }
    ifstream archivo("plan_transbordo.txt");
    string estado;
    archivo >> estado;
    if (estado == "sin_contingencia") error = "La ruta habitual no esta bloqueada; no requiere transbordo.";
    else if (estado == "sin_ruta") error = "No existe una alternativa habilitada con punto de transbordo.";
    else if (estado != "ok") error = "Prolog no devolvio una respuesta valida.";

    if (error.empty()) {
        archivo >> plan.punto >> plan.tiempoPrimero >> plan.tiempoSegundo;
        string linea, codigo;
        getline(archivo, linea); // Termina la linea de los tiempos.
        getline(archivo, linea);
        istringstream primera(linea);
        plan.rutaPrimera.clear();
        while (primera >> codigo) plan.rutaPrimera.push_back(codigo);
        getline(archivo, linea);
        istringstream segunda(linea);
        plan.rutaSegunda.clear();
        while (segunda >> codigo) plan.rutaSegunda.push_back(codigo);
        archivo >> plan.bloqueoOrigen >> plan.bloqueoDestino;
        if (archivo.fail()) error = "El plan de Prolog esta incompleto.";
    }
    archivo.close();
    borrarConsulta();
    return error.empty();
}

bool validarTramo(const vector<string>& ruta, const vector<Conexion>& conexiones, int tiempoEsperado) {
    if (ruta.size() < 2) return false;
    long long tiempo = 0;
    for (int i = 1; i < (int)ruta.size(); i++) {
        bool encontrado = false;
        for (int j = 0; j < (int)conexiones.size(); j++) {
            if (conexiones[j].estado == HABILITADA && conexiones[j].tiempoMin > 0 &&
                ((conexiones[j].origen == ruta[i - 1] && conexiones[j].destino == ruta[i]) ||
                 (conexiones[j].destino == ruta[i - 1] && conexiones[j].origen == ruta[i]))) {
                tiempo += conexiones[j].tiempoMin;
                encontrado = true;
                break;
            }
        }
        if (!encontrado) return false;
    }
    return tiempo == tiempoEsperado;
}

bool asignarTransbordo(Pedido& pedido, vector<Unidad>& unidades,
                      const vector<Punto>& puntos, const vector<Conexion>& conexiones,
                      const PlanTransbordo& plan, int espera, string& error) {
    error = "";
    if (pedido.estado != PENDIENTE || pedido.carga <= 0) {
        error = "La solicitud debe estar pendiente y tener pasajeros.";
        return false;
    }
    long long tiempoTotal = (long long)plan.tiempoPrimero + plan.tiempoSegundo + espera;
    if (espera < 0 || plan.tiempoPrimero <= 0 || plan.tiempoSegundo <= 0 || tiempoTotal > INT_MAX) {
        error = "Los tiempos del plan son invalidos.";
        return false;
    }
    bool permiteTransbordo = false;
    for (int i = 0; i < (int)puntos.size(); i++) {
        if (puntos[i].codigo == plan.punto && puntos[i].permiteTransbordo) permiteTransbordo = true;
    }
    if (!permiteTransbordo || !validarTramo(plan.rutaPrimera, conexiones, plan.tiempoPrimero) ||
        !validarTramo(plan.rutaSegunda, conexiones, plan.tiempoSegundo)) {
        error = "El intercambio o los tramos del plan no son validos.";
        return false;
    }
    if (plan.rutaPrimera[0] != pedido.origen.codigo || plan.rutaPrimera.back() != plan.punto ||
        plan.rutaSegunda[0] != plan.punto || plan.rutaSegunda.back() != pedido.destino.codigo) {
        error = "Las etapas no unen el origen, el intercambio y el destino.";
        return false;
    }
    vector<string> recorrido = plan.rutaPrimera;
    // El primer punto de la segunda etapa es el intercambio y ya esta incluido.
    for (int i = 1; i < (int)plan.rutaSegunda.size(); i++) recorrido.push_back(plan.rutaSegunda[i]);
    for (int i = 0; i < (int)recorrido.size(); i++) {
        for (int j = i + 1; j < (int)recorrido.size(); j++) {
            if (recorrido[i] == recorrido[j]) {
                error = "La ruta repite puntos.";
                return false;
            }
        }
    }
    int primero = -1, segundo = -1;
    for (int i = 0; i < (int)unidades.size(); i++) {
        if (unidades[i].estado == DISPONIBLE && unidades[i].capacidad >= pedido.carga) {
            if (primero == -1) primero = i;
            else if (unidades[i].codigo != unidades[primero].codigo) {
                segundo = i;
                break;
            }
        }
    }
    if (segundo == -1) {
        error = "Se requieren dos buses distintos disponibles, cada uno con capacidad para todo el grupo.";
        return false;
    }
    unidades[primero].estado = OCUPADA;
    unidades[segundo].estado = OCUPADA;
    pedido.unidadAsignada = unidades[primero];
    pedido.unidadTransbordo = unidades[segundo];
    pedido.estado = ASIGNADO;
    return true;
}

void mostrarRuta(const vector<string>& ruta) {
    for (int i = 0; i < (int)ruta.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << ruta[i];
    }
}

void procesarTransbordo(vector<Pedido>& pedidos, vector<Unidad>& unidades,
                       const vector<Punto>& puntos, const vector<Conexion>& conexiones,
                       string carpetaDatos, string script) {
    string codigo, error;
    cout << "\n--- PLANIFICAR TRANSBORDO ---\nCodigo de solicitud pendiente: ";
    if (!(cin >> codigo)) return;
    int posicion = -1;
    for (int i = 0; i < (int)pedidos.size(); i++) {
        if (pedidos[i].codigo == codigo) posicion = i;
    }
    if (posicion == -1 || pedidos[posicion].estado != PENDIENTE) {
        cout << "La solicitud no existe o no esta pendiente.\n";
        return;
    }
    PlanTransbordo plan = {};
    if (!consultarTransbordo(pedidos[posicion], puntos, conexiones, script, plan, error)) {
        cout << error << '\n';
        return;
    }
    vector<Pedido> nuevosPedidos = pedidos;
    vector<Unidad> nuevasUnidades = unidades;
    int espera = 5;
    if (!asignarTransbordo(nuevosPedidos[posicion], nuevasUnidades, puntos, conexiones, plan, espera, error)) {
        cout << "No se asigno el transbordo: " << error << '\n';
        return;
    }
    vector<CambioCSV> cambios;
    cambios.push_back({carpetaDatos + "/pedidos.csv", csvPedidos(nuevosPedidos)});
    cambios.push_back({carpetaDatos + "/unidades.csv", csvUnidades(nuevasUnidades)});
    if (!guardarCambiosCSV(cambios, error)) {
        cout << "No se guardo el transbordo: " << error << '\n';
        return;
    }
    pedidos = nuevosPedidos;
    unidades = nuevasUnidades;
    cout << "Solicitud " << codigo << " | Ruta habitual bloqueada en tramo "
         << plan.bloqueoOrigen << '-' << plan.bloqueoDestino << ".\n";
    mostrarRuta(plan.rutaPrimera);
    cout << " (Unidad " << pedidos[posicion].unidadAsignada.codigo << ", " << plan.tiempoPrimero << " min)\n";
    cout << "Transbordo en " << plan.punto << " | Espera: " << espera << " min\n";
    mostrarRuta(plan.rutaSegunda);
    cout << " (Unidad " << pedidos[posicion].unidadTransbordo.codigo << ", " << plan.tiempoSegundo << " min)\n";
    cout << "Tiempo total estimado: " << plan.tiempoPrimero + plan.tiempoSegundo + espera
         << " min | Estado: asignado.\n";
}

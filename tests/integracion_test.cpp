// Se reutilizan los lectores y el menu reales para comprobar todo el flujo.
#define main programaTransporte
#include "../cpp/main.cpp"
#undef main
#include <cassert>
#include <filesystem>
#include <chrono>

std::string leerArchivoPrueba(const std::filesystem::path& ruta) {
    std::ifstream archivo(ruta, std::ios::binary);
    return {std::istreambuf_iterator<char>(archivo), std::istreambuf_iterator<char>()};
}

std::string ejecutarMenuPrueba(const std::filesystem::path& datos, const std::string& texto) {
    std::istringstream entrada(texto);
    std::ostringstream salida;
    auto* anteriorEntrada = std::cin.rdbuf(entrada.rdbuf());
    auto* anteriorSalida = std::cout.rdbuf(salida.rdbuf());
    std::string ruta = datos.string();
    char nombre[] = "transporte";
    char* argumentos[] = {nombre, &ruta[0], nullptr};
    int resultado = programaTransporte(2, argumentos);
    std::cin.rdbuf(anteriorEntrada);
    std::cout.rdbuf(anteriorSalida);
    std::cin.clear();
    assert(resultado == 0);
    return salida.str();
}

int main() {
    const auto carpeta = std::filesystem::temp_directory_path() /
        ("prueba transporte " + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    assert(std::filesystem::create_directory(carpeta));
    for (const auto& nombre : {"puntos.csv", "conexiones.csv", "unidades.csv", "pedidos.csv"})
        std::filesystem::copy_file(std::filesystem::path("tests/data") / nombre, carpeta / nombre);

    auto resultado = ejecutarMenuPrueba(carpeta,
        "8\n1\nP06\nNuevo paradero\n0\nSurco\n0\n"
        "8\n2\nP03\nP06\n1.5\n5\n0\n"
        "8\n3\nU04\nBus nuevo\n40\n3.5\n2\n0\n0\n500\n"
        "8\n4\nS003\nP01\nP06\n30\n1\n19:00\n7\n");
    assert(resultado.find("Registro guardado en puntos.csv") != std::string::npos);
    assert(resultado.find("Registro guardado en conexiones.csv") != std::string::npos);
    assert(resultado.find("Registro guardado en unidades.csv") != std::string::npos);
    assert(resultado.find("Registro guardado en pedidos.csv") != std::string::npos);

    std::vector<Punto> puntos;
    std::vector<Conexion> conexiones;
    std::vector<Unidad> unidades;
    std::vector<Pedido> pedidos;
    assert(cargarPuntos((carpeta / "puntos.csv").string(), puntos));
    assert(cargarConexiones((carpeta / "conexiones.csv").string(), conexiones));
    assert(cargarUnidades((carpeta / "unidades.csv").string(), unidades));
    assert(cargarPedidos((carpeta / "pedidos.csv").string(), pedidos, puntos, unidades));
    assert(puntos.size() == 6 && conexiones.size() == 6 && unidades.size() == 4 && pedidos.size() == 5);
    assert(puntos[1].permiteTransbordo && !puntos.back().permiteTransbordo);
    assert(unidades.back().tipoCombustible == ELECTRICO && unidades.back().factorEmision == 0);
    assert(pedidos.back().estado == PENDIENTE && pedidos.back().unidadTransbordo.codigo.empty());

    std::string antes = leerArchivoPrueba(carpeta / "puntos.csv");
    resultado = ejecutarMenuPrueba(carpeta, "8\n1\nP01\nRepetido\n0\nAte\n0\n7\n");
    assert(resultado.find("No se registro") != std::string::npos);
    assert(leerArchivoPrueba(carpeta / "puntos.csv") == antes);
    resultado = ejecutarMenuPrueba(carpeta, "8\n2\nP01\nP05\nabc\n7\n");
    assert(resultado.find("No se registro") != std::string::npos);
    resultado = ejecutarMenuPrueba(carpeta, "texto\n7\n");
    assert(resultado.find("Ingrese una opcion numerica") != std::string::npos);
    ejecutarMenuPrueba(carpeta, ""); // EOF no debe dejar el menu en un ciclo infinito.

    std::string error;
    assert(!guardarCambiosCSV({{(carpeta / "puntos.csv").string(), "cambio"},
                              {(carpeta / "no_existe.csv").string(), "otro"}}, error));
    assert(leerArchivoPrueba(carpeta / "puntos.csv") == antes);

    // Ejecuta SWI-Prolog de verdad, consulta la alternativa y persiste ambos buses.
    resultado = ejecutarMenuPrueba(carpeta, "9\nS001\n7\n");
    assert(resultado.find("Tiempo total estimado: 29 min") != std::string::npos);
    assert(resultado.find("Unidad U01") != std::string::npos);
    assert(resultado.find("Unidad U02") != std::string::npos);
    unidades.clear();
    pedidos.clear();
    assert(cargarUnidades((carpeta / "unidades.csv").string(), unidades));
    assert(cargarPedidos((carpeta / "pedidos.csv").string(), pedidos, puntos, unidades));
    assert(unidades[0].estado == OCUPADA && unidades[1].estado == OCUPADA);
    assert(pedidos[0].estado == ASIGNADO && pedidos[0].unidadTransbordo.codigo == "U02");
    antes = leerArchivoPrueba(carpeta / "pedidos.csv");
    resultado = ejecutarMenuPrueba(carpeta, "9\nS001\n9\nS002\n9\nS004\n9\nS005\n5\n3\n7\n");
    assert(resultado.find("no esta pendiente") != std::string::npos);
    assert(resultado.find("Se requieren dos buses distintos") != std::string::npos);
    assert(resultado.find("No existe una alternativa") != std::string::npos);
    assert(resultado.find("no requiere transbordo") != std::string::npos);


    assert(leerArchivoPrueba(carpeta / "pedidos.csv") == antes);
    for (const auto& nombre : {"puntos.csv", "conexiones.csv", "unidades.csv", "pedidos.csv"})
        std::filesystem::remove(carpeta / nombre);
    std::filesystem::remove(carpeta);
    std::cout << "Integracion: menu, cuatro registros, reinicio y transbordo C++/Prolog correctos.\n";
}

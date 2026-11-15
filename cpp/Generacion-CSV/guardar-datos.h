#include <string>
#include <vector>

using namespace std;

struct Punto;
struct Conexion;
struct Unidad;
struct Pedido;

bool guardarPuntos(const vector<Punto>& puntos, const string& ruta = "data/puntos.csv");

bool guardarConexiones(const vector<Conexion>& conexiones, const string& ruta = "data/conexiones.csv");

bool guardarUnidades(const vector<Unidad>& unidades, const string& ruta = "data/unidades.csv");

bool guardarPedidos(const vector<Pedido>& pedidos, const string& ruta = "data/pedidos.csv");


#include "data/structures.h"
#include "CargaInicial/carga-inicial.h"
#include "MenuPrincipal/menu-principal.h"
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
  vector<Punto> puntos;
  vector<Conexion> conexiones;
  vector<Unidad> unidades;
  vector<Pedido> pedidos;
  string carpetaDatos = argc > 1 ? argv[1] : "data";

  if (!cargarDatosIniciales(puntos, conexiones, unidades, pedidos, carpetaDatos)) {
    return 1;
  }

  ejecutarMenuPrincipal(puntos, conexiones, unidades, pedidos, carpetaDatos);
  return 0;
}

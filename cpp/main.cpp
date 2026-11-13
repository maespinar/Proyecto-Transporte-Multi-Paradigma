#include "data/structures.h"
#include "CargaInicial/carga-inicial.h"
#include "MenuPrincipal/menu-principal.h"
#include <vector>

using namespace std;

int main() {
  vector<Punto> puntos;
  vector<Conexion> conexiones;
  vector<Unidad> unidades;
  vector<Pedido> pedidos;

  if (!cargarDatosIniciales(puntos, conexiones, unidades, pedidos)) {
    return 1;
  }

  ejecutarMenuPrincipal(puntos, conexiones, unidades, pedidos);

  return 0;
}

#include "marcar-pedido.h"

void marcarPedidoEntregado(Pedido& pedido) {
    pedido.estado = ENTREGADO;
    pedido.unidadAsignada.estado = DISPONIBLE; 
    cout << "El pedido con código " << pedido.codigo << " ha sido marcado como ENTREGADO." << endl;
    cout << "La unidad con código " << pedido.unidadAsignada.codigo << " ahora está DISPONIBLE." << endl;
}
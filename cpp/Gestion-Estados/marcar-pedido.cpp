#include "marcar-pedido.h"

void marcarPedidoEntregado(Pedido& pedido, Unidad& unidadAsignada) {
    if (pedido.estado == ASIGNADO) {
        if (unidadAsignada.codigo == pedido.unidadAsignada.codigo) {
            pedido.estado = ENTREGADO;    
            pedido.unidadAsignada.estado = DISPONIBLE;
            unidadAsignada.estado = DISPONIBLE; 
            cout << "El pedido con código " << pedido.codigo << " ha sido marcado como ENTREGADO." << endl;
            cout << "La unidad con código " << pedido.unidadAsignada.codigo << " ahora está DISPONIBLE." << endl;
        } else {
            cout << "Error: La unidad asignada al pedido no coincide con la unidad proporcionada." << endl;
        }
    } else {
        cout << "Error: El pedido no está en estado ASIGNADO y no puede ser marcado como ENTREGADO." << endl;
    }
}
//nodo.cpp

/*Por Adrian Rieker Gonzalez*/

#include "nodo.h"

//Metodo que cuando es pedido a un nodo elimina todos los que hay enganchados
//de forma recusiva
void nodo::eliminar_todo() {
	if (next)
		next->eliminar_todo();
	if ((this->info))

		//Se elimina lo que contiene el nodo y a continuacion 
		//el nodo en si mismo
		delete this->info;
	delete this;
}

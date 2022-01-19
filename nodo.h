
#ifndef _NODO_
#define _NODO_

//nodo.h

/*Clase nodo por:
	Adrian Rieker Gonzalez*/

#include"Shape.h"
#include<new>
#include<iostream>

using namespace std;

//nodo implementado como clase
class nodo {
private:
	Shape* info;
	nodo* next;
public:
	//Constructor por parametro y por defecto
	nodo(Shape* xd, nodo* sig) { info = xd; next = sig; };
	nodo() { info = NULL; next = NULL; };

	//Metodo de nodo que elimina todos los nodos 
	void eliminar_todo();

	//Metodos get y set
	void setInfo(Shape* nuevo) { info = nuevo; };
	Shape* getInfo() {return info; };
	void setNext(nodo* nuevo) { next = nuevo; };
	nodo* getNext() { return next; };
};

#endif //_NODO_

/*ObjectsList.cpp*/

/*Por: Adrian Rieker Gonzalez
	   Celia Ibanez Martin  */

#include "ObjectsList.h"
#include<stdio.h>
#include<new>
#include "Shape.h"
#include "Asteroid.h"

using namespace std;

//Constructor por defecto
ObjectsList::ObjectsList() {
	int i;
	Asteroid* aux_asteroide = NULL;
	head = NULL; //Una lista vacia.
	n = 0;    
	//Se crea la nave y se anade a la lista
	theShip = new Ship; 
	this->add(theShip);

	//Se crean los NUMASTEROIDS asteroides en el heap y se anaden 
	//sus referencias a la lista
	for(i = 0; i < NUMASTEROIDS; i++) {
		aux_asteroide = new Asteroid(3);
		this->add(aux_asteroide);
	}

	this->reposition(theShip);
}

//Destructor. Se eliminan todos los nodos
//mirar nodo::delete_nodos
ObjectsList:: ~ObjectsList() {
	head->eliminar_todo();

} 

//Se manda el mensaje move a todas las shape
void ObjectsList::move() {
	nodo* aux = head;
	//Se recorren todos los nodos 
	while (aux) {
		if(aux -> getInfo())
		aux->getInfo()->move();
		aux = aux->getNext();
	}
}

//Se manda el mensaje draw a todas las shape.
void ObjectsList::draw() {
	nodo* aux = head;
	while (aux) {
		if (aux -> getInfo()!= NULL) 
		{
			aux->getInfo()->draw();
			aux = aux->getNext();
		}
	}
}

//Para eliminar una Shape de la lista.
//Lo hemos hecho comparando los numeros de las Shape.
void ObjectsList::remove(Shape* figura) {
	nodo* tmp;
	nodo* p = head;
	
	if (figura && p) {

		//En caso de que la Shape buscada sea la primera de la lista.
		if (figura->getNo() == p->getInfo()->getNo()) {
			head = head->getNext();
			
			//Primero se elimina (libera memoria) de lo que contiene el nodo
			//y posteriormente se elimina el nodo.
			delete p->getInfo();
			delete p;
			n--;
			return;
		}

		//En caso de que la Shape buscada NO sea la primera de la lista.
		while ((tmp = p->getNext()) != NULL) {
			if (tmp->getInfo()->getNo() == figura->getNo()) {
				p->setNext(tmp->getNext());
				delete tmp->getInfo();
				delete tmp;
				n--;
				return;
			}
			else p = p->getNext();
		}
	}
}

//Se anade una nueva shape a la lista
void ObjectsList::add(Shape* nuevo) {
	
		nodo* aux = new nodo(nuevo, head);
		head = aux;
		n++;
		return;
}

//Collisions.
/*Se ha modificado respecto al prototipo original para anadir el parametro OvniBullet_
para detectar las colisiones entre el proyectil disparado por el ovni 
y la nave*/
/*Ahora collisions devolvera:
Colision Ovni-Proyectil de la nave: 5,6,7, dependiendo de si el ovni era BIG, MEDIUM o SMALL
Colision Proyectil del Ovni - Nave: 8
El resto sigue siendo como lo pedido en la memoria */

int ObjectsList::collisions(Bullet* labala,Bullet* OvniBullet_, Ship* elbarco, float* pos_) {
	nodo* auxHead = head; // lo que recorrera la lista
	
	Asteroid* auxAsteroid; // Puntero para poder acceder a los metodos de asteroid
	int auxsize_; //necesario para el tamano del ovni
	
	Ovni* auxOvni;
	float pos1[3]; //Variable para x1,y1
	float pos2[3]; //Variable para x2,y2
	
	//Se reccorren todos los nodos de la lista enlazada
	while (auxHead)
	{
		//Se comprueba si el campo info apunta a un asteroide
		//En caso de que si, se guarda en auxAsteroide y se ejecuta el if
		if (auxAsteroid = dynamic_cast<Asteroid*> (auxHead->getInfo()))
		{
			auxAsteroid->getPos(pos1);
			//Si hay una bala se comprobara si da a un asteroide o al ovni
			if (labala)
			{
				labala->getPos(pos2);
				//Comprobacion de la distancia
				if (mydistance(pos2[X], pos2[Y], pos1[X], pos1[Y]) <= labala->getSize() + auxAsteroid->getSize())
				{
					if (auxAsteroid->getSize() == SMALL)
					{
						//se quita el asteroide y el proyectil
						this->remove(auxAsteroid);
						this->remove(labala);
						//posicion para la explosion 
						pos_[X] = pos1[X];
						pos_[Y] = pos1[Y];
						return 4;
					}
					else
					{
						this->add(auxAsteroid->split());
						this->remove(labala);
						pos_[X] = pos1[X];
						pos_[Y] = pos1[Y];
						return (auxAsteroid->getSize() == BIG) ? 3 : 2;
					}
				}
			}
			//Comprobacion colision nave-asteroide
			if (elbarco)
			{
				elbarco->getPos(pos2);
				if (mydistance(pos2[X], pos2[Y], pos1[X], pos1[Y]) <= auxAsteroid->getSize() + elbarco->getSize())
				{

					pos_[X] = pos2[X];
					pos_[Y] = pos2[Y];
					return 1;
				}
			}
		}

		//Si hay ovni hay que comprobar la colision ovni-proyectil y proyectil_ovni - nave
		if (auxOvni = dynamic_cast<Ovni*> (auxHead->getInfo()))
		{
			auxOvni->getPos(pos1);
			
			//Si la nave ha disparado
			//comprobacion colision ovni-proyectil
			//devuelve 7 si tamano = BIG,6 si tamano = medium y 5 si tamano = small
			if (labala)
			{
				labala->getPos(pos2);
				if (mydistance(pos2[X], pos2[Y], pos1[X], pos1[Y]) <= labala->getSize() + auxOvni->getSize())
				{
					auxsize_ = auxOvni->getSize();
					this->remove(auxOvni);
					this->remove(labala);
					pos_[X] = pos1[X];
					pos_[Y] = pos1[Y];
					return(auxsize_ + 4);
				}
			}
		}

		//Para detectar la colision entre el proyectil disparado por el ovni y la nave
		if (OvniBullet_)
		{
			OvniBullet_->getPos(pos1);
			if (elbarco)
			{
				elbarco->getPos(pos2);
				if (mydistance(pos2[X], pos2[Y], pos1[X], pos1[Y]) <= (elbarco->getSize() + OvniBullet_->getSize()) + 0.7)
				{
					pos_[X] = pos1[X];
					pos_[Y] = pos1[Y];
					this->remove(OvniBullet_);
					return 8;
				}
			}
		}

		//Se mira el siguiente nodo
		auxHead = auxHead->getNext(); 
		}
	//Si se llega hasta aqui, no hay colision
	return 0;
}
	
void ObjectsList::reposition(Ship* ship_) {
	nodo* auxHead = head;
	Asteroid* auxAsteroid;
	float pos1[3];
	float pos2[3];
	
	//mientras auxHead apunte a un nodo
	while(auxHead)
	{
		//si el nodo contiene un asteroide
		//se guarda su direccion en auxAsteroid
		if (auxAsteroid = dynamic_cast<Asteroid*> (auxHead->getInfo()))
		{
			auxAsteroid->getPos(pos1);
			// Se comprueba que ship_ no es null
			if (ship_)
			{
				ship_->getPos(pos2);
				//Si el asteroide esta demasiado cerca
				if(mydistance(pos2[X], pos2[Y], pos1[X], pos1[Y]) <= auxAsteroid->getSize() + theShip->getSize())
				{
					auxAsteroid->reposition();
				}
			}
		}
		auxHead = auxHead->getNext();
	}
}


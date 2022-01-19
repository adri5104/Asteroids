#ifndef _OBJETSLIST_
#define _OBJETSLIST_
/*ObjectsList.h*/

/*Por: Adrian Rieker Gonzalez
       Celia Ibanez Martin*/

/*NOTA: Hemos decidido implementar nodo como una clase aparte.
Mirar nodo.cpp/.h */

#include<list> 
#include "Shape.h"
#include "Ship.h"
#include"commonstuff.h"
#include"nodo.h"
#include"Ovni.h"

using namespace std;

class ObjectsList
{
	private:
		int n; //numero de shapes 
		nodo* head; //Puntero al primer nodo
	public:	
		Ship* theShip; 
		ObjectsList(); //Constructor
		~ObjectsList(); //Destructor
		void move();
		void draw();
		void add(Shape*);
		void remove(Shape*);
		Ship* getShip() { return theShip; };
		int collisions(Bullet*,Bullet*, Ship*,  float*); //bool = true si se comprueban cosas d
		void reposition(Ship*);
};

#endif // !_OBJETSLIST_
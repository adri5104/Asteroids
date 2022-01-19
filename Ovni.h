//Ovni.h

/*Por: Adrian Rieker Gonzalez*/
/*El funcionamiento de los metodos esta explicado en el fichero 
Ovni.cpp*/

#ifndef __OVNI__
#define __OVNI__

#include "commonstuff.h"
#include "Shape.h"
#include "Bullet.h"
#include "ObjectsList.h"
#include "Ship.h"

class Ovni :public Shape {
private:
	int size;
	//Para cosas del ovni
	int time; 
	//Para que el ovni cambie de direccion cada cierto tiempo
	void OvniLogic();
public:
	Ovni(int);
	void draw(); 
	float getSize(); 
	Bullet* fire(Ship*);
};
#endif
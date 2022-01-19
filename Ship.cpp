#include "commonstuff.h"
#include "Ship.h"
#include "Bullet.h"
extern int nShips;

Ship::Ship()
{
	

	pos[X] = 0;
	pos[Y] = 0;
	pos[Z] = 0;

	rot[Z] = 0;
	rot[X] = -90;
	rot[Y] = 0;

	rspeed[X] = 0;
	rspeed[Y] = 0;
	rspeed[Z] = 0;

	tspeed[X] = 0;
	tspeed[Y] = 0;
	tspeed[Z] = 0;

	color[R] = 1;
	color[G] = 1;
	color[B] = 1;
}

 //la funcion rotar que nos permite dirigir la nave.
 //utilizamos thrustnumber para saber cuanta velocidad tiene la nave antes de la rotación
 //y asi que despues de girar mantenga la velocidad en linea recta con la nave

void Ship::rotate(float rx, float ry, float rz)
{
	rot[X] += rx;  rot[Y] += ry;  rot[Z] += rz;
	tspeed[X] = 0;
	tspeed[Y] = 0;
	if (thrustnumber) thrust(SHIPSPEED * thrustnumber);
}

Bullet* Ship::fire() //hacemos que la nave dispare una bala en la direccion que apunta
{
	Bullet* tmp;

	tmp = new Bullet(pos[X], pos[Y], BULLETSPEED * sin(D2R * rot[Y]), BULLETSPEED * cos(D2R * rot[Y]));

	return tmp;
}


//usamos thrust para aumentar y disminuir la velocidad de la nave, o si vale 0, que fse pare
//jugamos con ello en el main para que la nave frene por completo
void Ship::thrust(float howmuch)
{
	if (!howmuch)
	{
		tspeed[X] = 0;
		tspeed[Y] = 0;
		return;
	}
	tspeed[X] += howmuch * sin(D2R * rot[Y]);
	tspeed[Y] += howmuch * cos(D2R * rot[Y]);
}


void Ship::hyperjump() //una funcion para que la nave aparezca en otro sitio
//pensamos que al desplazarse sería mejor que la nave se detuviera por completo
//en vez de mantener su velocidad y que se estrelle con otras naves
{
	pos[X] = RAND_DOM(0, (SIZEX - 2)) - SIZEX2;
	pos[Y] = RAND_DOM(0, (SIZEY - 2)) - SIZEY2;
	tspeed[X] = 0;
    tspeed[Y] = 0;
    thrustnumber = 0;

}


void Ship::draw() //para dibujar la nave
{
	predraw();
	glColor3f(1, 1, 1);
	glutSolidCone(0.25, 1, 20, 10);

	
	glColor3f(1, 0, 1);
	glTranslatef(0, 0, -0.2);
	glutSolidTorus(0.05, 0.15, 7, 9);

	glColor3f(1, 0.15, 0.5);
	glTranslatef(0, 0, -0.21);
	glutSolidTorus(0.05, 0.075, 7, 9);

	glColor3f(1, 0.15, 0.5);
	glTranslatef(0, 0, -0.21);
	glutSolidTorus(0.02, 0.05, 7, 9);
	

	postdraw();
}


float Ship::getSize() //por si queremos saber que la nave tiene un tamaño de 0.25
{
	return (0.25);
}

void Ship::resetpos() //funcion para cuando la nave se estrella y hay que ponerla otra vez en el centro, sin velocidad.
{
	pos[X] = 0;
	pos[Y] = 0;
	pos[Z] = 0;

	rot[Z] = 0;
	rot[X] = -90;
	rot[Y] = 0;

	rspeed[X] = 0;
	rspeed[Y] = 0;
	rspeed[Z] = 0;

	tspeed[X] = 0;
	tspeed[Y] = 0;
	tspeed[Z] = 0;

	color[R] = 1 + 0.1 * nShips;
	color[G] = 0.4;
	color[B] = 0;
}


//estas funciones sirven para que en el main thrustnumber lleve la cuenta de que velocidad tiene la nave

void Ship::thrustnumberup() {
	thrustnumber++;
}

void Ship::thrustnumberdown() {
	thrustnumber--;
}

void Ship::thrustnumbernull() {
	thrustnumber = 0;
}

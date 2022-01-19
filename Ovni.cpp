
#include "Ovni.h"

extern Ship*  theShip;
extern ObjectsList worldobjects;

//Constructor
Ovni::Ovni(int t)
{
	size = t;

	pos[X] = RAND_DOM(0, SIZEX);
	pos[Y] = RAND_DOM(0, SIZEY);
	pos[Z] = 0;

	rot[Z] = 0;
	rot[X] = -75;
	rot[Y] = 0;

	rspeed[X] = 0;
	rspeed[Y] = 0;
	rspeed[Z] = 1.6;

	tspeed[X] = OVNISPEED;
	tspeed[Y] = 0;
	tspeed[Z] = 0;

	color[R] = 1;
	color[G] = 1;
	color[B] = 1;

	time = 0;

	//  printf("Mi no is %d",mynumber);
}

void Ovni::draw()
{
	if(this) this ->OvniLogic();
	predraw();
	glutSolidTorus(size*0.1, size*0.2, 5,8);
	glTranslatef(0, 0, 0.1);
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glutWireSphere(size * 0.15, 20, 20);
	postdraw();
}

float Ovni::getSize()
{
	return (size);
}

//Metodo privado que se llama cada vez que se ejecuta draw.
//Sirve para que el ovni cambie de direccion horizontal cada cierto tiempo
//y para el movimiento vertical "erratico".
void Ovni::OvniLogic()
{
	time++;
	if (time >= 150)
	{
		tspeed[X] = -tspeed[X];
		time = 0;
	}
	pos[Y] = 1* sin(time * OVNISPEED*3) + 4;
}

//Metodo que sirve para que el ovni dispare.
//La direccion de la bala es mejorable, deberia ir dirigida hacia la nave.
Bullet* Ovni::fire(Ship* unaNave)
{
	float ShipPos[3];
	unaNave->getPos(ShipPos);
	//theShip->getPos(ShipPos); 
	Bullet* aux = new Bullet(pos[X], pos[Y], BULLETSPEED_OVNI * ((ShipPos[Y]-pos[Y])/(distance2(pos[X], pos[Y], ShipPos[X], ShipPos[Y]))), BULLETSPEED_OVNI * ((ShipPos[X] - pos[X]) / (distance2(pos[X], pos[Y], ShipPos[X], ShipPos[Y]))));
	return aux;
}
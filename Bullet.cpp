
#include "commonstuff.h"
#include "Bullet.h"

Bullet::Bullet(float px, float py, float vx, float vy)
{
  pos[X] = px;
  pos[Y] = py;
  pos[Z] = 0;

  rot[Z] = 0;
  rot[X] = 0;
  rot[Y] = 0;

  rspeed[X] = 0;
  rspeed[Y] = 0;
  rspeed[Z] = 0;

  tspeed[X] = vx;
  tspeed[Y] = vy;
  tspeed[Z] = 0;

  color[R] = 1;
  color[G] = 0.7;
  color[B] = 0;
}

void Bullet::draw()
{
  predraw();
  glutWireSphere(0.1,5,5);	
  postdraw();
}


float Bullet::getSize()
{
  return (0.1);
}

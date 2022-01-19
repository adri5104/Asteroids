
//***********************
// Asteroids Open GL
// Claudio Rossi, Universidad Politécnica de Madrid
// (C) 2015
//***********************

#include "commonstuff.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>
#include "Shape.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Ship.h"
#include "ObjectsList.h"
#include "Flame.h"
#include"Ovni.h"

//***********************
// Prototipos de funciones
//***********************

// Callback de la logica del juego
void myLogic();

//Callback de dibujo
void OnDibuja(void);

//Callbacks para teclado y raton
void OnKeyboardDown(unsigned char key, int x, int y);
void OnMouseBtn(int button, int state,int x, int y);
void OnMouseMoveBtn(int x, int y);
void OnSpecKeyboardDown(int key, int x, int y);
void printdata();
void gameover(int score);

//***********************
// Variables globales
//***********************

// Posición y step de la camara
float cam_pos[6]={0, 0, 27};

// posición de la explosión
float expl_pos[2]={-1000,-1000};

// Modo del Mouse
int MODE=NONE;

// Objetos globales
ObjectsList worldobjects;
Ship *theShip=NULL;
Bullet *theBullet=NULL;
Flame *theFlame=NULL;

// Objetos globales para el ovni
Ovni* theOvni = NULL;
Bullet* OvniBullet;

// Varias constantes usadas en el programa
int shotTime=0;
int nShips=3;
int score=0;
int FlameTime=0;
int FT=20;

//Variable para saber cuando debe aparecer el ovni
// y otra para que desaparezca el proyectil del ovni
int OvniTime = 0;
int shotTime_ovni = 0;

//***********************
// Programa principal
//***********************

int main(int argc,char* argv[])
{

  // inicializaciones

  //Creacion y definicion de la ventana
  glutInit(&argc, argv);
  glutInitWindowSize(WINX,WINY);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Asteroids");

  //Habilita las luces, la renderizacion y el color de los materiales
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glClearColor(0.0f, 0.0f, 0.0f, 5.0f);

  //Define la proyeccion
  glMatrixMode(GL_PROJECTION);
  gluPerspective( 40.0, WINX/WINY, 0.1, 50);

  //Para definir el punto de vista
  glMatrixMode(GL_MODELVIEW);

  // Define call backs te GLUT

  // Display function: contiene las instrucciones de dibujo
  glutDisplayFunc(OnDibuja);
    // Idle function: contiene la logica del juego
  glutIdleFunc(myLogic);

  // Callbacks de teclado y ratón
  glutKeyboardFunc(OnKeyboardDown);
  glutSpecialFunc(OnSpecKeyboardDown);
  glutMotionFunc(OnMouseMoveBtn);
  glutMouseFunc(OnMouseBtn);


  // Posicciona el punto de vista (cámara)
  gluLookAt(cam_pos[0],cam_pos[1],cam_pos[2],  // posicion del  ojo
	    0.0, 0.0, 0.0,		        // hacia que punto mira
	    0.0, 1.0, 0.0);         // vector "UP"  (vertical positivo)



  // Creacción de los objetos iniciales
  theShip = worldobjects.getShip();


  // ObjectsList es declarada estática, se inicializa "automaticamente" - contiene los asteroides

  // bucle infinito de Open GL
  glutMainLoop();

  // Esto solo sirve para que el compilador no proteste, nunca se llegará aquí
  return 0;

}

/************************** FIN DEL MAIN ************************************/


//***********************
// Callbacks
//***********************

// Imprime puntuacción y num. de naves
void printdata()
{
  int i,l;
  char buffer[50];

  glPushMatrix();

  sprintf(buffer,"Ships: %d       Score: %d",nShips,score); //NOTA: Nos daba problemas con sprintf en Visual Studio. Para que compilara
															//en visual studio tuvimos que cambiarlo por sprintf_s
  
  l=strlen(buffer);

  glDisable(GL_LIGHTING);
  glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos3f(-9, 9,-0.1);
  for( i=0; i < l; i++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]); // Print a character on the screen
    }
  glEnable(GL_LIGHTING);

  glPopMatrix();

}


//no usada
// La hemos modificado un poco pero no hemos tenido tiempo de implementarlo


void gameover(int score)
{
  int time = 0;
  while (time++ >= 400) {
      int i, l;
      char buffer[30];

      glPushMatrix();

      sprintf(buffer, "GAME OVER! Points: %d", score);//NOTA: Nos daba problemas con sprintf en Visual Studio. Para que compilara
														//en visual studio tuvimos que cambiarlo por sprintf_s
  

      l = strlen(buffer);

      glDisable(GL_LIGHTING);
      glColor3f(1.0f, 1.0f, 1.0f);
      glRasterPos3f(-9, 9, -0.1);
      for (i = 0; i < l; i++)
      {
          glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]); // Print a character on the screen
      }
      glEnable(GL_LIGHTING);

      glPopMatrix();
  }
  exit(1);
}


// Logica del juego: mueve los objeto mandando el mensaje "move"
void myLogic()
{
    int res;

  // borra el proyectil después de cierto tiempo si no ha dado con nada
  if(shotTime++>MAXSHOTTIME)
    {
      worldobjects.remove(theBullet);
      theBullet = NULL;
      shotTime = 0;
    }

  // Pide al mudo que mueve los objetos
  worldobjects.move();

  // Pide si ha habido colisión, pasa referencia a proyectil y nave, retorna tipo de colisión y posición de la colisión
  // res==0:  No ha colisicón
  // res==1:  Asteroide/Nave
  // res==2,3,4:  Asteroide/Proyectil, depende del tipo de asteroide (grande/mediano/pequeño)
  // NUEVO: colisiones del ovni
  // res = 5,6,7: Proyectil(de la nave)/ Ovni, depende del tipo de Ovni (grande/mediano/pequeño)
  res = worldobjects.collisions(theBullet,OvniBullet,theShip,expl_pos);

  // Explosion
  if(res>0 || FlameTime>0)
    {
      FlameTime++;
      if(!theFlame)
	{
	  theFlame = new Flame(expl_pos);
	  worldobjects.add(theFlame);
	}
      else
	if(FlameTime>FT)
	  {
	    worldobjects.remove(theFlame);
	    theFlame=NULL;
	    FlameTime = 0;
	  }
    }

  if(res==1)
    {
      nShips--;

      if (nShips == 0)
      {
          printf("La puntuacion es: %d", score);
          exit(1);
      }

      theShip->resetpos();
      worldobjects.reposition(theShip);
    }

  if(res>=2 && res < 5)
    {
      theBullet = NULL;
      shotTime = 0;
      score += 100*(res-1);
    }


  //----------------- COSAS DEL OVNI -------------

   // El ovni aparece transcurrido OVNITIME_APARICION
   if((OvniTime++ >= OVNITIME_APARICION) && !theOvni)
   {
        theOvni = new Ovni((int )RAND_DOM(1,3));
        worldobjects.add(theOvni);
        OvniTime = 0;
   }

   // El Ovni desaparece pasado OVNITIME_DESAPARICION
   // a no ser que sea alcanzado por un proyectil de la nave
   if ((OvniTime++ >= OVNITIME_DESAPARICION) && theOvni)
   {
        worldobjects.remove(theOvni);
        worldobjects.remove(OvniBullet);
        OvniBullet = NULL;
        theOvni = NULL;
        OvniTime = 0;
   }

   // El ovni dispara
   if(theOvni && !OvniBullet)
   {
        OvniBullet = theOvni->fire(theShip);
        worldobjects.add(OvniBullet);
        shotTime_ovni = 0;
   }

   // El proyectil del ovni desaparece pasado un tiempo
   if ((shotTime_ovni++ >= OVNI_MAXSHOTTIME) && OvniBullet)
   {
        worldobjects.remove(OvniBullet);
        OvniBullet = NULL;
   }

   if (theOvni) shotTime_ovni++;

   // Colision ovni - proyectil (de la nave)
   if (res >= 5 && res < 8)
   {
        theOvni = NULL;
        theBullet = NULL;
        OvniTime = 0;

        //Para que desaparezca tambien el proyectil del ovni (sino el juego era muy complicado)
        worldobjects.remove(OvniBullet);
        OvniBullet = NULL;

        shotTime = 0;
        score += (res == 7) ? 200 : ((res == 6) ? 150 : 50);
    }

    // Colision proyectil del ovni contra la nave
   if (res == 8)
   {
       nShips--;
       theShip->resetpos();
       worldobjects.reposition(theShip);
       OvniBullet = NULL;
       
       //Si el ovni da a la nave desaparece
       worldobjects.remove(theOvni);
       theOvni = NULL;
       if (nShips == 0)
       {
           printf("La puntuacion es: %d", score);
           exit(1);
       }
   }
}



/**************************************************************/

void OnDibuja(void)
{
  //Borrado de la pantalla
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Manda el mensaje "draw" al mundo
  worldobjects.draw();

  // imprime datos
  printdata();

  //Al final, cambiar el buffer
  glutSwapBuffers();
  glutPostRedisplay();//se le indica que redibuje la pantalla
  /**************************************************************/}






void OnKeyboardDown(unsigned char key, int x, int y)
{
  int mod;

  mod=glutGetModifiers();

  switch(key)
    {

    case ESC:
      exit(1);
    case ' ':
      // Si no hay proyectil, lo crea
      if(!theBullet)
	{
	  theBullet=theShip->fire();
	  worldobjects.add(theBullet);

	}

      break;
    case 'q': theShip->hyperjump(); break;  // hyper jump (mueve la nave a una posición random
    case 'a':
    case 'A': theShip->rotate(0, -10, 0); break;
    case 'd':
    case 'D': theShip->rotate(0, 10, 0); break;
    case 'w':
    case 'W': theShip->thrust(SHIPSPEED); theShip->thrustnumberup(); break;
    case 's':
    case 'S': theShip->thrust(-SHIPSPEED); theShip->thrustnumberdown(); break;
    case 'z':
    case 'Z': theShip->thrust(0); theShip->thrustnumbernull(); break;  // frena en seco

  }

}


void OnSpecKeyboardDown(int key, int x, int y)
{
    switch (key)
    {

    case GLUT_KEY_DOWN: theShip->thrust(-SHIPSPEED); theShip->thrustnumberdown(); break;
        break;
    case GLUT_KEY_UP: theShip->thrust(SHIPSPEED); theShip->thrustnumberup(); break;
        break;
    case GLUT_KEY_LEFT:  theShip->rotate(0, -10, 0); break;
        break;
    case GLUT_KEY_RIGHT: theShip->rotate(0, 10, 0); break;
        break;
    case GLUT_KEY_PAGE_DOWN:
        break;
    case GLUT_KEY_PAGE_UP:
        break;
    case '-': theShip->thrust(0); theShip->thrustnumbernull(); break;
    }

}


// No usada
void OnMouseBtn(int button, int state,int x, int y)
{
  if(state==GLUT_DOWN)
    if(button==GLUT_LEFT_BUTTON)
      MODE=TRANS;
    else
      if(button==GLUT_RIGHT_BUTTON)
        MODE=ROT;
      else
        MODE=NONE;
    printf("MOUSE!\n");
}


// No usada
void  OnMouseMoveBtn  (int x, int y)
{

  //  printf("MOUSE MOVES!\n");

  if(MODE==TRANS)
    {

    }
  else
    {

    }



}

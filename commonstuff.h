/*Explicar que se ha anadido*/

#include <stdio.h>
#include <math.h>


//#ifdef linux
#ifdef __APPLE__
#include "GLUT/glut.h"
#include <stdlib.h>
#else
#include <windows.h>
#include "glut.h"
#endif

#define X 0
#define Y 1
#define Z 2

#define BIG 3
#define MEDIUM 2
#define SMALL 1

#define R 0
#define G 1
#define B 2

#define SIZEX 20
#define SIZEY 20
#define SIZEZ 0
#define SIZEX2 10
#define SIZEY2 10
#define SIZEZ2 0

#define WINX 800
#define WINY 600		

#define ASTEROID_SPEED 0.03
#define BULLETSPEED 0.5
#define SHIPSPEED 0.03
#define NUMASTEROIDS 10
#define MAXSHOTTIME 100

#define ESC 27
#define EPS 1
#define NONE  0
#define TRANS 1
#define ROT   2

// ----------- COSAS DEL OVNI ------------

#define OVNISPEED 0.03
#define BULLETSPEED_OVNI 3
#define OVNITIME_APARICION 1000 //tiempo para que el ovni aparezca si no habia uno ya
#define OVNITIME_DESAPARICION 850 //tiempo que permanece el ovni en pantalla
#define OVNI_MAXSHOTTIME 180 //Shottime para el proyectil del ovni


#ifdef linux
#define SEED_RAND(seed)  (srandom((seed)))
#define RAND_FRAC()      ((double)random()*(1.0/2147483647.0))
#else
#define SEED_RAND(seed)  (srand((seed)))
#define RAND_FRAC()       ((double)rand()*(1./RAND_MAX))
#endif

#define distance2(x1,y1,x2,y2)  (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)
#define mydistance(x1,y1,x2,y2)   sqrt(distance2(x1,y1,x2,y2))
#define RAND_DOM(lo,hi)  ((int) floor(RAND_FRAC()*(((hi)-(lo))+0.999999))+(lo))
#define RAND_DOMF(lo,hi)  ( (RAND_FRAC()*(((hi)-(lo))))+(lo))
#define RAND_BIT()       ((RAND_FRAC()>=.5)? 1 : 0 )
#define MIN(a,b)         ((a < b) ? (a) : (b))
#define MAX(a,b)         ((a > b) ? (a) : (b))
#define D2R              1.7453292519943296e-2
#define R2D              57.29577951308232

#define SQR(a) ((a)*(a))

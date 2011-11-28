
/**
 * File: realtime_surfels.h
 * --------------------
 * GLUT window for viewing surfel clouds
 * in real-time.
 * Author: Ryan Schmitt
 */

#ifndef _REALTIME_SURFELS_H_
#define _REALTIME_SURFELS_H_

/* HEADERS */

#include <glew.h>
#include <GLUT/glut.h>
#include "RDSSceneDesc.h"
#include "FlyingCamera.h"
#include "BasicModel.h"
#include "ScreenDraw.h"

/* #DEFINES */

#define CAMERA_SPEED 0.05f //speed of camera as a float
#define INVERT 1 //true or false
#define MOUSE_SENS 0.5 //float between 0.0 and 1.0

/* CODE */

namespace RDST
{

/**
* Real-Time Surfel Rendering
*/
class RealtimeSurfels
{
private:

   /* HIDDEN CTORS */
   //important for static class

   explicit RealtimeSurfels() {}
   explicit RealtimeSurfels(const RealtimeSurfels& rhs) {}
   ~RealtimeSurfels() {}

   /* INTERNAL VARIABLES */

   static int width, height, curWidth, curHeight;
   static FlyingCamera* camera;
   static bool keysDown[6];
   static int mouseX, mouseY;
   static int frame, elapsedTime, timebase, timeNow, timeLast;
   static float fps;
   static GLuint vboId;
   static SceneDescription* desc;

   /* INTERNAL FUNCTIONS */

   static void display();
   static void drawFPS();
   static void drawCamPos();
   static void drawScene();
   static void view();
   static void lights();
   static void geometry();
   static void idle();
   static void timer(int);
   static void reshape(int, int);
   static void keyboardDown(unsigned char, int, int);
   static void keyboardUp(unsigned char, int, int);
   static void motion(int, int);
   static void passiveMotion(int, int);
   static void loadVBO();

public:

   /* EXTERNAL API */

   static void Render(const SceneDescription& scene);
};

}

#endif

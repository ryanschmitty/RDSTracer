#ifndef __SCREEN_DRAW_H__
#define __SCREEN_DRAW_H__

#include <GLUT/glut.h>

//-----------------------------------------------------------------------------
// Draws text to the screen.
// NOTE: coords are from lower left screen. [0,100]
static void drawText(int x, int y, void * font, char* text)
{
   char *c;

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0.0, 100.0, 0.0, 100.0); //Sets up coord system

      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
         glLoadIdentity();

         glDisable(GL_LIGHTING);

         glRasterPos2i(x,y);
         for (c = text; *c != '\0'; c++) {
            glutBitmapCharacter(font, *c);
         }

         glEnable(GL_LIGHTING);

      glPopMatrix();

      glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
}

#endif

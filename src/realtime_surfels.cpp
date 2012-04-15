
/**
 * File: realtime_surfels.cpp
 * --------------------
 * GLUT window for viewing surfel clouds
 * in real-time.
 * Author: Ryan Schmitt
 */

#include <stdio.h>
#include "realtime_surfels.h"

namespace RDST
{

//Static variables definitions
int RealtimeSurfels::width, RealtimeSurfels::height, RealtimeSurfels::curWidth, RealtimeSurfels::curHeight;
FlyingCamera* RealtimeSurfels::camera;
bool RealtimeSurfels::keysDown[6];
int RealtimeSurfels::mouseX, RealtimeSurfels::mouseY;
int RealtimeSurfels::frame, RealtimeSurfels::elapsedTime, RealtimeSurfels::timebase, RealtimeSurfels::timeNow, RealtimeSurfels::timeLast;
float RealtimeSurfels::fps;
GLuint RealtimeSurfels::vboId;
SceneDescription* RealtimeSurfels::desc;

void RealtimeSurfels::Render(const SceneDescription& scene) {
   RealtimeSurfels::desc = const_cast<SceneDescription*>(&scene);
   RealtimeSurfels::width = RealtimeSurfels::curWidth = 800;
   RealtimeSurfels::height = RealtimeSurfels::curHeight = 600;
   RealtimeSurfels::elapsedTime = RealtimeSurfels::frame = RealtimeSurfels::timebase = RealtimeSurfels::timeNow = RealtimeSurfels::timeLast = 0;

   // Create the camera
   RealtimeSurfels::camera = new FlyingCamera(45.0,       // fov
                                              0, 0, 20,   // location
                                              1.0, 300.0);// zNear, zFar

   //OpenGL setup
   int argc = 0;
   char* argv = "needs something here";
   glutInit(&argc, &argv);
   glutInitWindowSize(RealtimeSurfels::width, RealtimeSurfels::height);
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutCreateWindow("Surfel Cloud");
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glClearColor(0.0, 0.0, 0.0, 1.0);
//   glEnable(GL_CULL_FACE);
//   glCullFace(GL_BACK);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

   //register callback functions
   glutDisplayFunc(RealtimeSurfels::display);
//   glutIdleFunc(RealtimeSurfels::idle);
   glutTimerFunc(16, timer, 16); //16 millisecond render loop (60fps)
   glutReshapeFunc(RealtimeSurfels::reshape);
   glutKeyboardFunc(RealtimeSurfels::keyboardDown);
   glutKeyboardUpFunc(RealtimeSurfels::keyboardUp);
   glutMotionFunc(RealtimeSurfels::motion);
   glutPassiveMotionFunc(RealtimeSurfels::passiveMotion);

   //Error check
   GLenum err = glewInit();
   if (err != GLEW_OK || !glewIsSupported("GL_VERSION_2_0")) {
      printf("OpenGL 2.0 not supported. No shaders!\n");
      printf("%s\n", glewGetErrorString(err));
      printf("%s\n", (char*)glGetString( GL_VERSION ) );
      exit(-1);
   }
   printf("OpenGL 2.0 supported. Using OpenGL %s \n\n",(char*)glGetString( GL_VERSION ));

   //Load VBO
   loadVBO();

   //start the main loop
   glutMainLoop();

   //Clean up
   glDeleteBuffers(1, &RealtimeSurfels::vboId);
   delete RealtimeSurfels::camera;
}

void RealtimeSurfels::display() {
   RealtimeSurfels::timeNow = glutGet(GLUT_ELAPSED_TIME);
   //handle movement
   if( RealtimeSurfels::keysDown[0] ) //w
      RealtimeSurfels::camera->moveForward(CAMERA_SPEED * (RealtimeSurfels::timeNow - RealtimeSurfels::timeLast));
   if( keysDown[1] ) //a
      camera->moveLeft(CAMERA_SPEED * (RealtimeSurfels::timeNow - RealtimeSurfels::timeLast));
   if( keysDown[2] ) //s
      camera->moveBackward(CAMERA_SPEED * (RealtimeSurfels::timeNow - RealtimeSurfels::timeLast));
   if( keysDown[3] ) //d
      camera->moveRight(CAMERA_SPEED * (RealtimeSurfels::timeNow - RealtimeSurfels::timeLast));

   //DRAW
   drawScene();

   //SCREEN TEXT
   glClear(GL_DEPTH_BUFFER_BIT);
   drawFPS();
   drawCamPos();

   /* ACTION */
   glutSwapBuffers();
//   glutPostRedisplay();
   RealtimeSurfels::timeLast = RealtimeSurfels::timeNow;
}

void RealtimeSurfels::drawFPS() {
   //calc fps
   RealtimeSurfels::frame++;
   if( RealtimeSurfels::timeNow - RealtimeSurfels::timebase > 1000 ) { //Only refresh every second
      RealtimeSurfels::fps = RealtimeSurfels::frame*1000.0/(RealtimeSurfels::timeNow-RealtimeSurfels::timebase);
      RealtimeSurfels::timebase = RealtimeSurfels::timeNow;
      RealtimeSurfels::frame = 0;
   }
   //write fps
   char fpsString[30];
   sprintf(fpsString, "fps: %g", RealtimeSurfels::fps);
   //draw fps
   glColor3f(1.0,0.0,0.0);
   drawText(85,95, GLUT_BITMAP_HELVETICA_12, fpsString);
}

void RealtimeSurfels::drawCamPos() {
   char posString[30];
   sprintf(posString, "<%.2f, %.2f, %.2f>", camera->loc.x, camera->loc.y, camera->loc.z);
   glColor3f(1.0,0.0,0.0);
   drawText(80,90, GLUT_BITMAP_HELVETICA_12, posString);
}

void RealtimeSurfels::drawScene() {
   //draw
   //   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   view();
   lights();
   geometry();
}

void RealtimeSurfels::view() {
   /* CAMERA */
   gluLookAt( camera->loc.x, camera->loc.y, camera->loc.z, // eye location
              camera->loc.x+camera->dir.x, camera->loc.y+camera->dir.y, camera->loc.z+camera->dir.z, // Look at point
              camera->up.x, camera->up.y, camera->up.z // up vector
            );
}

void RealtimeSurfels::lights() {
   /* LIGHTS */
//   glEnable(GL_LIGHTING);
   glDisable(GL_LIGHTING);
   return;
   int i=0;
   unsigned int curLight = GL_LIGHT0;
   for(std::vector<PointLightPtr>::const_iterator it = desc->lights().begin(); it != desc->lights().end(); ++it) {
      if (i++>7) break; //Only allow 8 lights
      glEnable(curLight++);
      glm::vec3 lpos = (*it)->getPos();
      glm::vec4 lcolor = (*it)->getColor();
      //Point light position
      GLfloat lightPos[] = {lpos.x, lpos.y, lpos.z, 0.0};
      //Ambient composition
      GLfloat ambientComp[] = {lcolor.r, lcolor.g, lcolor.b, 0.0};
//      GLfloat ambientComp[] = {0.0, 0.0, 0.0, 0.0};
      //Diffuse composition
//      GLfloat diffuseComp[] = {lcolor.r, lcolor.g, lcolor.b, 0.0};
      GLfloat diffuseComp[] = {0.0, 0.0, 0.0, 0.0};
      //Specular composition
      GLfloat specularComp[] = {0.0, 0.0, 0.0, 0.0};
      //Ambient light
//      GLfloat globalAmbient[] = {0.5, 0.5, 0.5, 0.0};
//      glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
      //Point light
      glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
      glLightfv(GL_LIGHT0, GL_AMBIENT, ambientComp);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseComp);
      glLightfv(GL_LIGHT0, GL_SPECULAR, specularComp);
   }
}

void RealtimeSurfels::geometry() {
   /* DRAW GEOMETRY */
   glBindBuffer(GL_ARRAY_BUFFER, vboId);

   // enable vertex arrays
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
   glEnableClientState(GL_VERTEX_ARRAY);

   // before draw, specify vertex and index arrays with their offsets
   int numVertices = desc->surfels().size() * 3; //3 vertices per surfel
   int numItemsPerList = 3*numVertices; //3 GLfloats per vertex/color/normal
   int bytesPerList = sizeof(GLfloat) * numItemsPerList;
   glVertexPointer(3, GL_FLOAT, 0, 0);
   glNormalPointer(GL_FLOAT, 0, (void*)bytesPerList);
   glColorPointer(3, GL_FLOAT, 0, (void*)(2*bytesPerList));

   //Draw a number of vertices (e.g. all of them!)
   glDrawArrays(GL_TRIANGLES, 0, 3*desc->surfels().size());

   glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
   glDisableClientState(GL_COLOR_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RealtimeSurfels::idle() {
   glutPostRedisplay();
}

void RealtimeSurfels::timer(int millisec) {
   glutTimerFunc(millisec, timer, millisec);
   glutPostRedisplay();
}

void RealtimeSurfels::reshape(int w, int h) {
   curWidth = w;
   curHeight = h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(camera->fov,
                  float(w)/h,
                  camera->zNear,
                  camera->zFar);
   glMatrixMode(GL_MODELVIEW);
   glViewport(0, 0, w, h);
}

void RealtimeSurfels::keyboardDown(unsigned char key, int x, int y ) {
   switch( key ) {
      case 'w':
         keysDown[0] = true;
         break;
      case 'a':
         keysDown[1] = true;
         break;
      case 's':
         keysDown[2] = true;
         break;
      case 'd':
         keysDown[3] = true;
         break;
   }
}

void RealtimeSurfels::keyboardUp(unsigned char key, int x, int y ) {
   switch( key ) {
      case 'w':
         keysDown[0] = false;
         break;
      case 'a':
         keysDown[1] = false;
         break;
      case 's':
         keysDown[2] = false;
         break;
      case 'd':
         keysDown[3] = false;
         break;
      default:
         break;
   }
}

void RealtimeSurfels::motion(int x, int y) {
   //Update camera
   camera->setYaw(float(x-mouseX)*MOUSE_SENS);
   float dy;
   if(INVERT)
      dy = mouseY-y;
   else
      dy = y-mouseY;
   camera->setPitch(dy*MOUSE_SENS);

   //Store new mouse position
   mouseX = x;
   mouseY = y;

   //Display changes
//   glutPostRedisplay();
}

void RealtimeSurfels::passiveMotion(int x, int y) {
   mouseX = x;
   mouseY = y;
}

void RealtimeSurfels::loadVBO() {

   const std::vector<GeomObjectPtr>& tris = desc->surfels();
   //Allocate space on the stack since we won't need it later
   const unsigned int dataSize = 3*3*tris.size();
   GLfloat* vertices = new GLfloat[dataSize];
   GLfloat* normals  = new GLfloat[dataSize];
   GLfloat* colors   = new GLfloat[dataSize];

   int i=0;
   for(std::vector<GeomObjectPtr>::const_iterator it = tris.begin(); it != tris.end(); ++it) {
      //Get all the data!
      Triangle* tri = static_cast<Triangle*>(&(**it));
      glm::vec3 v0 = tri->getVertex0();
      glm::vec3 v1 = tri->getVertex1();
      glm::vec3 v2 = tri->getVertex2();
      glm::vec3 vCp1(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
      glm::vec3 vCp2(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
      glm::vec3 normal = glm::cross(vCp1, vCp2);
      normal = glm::normalize(normal);
      glm::vec4 color = tri->getColor();
      //Store all the data!
      //Vertex 0
      vertices[i] = v0.x;
      normals[i]  = normal.x;
      colors[i]   = color.r;
      ++i;
      vertices[i] = v0.y;
      normals[i]  = normal.y;
      colors[i]   = color.g;
      ++i;
      vertices[i] = v0.z;
      normals[i]  = normal.z;
      colors[i]   = color.b;
      ++i;
      //Vertex 1
      vertices[i] = v1.x;
      normals[i]  = normal.x;
      colors[i]   = color.r;
      ++i;
      vertices[i] = v1.y;
      normals[i]  = normal.y;
      colors[i]   = color.g;
      ++i;
      vertices[i] = v1.z;
      normals[i]  = normal.z;
      colors[i]   = color.b;
      ++i;
      //Vertex 2
      vertices[i] = v2.x;
      normals[i]  = normal.x;
      colors[i]   = color.r;
      ++i;
      vertices[i] = v2.y;
      normals[i]  = normal.y;
      colors[i]   = color.g;
      ++i;
      vertices[i] = v2.z;
      normals[i]  = normal.z;
      colors[i]   = color.b;
      ++i;
   }

   int numItems = 3*3*tris.size();
   size_t bytesPerList = sizeof(GLfloat)*numItems;

   //Generate VBO and load it full of data
   glGenBuffers(1, &vboId);
   glBindBuffer(GL_ARRAY_BUFFER, vboId);
   glBufferData(GL_ARRAY_BUFFER, 3*bytesPerList, 0, GL_STATIC_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0,              bytesPerList, vertices);
   glBufferSubData(GL_ARRAY_BUFFER, bytesPerList,   bytesPerList, normals);
   glBufferSubData(GL_ARRAY_BUFFER, 2*bytesPerList, bytesPerList, colors);

   //Clean up
   delete [] vertices;
   delete [] normals;
   delete [] colors;
}

}

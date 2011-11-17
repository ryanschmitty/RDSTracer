#include <glew.h>
#include <GLUT/glut.h>
#include "FlyingCamera.h"
#include "BasicModel.h"
#include "ScreenDraw.h"

/* #DEFINES */

#define CAMERA_SPEED 0.05f //speed of camera as a float
#define INVERT 1 //true or false
#define MOUSE_SENS 0.5 //float between 0.0 and 1.0

/* VARIABLES */

int width, height, curWidth, curHeight;
FlyingCamera* camera;
bool keysDown[6];
int mouseX, mouseY;
const int NUM_MODELS = 3;
BasicModel* bm[NUM_MODELS];
int model;
//MRT/Deferred things
//DeferredShadingEngine* dfe;
//Shader* gatherShader;
//Shader* directTargetShader;
//Shader* directionalLightShader;
//Shader* pointLightShader;
//GLuint  normalbuffer, positionbuffer, colorbuffer;
//enum    RENDER_MODE{FIXED_FUNCTION, NORMALS, POSITIONS, COLORS, DEFERRED, NUM_MODES};
//int     mode;
//float lightPos[3];
//float radius;
// vars for fps
int frame, elapsedTime, timebase, timeNow, timeLast;
float fps;

/* FUNCTIONS */

void display();
void useFixedFunction();
void useDeferred();
void doDirectionalAndAmbient();
void doPoint();
void drawPointLights();
void doDirectTarget(GLuint);
void drawFPS();
void drawCamPos();
void drawScene();
void view();
void lights();
void geometry();
void idle();
void reshape(int, int);
void keyboardDown(unsigned char, int, int);
void keyboardUp(unsigned char, int, int);
void motion(int, int);
void passiveMotion(int, int);

/* REAL CODE */

void display() {
   timeNow = glutGet(GLUT_ELAPSED_TIME);
   //handle movement
   if( keysDown[0] ) //w
      camera->moveForward(CAMERA_SPEED * (timeNow - timeLast));
   if( keysDown[1] ) //a
      camera->moveLeft(CAMERA_SPEED * (timeNow - timeLast));
   if( keysDown[2] ) //s
      camera->moveBackward(CAMERA_SPEED * (timeNow - timeLast));
   if( keysDown[3] ) //d
      camera->moveRight(CAMERA_SPEED * (timeNow - timeLast));

   //DRAW w/ mode switch
//   if( mode%NUM_MODES == FIXED_FUNCTION ) {
      useFixedFunction();
//   }
//   else {
//      useDeferred();
//   }

   //SCREEN TEXT
   glClear(GL_DEPTH_BUFFER_BIT);
   drawFPS();
   drawCamPos();

   /* ACTION */
   glutSwapBuffers();
   glutPostRedisplay();
   timeLast = timeNow;
}

void useFixedFunction() {
   drawScene();
}

//void useDeferred() {
//
//   //BUILD G-BUFFER
//   dfe->preDrawScene();
//   gatherShader->attach();
//   GLint loc = glGetUniformLocation(gatherShader->shaderProg, "far");
//   glUniform1f(loc, camera->zFar);
//   drawScene();
//   gatherShader->detach();
//   dfe->postDrawScene();
//
//   //RENDER SCENE USING G-BUFFER
//   if( mode%NUM_MODES == NORMALS ) {
//      doDirectTarget(normalbuffer);
//   }
//   else if( mode%NUM_MODES == POSITIONS ) {
//      doDirectTarget(positionbuffer);
//   }
//   else if( mode%NUM_MODES == COLORS ) {
//      doDirectTarget(colorbuffer);
//   }
//   else if( mode%NUM_MODES == DEFERRED ) {
//      doDirectionalAndAmbient();
//      //doPoint();
//   }
//}

//void doDirectionalAndAmbient() {
//   directionalLightShader->attach();
//   //Set shader uniforms
//   GLint loc = glGetUniformLocation(directionalLightShader->shaderProg, "normals"); //Get uniform sampler location
//   glActiveTexture(GL_TEXTURE0);               // Bind         to         unit
//   glBindTexture(GL_TEXTURE_2D, normalbuffer); //      normals    texture      0
//   glUniform1i(loc, 0); //Pass 0 to denote texture unit 0
//   loc = glGetUniformLocation(directionalLightShader->shaderProg, "positions"); //Get uniform sampler location
//   glActiveTexture(GL_TEXTURE1);                 // Bind           to         unit
//   glBindTexture(GL_TEXTURE_2D, positionbuffer); //      positions    texture      1
//   glUniform1i(loc, 1); //Pass 1 to denote texture unit 1
//   loc = glGetUniformLocation(directionalLightShader->shaderProg, "colors"); //Get uniform sampler location
//   glActiveTexture(GL_TEXTURE2);              // Bind        to         unit
//   glBindTexture(GL_TEXTURE_2D, colorbuffer); //      colors    texture      2
//   glUniform1i(loc, 2); //Pass 2 to denote texture unit 2
//   loc = glGetUniformLocation(directionalLightShader->shaderProg, "screenWidth");
//   glUniform1i(loc, curWidth);
//   loc = glGetUniformLocation(directionalLightShader->shaderProg, "screenHeight");
//   glUniform1i(loc, curHeight);
//   float lightDir[3] = {0.0, -1.0, 0.0};
//   loc = glGetUniformLocation(directionalLightShader->shaderProg, "lightDirection");
//   glUniform3fv(loc, 1, lightDir);
//   float lightColor[3] = {0.5, 0.5, 0.5};
//   loc = glGetUniformLocation(directionalLightShader->shaderProg, "diffuseLightColor");
//   glUniform3fv(loc, 1, lightColor);
//   float ambientColor[3] = {0.2, 0.2, 0.2};
//   loc = glGetUniformLocation(directionalLightShader->shaderProg, "ambientLightColor");
//   glUniform3fv(loc, 1, ambientColor);
//   // draw
//   dfe->drawFullScreenQuad(curWidth, curHeight);
//   //breakdown
//   directionalLightShader->detach();
//}

//void doPoint() {
//   pointLightShader->attach();
//   //Set shader uniforms
//   GLint loc = glGetUniformLocation(pointLightShader->shaderProg, "normals"); //Get uniform sampler location
//   glActiveTexture(GL_TEXTURE0);               // Bind         to         unit
//   glBindTexture(GL_TEXTURE_2D, normalbuffer); //      normals    texture      0
//   glUniform1i(loc, 0); //Pass 0 to denote texture unit 0
//   loc = glGetUniformLocation(pointLightShader->shaderProg, "positions"); //Get uniform sampler location
//   glActiveTexture(GL_TEXTURE1);                 // Bind           to         unit
//   glBindTexture(GL_TEXTURE_2D, positionbuffer); //      positions    texture      1
//   glUniform1i(loc, 1); //Pass 1 to denote texture unit 1
//   loc = glGetUniformLocation(pointLightShader->shaderProg, "colors"); //Get uniform sampler location
//   glActiveTexture(GL_TEXTURE2);              // Bind        to         unit
//   glBindTexture(GL_TEXTURE_2D, colorbuffer); //      colors    texture      2
//   glUniform1i(loc, 2); //Pass 2 to denote texture unit 2
//   loc = glGetUniformLocation(pointLightShader->shaderProg, "screenWidth");
//   glUniform1i(loc, curWidth);
//   loc = glGetUniformLocation(pointLightShader->shaderProg, "screenHeight");
//   glUniform1i(loc, curHeight);
//   loc = glGetUniformLocation(pointLightShader->shaderProg, "cameraPosition");
//   float cameraPos[3] = {camera->loc.x, camera->loc.y, camera->loc.z};
//   glUniform3fv(loc, 1, cameraPos);
//   loc = glGetUniformLocation(pointLightShader->shaderProg, "far");
//   glUniform1f(loc, camera->zFar);
//   loc = glGetUniformLocation(pointLightShader->shaderProg, "radius");
//   radius = 6.0;
//   glUniform1f(loc, radius);
//   float lightColor[3] = {0.5, 0.5, 0.5};
//   loc = glGetUniformLocation(pointLightShader->shaderProg, "diffuseLightColor");
//   glUniform3fv(loc, 1, lightColor);
//   // draw
//   drawPointLights();
//   //breakdown
//   pointLightShader->detach();
//}

//void drawPointLights() {
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   glMatrixMode(GL_MODELVIEW);
//   glLoadIdentity();
//   view();
//   glDisable(GL_LIGHTING);
//   glPushMatrix();
//   glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
//   glutSolidSphere(radius, 10, 10);
//   glPopMatrix();
//}

//void doDirectTarget(GLuint bufferToDraw) {
//   directTargetShader->attach();
//   //Set shader uniforms
//   GLint loc = glGetUniformLocation(directTargetShader->shaderProg, "normals"); //Get uniform sampler location
//   glActiveTexture(GL_TEXTURE0);               // Bind         to         unit
//   glBindTexture(GL_TEXTURE_2D, bufferToDraw); //      texture    texture      0
//   loc = glGetUniformLocation(directTargetShader->shaderProg, "screenWidth");
//   glUniform1i(loc, curWidth);
//   loc = glGetUniformLocation(directTargetShader->shaderProg, "screenHeight");
//   glUniform1i(loc, curHeight);
//   // draw
//   dfe->drawFullScreenQuad(curWidth, curHeight);
//   //breakdown
//   directTargetShader->detach();
//}

void drawFPS() {
   //calc fps
   frame++;
   if( timeNow - timebase > 1000 ) { //Only refresh every second
      fps = frame*1000.0/(timeNow-timebase);
      timebase = timeNow;
      frame = 0;
   }
   //write fps
   char fpsString[30];
   sprintf(fpsString, "fps: %g", fps);
   //draw fps
   glColor3f(1.0,0.0,0.0);
   drawText(85,95, GLUT_BITMAP_HELVETICA_12, fpsString);
}

void drawCamPos() {
   char posString[30];
   sprintf(posString, "<%.2f, %.2f, %.2f>", camera->loc.x, camera->loc.y, camera->loc.z);
   glColor3f(1.0,0.0,0.0);
   drawText(80,90, GLUT_BITMAP_HELVETICA_12, posString);
}

void drawScene() {
   //draw
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   view();
   lights();
   geometry();
}

void view() {
   /* CAMERA */
   gluLookAt(  camera->loc.x, camera->loc.y, camera->loc.z, // eye location
               camera->loc.x+camera->dir.x, camera->loc.y+camera->dir.y, camera->loc.z+camera->dir.z, // Look at point
               camera->up.x, camera->up.y, camera->up.z // up vector
            );
}

void lights() {
   /* LIGHTS */
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   //Point light position
   GLfloat lightPos[] = {0.0, 5.0, 0.0, 0.0};
   //Ambient composition
   GLfloat ambientComp[] = {0.0, 0.0, 0.0, 0.0};
   //Diffuse composition
   GLfloat diffuseComp[] = {0.5, 0.5, 0.5, 0.0};
   //Specular composition
   GLfloat specularComp[] = {0.0, 0.0, 0.0, 0.0};
   //Ambient light
   GLfloat globalAmbient[] = {0.2, 0.2, 0.2, 0.0};
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
   //Point light
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambientComp);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseComp);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specularComp);
}

void geometry() {
   /* DRAW GEOMETRY */
   //diffuse and ambient material properties
   GLfloat matColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
   //specular
   GLfloat matSpec[4] = {1.0f, 1.0f, 1.0f, 1.0f};
   GLfloat shininess = 128.0f;

   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matColor);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

   glPushMatrix();
   glScalef(10.0, 10.0, 10.0);
   bm[model]->draw();
   glPopMatrix();
}

void idle() {
   glutPostRedisplay();
}

void reshape(int w, int h) {
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

void keyboardDown(unsigned char key, int x, int y ) {
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

void keyboardUp(unsigned char key, int x, int y ) {
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
      case '1':
         model = 0;
         break;
      case '2':
         model = 1;
         break;
      case '3':
         model = 2;
         break;
   }
}

void motion(int x, int y) {
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
   glutPostRedisplay();
}

void passiveMotion(int x, int y) {
   mouseX = x;
   mouseY = y;
}

int main(int argc, char** argv) {

   width = curWidth = 800;
   height = curHeight = 600;
   elapsedTime = frame = timebase = timeNow = timeLast = 0;
   model = 0;

   // Create the camera
   camera = new FlyingCamera(45.0,       // fov
                             0, 0, 20,   // location
                             1.0, 300.0);// zNear, zFar

   //Load model
   bm[0] = new BasicModel("./assets/bunny500.m");
   bm[1] = new BasicModel("./assets/gargoyle_2k.m");
   bm[2] = new BasicModel("./assets/dragon10k.m");

   //OpenGL setup
   glutInit(&argc, argv);
   glutInitWindowSize(width, height);
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutCreateWindow("Deferred Shading");
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glClearColor(0.0, 0.0, 0.0, 1.0);

   //register callback functions
   glutDisplayFunc(display);
   glutIdleFunc(idle);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboardDown);
   glutKeyboardUpFunc(keyboardUp);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(passiveMotion);

   //Error check
   GLenum err = glewInit();
   if (err != GLEW_OK || !glewIsSupported("GL_VERSION_2_0"))
   {
      printf("OpenGL 2.0 not supported. No shaders!\n");
      printf("%s\n", glewGetErrorString(err));
      printf("%s\n", (char*)glGetString( GL_VERSION ) );
      exit(-1);
   }
   printf("OpenGL 2.0 supported. Using OpenGL %s \n\n",(char*)glGetString( GL_VERSION ));

//   //DEFERRED SHADING SETUP
//   //normalbuffer
//   glGenTextures(1, &normalbuffer);
//   glBindTexture(GL_TEXTURE_2D, normalbuffer);
//   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//   //position buffer
//   glGenTextures(1, &positionbuffer);
//   glBindTexture(GL_TEXTURE_2D, positionbuffer);
//   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//   //color buffer
//   glGenTextures(1, &colorbuffer);
//   glBindTexture(GL_TEXTURE_2D, colorbuffer);
//   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//   //shaders
//   printf("*** Gather Shader ***\n\n");
//   gatherShader = new Shader();
//   gatherShader->addVert("./shaders/deferred_gather.vert");
//   gatherShader->addFrag("./shaders/deferred_gather.frag");
//   gatherShader->build();
//   printf("*** Gather Shader DONE ***\n\n");
//   printf("*** Direct Target Shader ***\n\n");
//   directTargetShader = new Shader();
//   directTargetShader->addFrag("./shaders/deferred_direct_target.frag");
//   directTargetShader->build();
//   printf("*** Direct Target Shader DONE ***\n\n");
//   printf("*** Directional Light Shader ***\n\n");
//   directionalLightShader = new Shader();
//   directionalLightShader->addFrag("./shaders/dLight.frag");
//   directionalLightShader->build();
//   printf("*** Directional Light Shader DONE ***\n\n");
//   printf("*** Point Light Shader ***\n\n");
//   pointLightShader = new Shader();
//   pointLightShader->addVert("./shaders/pLight.vert");
//   pointLightShader->addFrag("./shaders/pLight.frag");
//   pointLightShader->build();
//   printf("*** Point Light Shader DONE ***\n\n");
//   //engine
//   dfe = new DeferredShadingEngine(width, height, normalbuffer, positionbuffer, colorbuffer);
//   dfe->init();

   //start the main loop
   glutMainLoop();
}

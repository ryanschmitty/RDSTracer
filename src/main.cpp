#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cstring>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include "POVRayParser.h"
#include "RDSScene.h"
#include "RDSSceneDesc.h"
#include "RDSImage.h"
#include "RDSTracer.h"
#include "RDSbvh.h"
#include "RandUtil.h"

//--- REAL-TIME STUFF --//

/* HEADERS */

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
RDST::SceneDescription* desc;
int model;
int frame, elapsedTime, timebase, timeNow, timeLast;
float fps;

/* FUNCTIONS */

void display();
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

   //DRAW
   drawScene();

   //SCREEN TEXT
   glClear(GL_DEPTH_BUFFER_BIT);
   drawFPS();
   drawCamPos();

   /* ACTION */
   glutSwapBuffers();
   glutPostRedisplay();
   timeLast = timeNow;
}

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

//   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matColor);
//   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
//   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

      glBegin(GL_TRIANGLES);
   const std::vector<RDST::GeomObjectPtr>& tris = desc->objs();
   for(std::vector<RDST::GeomObjectPtr>::const_iterator it = tris.begin(); it != tris.end(); ++it) {
       RDST::Triangle* tri = static_cast<RDST::Triangle*>(&(**it));
       matColor[0] = tri->getColor().x;
       matColor[1] = tri->getColor().y;
       matColor[2] = tri->getColor().z;
       glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matColor);
       glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
       glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
       glm::vec3 v0 = tri->getVertex0();
       glm::vec3 v1 = tri->getVertex1();
       glm::vec3 v2 = tri->getVertex2();
       glm::vec3 vCp1(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
       glm::vec3 vCp2(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
       glm::vec3 normal = glm::cross(vCp1, vCp2);
       normal = glm::normalize(normal);
       glNormal3f(normal.x, normal.y, normal.z);
       glVertex3f(v0.x, v0.y, v0.z);
       glNormal3f(normal.x, normal.y, normal.z);
       glVertex3f(v1.x, v1.y, v1.z);
       glNormal3f(normal.x, normal.y, normal.z);
       glVertex3f(v2.x, v2.y, v2.z);
   }
   glEnd();

//   glPushMatrix();
//   glScalef(10.0, 10.0, 10.0);
//   bm[model]->draw();
//   glPopMatrix();
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

//--- REAL-TIME STUFF ---//

//----------------------------------------
// Prototypes
//----------------------------------------

void printUsageAndExit(char*);
RDST::Options parseParameters(int, char**);

//----------------------------------------
// Implementation
//----------------------------------------

void printUsageAndExit(char* name)
{
   std::cerr.fill('.');
   std::cerr << "usage: " << name << " (-f <pov_input> | +I<pov_input>)\n";
   std::cerr << "optional options:\n";
   std::cerr << std::left << std::setw(47) << "  -w <width> | +W<width>" << "image width (default: 640)\n";
   std::cerr << std::left << std::setw(47) << "  -h <height> | +H<height>" << "image height (default: 480)\n";
   std::cerr << std::left << std::setw(47) << "  -aa <subsamples>" << "number of Anti-Alias subsamples (default: 1)\n";
   std::cerr << std::left << std::setw(47) << "  --disablejitter" << "disables jittered subsamples\n";
   std::cerr << std::left << std::setw(47) << "  -g <gamma>" << "gamma correction for specified gamma\n";
   std::cerr << std::left << std::setw(47) << "  -als <numAreaLightSamples>" << "number of samples per area light (default: 16)\n";
   std::cerr << std::left << std::setw(47) << "  --filter <box | gaussian <alpha> | mitchell>" << "downsample filter for Anti-Aliasing (default: box)\n";
   std::cerr << std::left << std::setw(47) << "  -t <threads>" << "number of threads to use (default: 8)\n";
   std::cerr << std::left << std::setw(47) << "  -mcs <samples>" << "number of monte carlo samples (default: 16)\n";
   std::cerr << std::left << std::setw(47) << "  -mcb <bounces>" << "number of monte carlo bounces (default: 0)\n";
   exit(EXIT_SUCCESS);
}

RDST::Options parseParameters(int argc, char** argv)
{
   //Check for proper number of parameters
   if (argc < 2)
      printUsageAndExit(argv[0]);
   //Process command line arguments
   RDST::Options opts;
   for (int i=1; i<argc; ++i) {
      if (strstr(argv[i], "+W")) opts.width = boost::lexical_cast<int>(&argv[i][2]);
      else if (!strcmp(argv[i], "-w")) opts.width = boost::lexical_cast<int>(argv[++i]);
      else if (strstr(argv[i], "+H")) opts.height = boost::lexical_cast<int>(&argv[i][2]);
      else if (!strcmp(argv[i], "-h")) opts.height = boost::lexical_cast<int>(argv[++i]);
      else if (!strcmp(argv[i], "-t")) opts.numThreads = boost::lexical_cast<int>(argv[++i]);
      else if (!strcmp(argv[i], "-mcs")) opts.monteCarloSamples = boost::lexical_cast<int>(argv[++i]);
      else if (!strcmp(argv[i], "-mcb")) opts.bounces = boost::lexical_cast<int>(argv[++i]);
      else if (!strcmp(argv[i], "-g")) {
         opts.enableGammaCorrection = true;
         opts.gamma = boost::lexical_cast<float>(argv[++i]);
      }
      else if (!strcmp(argv[i], "-aa")) {
         opts.enableAA = true;
         opts.subsamples = boost::lexical_cast<int>(argv[++i]);
         opts.jitter = true;
      }
      else if (!strcmp(argv[i], "--disablejitter")) {
         opts.jitter = false;
      }
      else if (!strcmp(argv[i], "-als")) {
         opts.areaLightSamples = boost::lexical_cast<int>(argv[++i]);
      }
      else if (!strcmp(argv[i], "--filter")) {
         std::string filterType = std::string(argv[++i]);
         std::transform(filterType.begin(), filterType.end(), filterType.begin(), ::tolower);
         if (filterType == "mitchell")
            opts.filter = RDST::Options::MITCHELL;
         else if (filterType == "gaussian") {
            opts.filter = RDST::Options::GAUSSIAN;
            opts.gaussianAlpha = boost::lexical_cast<float>(argv[++i]);
         }
         else
            opts.filter = RDST::Options::BOX;
      }
      else if (strstr(argv[i], "+I")) {
         opts.povRayFile = std::string(&argv[i][2]);
         opts.imgname = opts.povRayFile.substr(0,opts.povRayFile.find(".pov")); //erase .pov extension if it exists
      }
      else if (!strcmp(argv[i], "-f")) {
         opts.povRayFile = std::string(argv[++i]);
         opts.imgname = opts.povRayFile.substr(0,opts.povRayFile.find(".pov")); //erase .pov extension if it exists
      }
      else if (!strcmp(argv[i], "--help")) {
         printUsageAndExit(argv[0]);
      }
   }
   return opts;
}

int main(int argc, char** argv)
{
   boost::timer timer;
   double start = timer.elapsed();
   RDST::seed();

   RDST::Options opts = parseParameters(argc, argv);
   RDST::Image img(opts.width, opts.height, opts.enableGammaCorrection, opts.gamma);
   RDST::SceneDescription localdesc = RDST::POVRayParser::ParseFile(opts.povRayFile);
   desc = &localdesc;
   desc->setOpts(opts);
//   RDST::Tracer::RayTrace(desc, img);
//   img.writeToDisk(opts.imgname);

   double fsecs = timer.elapsed() - start;
   int secs = (int)fsecs;
   int millis = int((fsecs - secs) * 1000);
   int hours = secs/3600;
   secs -= 3600*hours;
   int mins = secs/60;
   secs -= 60*mins;
   std::cout << "\nRuntime: ";
   if (hours > 0)
      std::cout << hours << "h";
   if (mins > 0)
      std::cout << mins << "m";
   if (secs > 0)
      std::cout << secs << "s";
   std::cout << millis << "ms\n";


   //WRITE IT OUT! FOR BASICMODEL
   std::ofstream surfelFile;
   surfelFile.open("surfelcloud.m");
   const std::vector<RDST::GeomObjectPtr>& tris = desc->objs();
   int i = 1;
   printf("num objs: %d\n", tris.size());
   for(std::vector<RDST::GeomObjectPtr>::const_iterator it = tris.begin(); it != tris.end(); ++it) {
       glm::vec3 v0 = (static_cast<RDST::Triangle*>(&(**it)))->getVertex0();
       surfelFile << "Vertex " << i++ << "  " << v0.x << " " << v0.y << " " << v0.z << "\n";
       glm::vec3 v1 = (static_cast<RDST::Triangle*>(&(**it)))->getVertex1();
       surfelFile << "Vertex " << i++ << "  " << v1.x << " " << v1.y << " " << v1.z << "\n";
       glm::vec3 v2 = (static_cast<RDST::Triangle*>(&(**it)))->getVertex2();
       surfelFile << "Vertex " << i++ << "  " << v2.x << " " << v2.y << " " << v2.z << "\n";
   }
   int k = 0;
   for (int j=1; j<=(i-1)/3; ++j) {
       surfelFile << "Face " << j << "  " << (k+1) << " " << (k+2) << " " << (k+3) << "\n";
       k+=3;
   }
   surfelFile.close();

   //REAL-TIME STUFFS
   width = curWidth = 800;
   height = curHeight = 600;
   elapsedTime = frame = timebase = timeNow = timeLast = 0;
   model = 0;

   // Create the camera
   camera = new FlyingCamera(45.0,       // fov
                             0, 0, 20,   // location
                             1.0, 300.0);// zNear, zFar

   //Load model
   bm[0] = new BasicModel("./surfelcloud.m");
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

   //start the main loop
   glutMainLoop();
}

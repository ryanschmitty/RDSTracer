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
#include "realtime_surfels.h"

#include <glew.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

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
   std::cerr << std::left << std::setw(47) << "  --realtime" << "Render surfel cloud in realtime\n";
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
      else if (!strcmp(argv[i], "--realtime")) {
         opts.realtime = true;
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
   RDST::SceneDescription desc = RDST::POVRayParser::ParseFile(opts.povRayFile);
   desc.setOpts(opts);

//      int argc2 = 0;
//      char** argv2;
//      glutInit(&argc2, argv2);
//      glutInitWindowSize(8, 8); // 8x8 cube faces, not that this setting matters for the FBO
//      glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//      glutCreateWindow("Surfel Rasterizing");
//      glEnable(GL_DEPTH_TEST);
//      glEnable(GL_NORMALIZE);
//      glClearColor(0.0, 0.0, 0.0, 1.0);
//      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//      glEnable(GL_COLOR_MATERIAL);
//      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//      
//      //Error check
//      GLenum err = glewInit();
//      if (err != GLEW_OK || !glewIsSupported("GL_VERSION_2_0")) {
//         printf("OpenGL 2.0 not supported. No shaders!\n");
//         printf("%s\n", glewGetErrorString(err));
//         printf("%s\n", (char*)glGetString( GL_VERSION ) );
//         exit(-1);
//      }
//      printf("OpenGL 2.0 supported. Using OpenGL %s \n\n",(char*)glGetString( GL_VERSION ));

   if (!opts.realtime) {
      RDST::Tracer::RayTrace(desc, img);
      img.writeToDisk(opts.imgname);
   }
   
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

   if (opts.realtime)
      RDST::RealtimeSurfels::Render(desc);

   //WRITE IT OUT! FOR BASICMODEL
//   std::ofstream surfelFile;
//   surfelFile.open("surfelcloud.m");
//   const std::vector<RDST::GeomObjectPtr>& tris = desc->objs();
//   int i = 1;
//   for(std::vector<RDST::GeomObjectPtr>::const_iterator it = tris.begin(); it != tris.end(); ++it) {
//       glm::vec3 v0 = (static_cast<RDST::Triangle*>(&(**it)))->getVertex0();
//       surfelFile << "Vertex " << i++ << "  " << v0.x << " " << v0.y << " " << v0.z << "\n";
//       glm::vec3 v1 = (static_cast<RDST::Triangle*>(&(**it)))->getVertex1();
//       surfelFile << "Vertex " << i++ << "  " << v1.x << " " << v1.y << " " << v1.z << "\n";
//       glm::vec3 v2 = (static_cast<RDST::Triangle*>(&(**it)))->getVertex2();
//       surfelFile << "Vertex " << i++ << "  " << v2.x << " " << v2.y << " " << v2.z << "\n";
//   }
//   int k = 0;
//   for (int j=1; j<=(i-1)/3; ++j) {
//       surfelFile << "Face " << j << "  " << (k+1) << " " << (k+2) << " " << (k+3) << "\n";
//       k+=3;
//   }
//   surfelFile.close();
}

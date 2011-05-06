#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <boost/lexical_cast.hpp>
#include "POVRayParser.h"
#include "RDSScene.h"
#include "RDSImage.h"
#include "RDSTracer.h"

std::string filename, imgname;
short w, h; //img width and height


void printUsageAndExit(char* name)
{
   std::cerr << "Usage: " << name << " +W<width> +H<height> +I<pov_input>" << std::endl;
   exit(EXIT_FAILURE);
}

void parseParameters(int argc, char** argv)
{
   //Check for proper number of parameters
   if (argc != 4)
      printUsageAndExit(argv[0]);

   //Parse Parameters
   for (int i=1; i < argc; i++) {
      if (argv[i][0] != '+')
         printUsageAndExit(argv[0]);
      switch (argv[i][1]) {
         case 'W':
            w = boost::lexical_cast<short>(&argv[i][2]);
            if (w <= 0) {
               std::cerr << "***Error: width not a positive integer: " << w << std::endl;
               exit(EXIT_FAILURE);
            }
            break;
         case 'H':
            h = boost::lexical_cast<short>(&argv[i][2]);
            if (h <= 0) {
               std::cerr << "***Error: height not a positive integer: " << h << std::endl;
               exit(EXIT_FAILURE);
            }
            break;
         case 'I':
            //POVRay file
            filename = std::string(&argv[i][2]);
            //Image file
            imgname = filename.substr(0,filename.find(".pov"));//erase .pov extension if it exists
            if (imgname.empty()) {
               std::cerr << "***Error: empty file name." << std::endl;
               exit(EXIT_FAILURE);
            }
            break;
         default:
            printUsageAndExit(argv[0]);
            break;
      }
   }
}

int main(int argc, char** argv)
{
   //clock_t start = clock();

   parseParameters(argc, argv);
   RDST::Image img(w*2, h*2);
   RDST::SceneDescription desc(RDST::POVRayParser::ParseFile(filename));
   RDST::Tracer::RayTrace(desc, img);
   img.downSample(2,2).writeToDisk(imgname);

   //std::cout << "\nRuntime: " << float(clock() - start) / CLOCKS_PER_SEC << " seconds\n";
}

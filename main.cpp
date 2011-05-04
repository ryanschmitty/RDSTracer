#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <boost/lexical_cast.hpp>
#include "POVRayParser.h"
#include "RDSScene.h"
#include "RDSImage.h"
#include "RDSTracer.h"
#include "RDSbvh.h"

struct Options
{
   Options()
   : povRayFile(""), imgname(""), height(480), width(640), enableAA(false), sqrtSS(1)
   {}
   std::string povRayFile, imgname;
   int height, width;
   bool enableAA;
   int sqrtSS; //square root of the sub-samples
};

void printUsageAndExit(char* name)
{
   std::cerr << "usage: " << name << " [-w <width> | +W<width>] [-h <height> | +H<height>] [-aa <#sub-samples>] (-f <pov_input> | +I<pov_input>)\n";
   exit(EXIT_SUCCESS);
}

Options parseParameters(int argc, char** argv)
{
   //Check for proper number of parameters
   if (argc < 2 || argc > 9)
      printUsageAndExit(argv[0]);
   //Process command line arguments
   Options opts;
   for (int i=1; i<argc; ++i) {
      if (!strcmp(argv[i], "+W")) opts.width = boost::lexical_cast<int>(&argv[i][2]);
      else if (!strcmp(argv[i], "-w")) opts.width = boost::lexical_cast<int>(argv[++i]);
      else if (!strcmp(argv[i], "+H")) opts.height = boost::lexical_cast<int>(&argv[i][2]);
      else if (!strcmp(argv[i], "-h")) opts.height = boost::lexical_cast<int>(argv[++i]);
      else if (!strcmp(argv[i], "-aa")) {
         int subSamples = boost::lexical_cast<int>(argv[++i]);
         float f_sqrt = sqrtf((float)subSamples);
         int i_sqrt = (int)f_sqrt;
         if (f_sqrt != i_sqrt) {
            std::cerr << "Anti-Aliasing sub-samples must be a perfect square.\n";
            exit(EXIT_FAILURE);
         }
         opts.enableAA = true;
         opts.sqrtSS = i_sqrt;
      }
      else if (!strcmp(argv[i], "+I")) {
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
   clock_t start = clock();

   Options opts = parseParameters(argc, argv);
   //Create big buffer
   RDST::Image img(opts.width*opts.sqrtSS, opts.height*opts.sqrtSS);
   RDST::SceneDescription desc(RDST::POVRayParser::ParseFile(opts.povRayFile));
   RDST::Tracer::RayTrace(desc, img, opts.enableAA);
   //Anti-Alias by downsampling the big buffer
   if (opts.enableAA)
      img.downSample(opts.sqrtSS,opts.sqrtSS).writeToDisk(opts.imgname);
   else
      img.writeToDisk(opts.imgname);

   std::cout << "\nRuntime: " << float(clock() - start) / CLOCKS_PER_SEC << " seconds\n";
}
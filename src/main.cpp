#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <boost/lexical_cast.hpp>
#include "POVRayParser.h"
#include "RDSScene.h"
#include "RDSSceneDesc.h"
#include "RDSImage.h"
#include "RDSTracer.h"
#include "RDSbvh.h"
#include "RandUtil.h"

void printUsageAndExit(char* name)
{
   std::cerr << "usage: " << name << " [-w <width> | +W<width>] [-h <height> | +H<height>] [-aa <#sub-samples>] [--disablejitter] [-g <gamma>] [-als numAreaLightSamples](-f <pov_input> | +I<pov_input>)\n";
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
   clock_t start = clock();
   RDST::seed();

   RDST::Options opts = parseParameters(argc, argv);
   RDST::Image img(opts.width, opts.height, opts.enableGammaCorrection, opts.gamma);
   RDST::SceneDescription desc = RDST::POVRayParser::ParseFile(opts.povRayFile);
   desc.setOpts(opts);
   RDST::Tracer::RayTrace(desc, img);
   img.writeToDisk(opts.imgname);

   std::cout << "\nRuntime: " << float(clock() - start) / CLOCKS_PER_SEC << " seconds\n";
}

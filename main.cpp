#include <cstdio>
#include <cstdlib>
#include <string>
#include "POVRayParser.h"
#include "RDSScene.h"

std::string parseParameters(int argc, char** argv)
{
   if (argc != 2) {
      fprintf(stderr,"Usage: %s <input_file>\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   return std::string(argv[1]);
}

int main(int argc, char** argv)
{
   std::string file(parseParameters(argc, argv));
   RDST::POVRayParser::ParseFile(file);
}
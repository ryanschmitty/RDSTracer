#ifndef __MODEL_BASIS_H__
#define __MODEL_BASIS_H__

#include <GLUT/glut.h>
#include "vec3.h"

class ModelBasis
{
   protected:
      // inner class to store triangle faces
      // note that v1, v2, and v3 are indices into the vertex array
      class Tri {
         public:
            int v1;
            int v2;
            int v3;
            Vec3f normal;
            Vec3f color;

            // constructors
            Tri(int in_v1, int in_v2, int in_v3) :
               v1(in_v1), v2(in_v2), v3(in_v3), normal(0, 1, 0) {}

            Tri() : normal(0, 1, 0) {}
      };

      float max_x;
      float max_y;
      float max_z;
      float min_x;
      float min_y;
      float min_z;
      float max_extent;
      Vec3f center;

      // Parses the Vertex from the line in the following format
      //    "Vertex <(ignored)> <x> <y> <z>"
      // returns *new* Vec3f (you delete)
      Vec3f* parseVertex(const std::string line) const
      {
         int temp = -1;
         float x, y, z;
         if(sscanf(line.c_str(), "Vertex %d %g %g %g", &temp, &x, &y, &z) != 4) {
            printf("Error reading Vertex %d\n", temp);
            //FIXME: maybe exit(-1)?
         }
         Vec3f *v = new Vec3f(x,y,z);
         return v;
      }

      // Parses the Face from the line in the following format
      //    "Face <(ignored)> <v1> <v2> <v3> {rgb=(<r>,<g>,<b>)}"
      // Note that the rgb component is optional
      // returns *new* Tri (you delete)
      Tri* parseFace(const std::string line) const
      {
         int temp = -1, v1, v2, v3;
         if(sscanf(line.c_str(), "Face %d %d %d %d", &temp, &v1, &v2, &v3) != 4) {
            printf("Error reading Face %d\n", temp);
            //FIXME: maybe exit(-1)?
         }
         Tri *t = new Tri(v1-1,v2-1,v3-1); //Offset cuz vertices in file start at 1
         size_t start;
         // Check for color
         if((start = line.find("{")) != std::string::npos) {
            start = line.find("rgb=(");
            std::string colorLine = line.substr(start);
            if(sscanf(colorLine.c_str(), " rgb = ( %g , %g , %g) } ", &(t->color.x), &(t->color.y), &(t->color.z)) != 3) {
               printf("Error reading Color\n");
               //FIXME: maybe exit(-1)?
            }
         }
         else {
            t->color = Vec3f(1.0,0.0,0.0);
         }
         return t;
      }

      // returns the first float found in the string
      float parseFloat(const std::string line) const
      {
         float f;
         if(sscanf(line.c_str(), "%g", &f) != 1) {
            printf("Error reading float\n");
            //FIXME: maybe exit(-1)?
         }
         return f;
      }
};

#endif

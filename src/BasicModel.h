#ifndef BASIC_MODEL_H
#define BASIC_MODEL_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <float.h>
#include "ModelBasis.h"

using namespace std;

class BasicModel : virtual public ModelBasis
{
   public:
      BasicModel(std::string filename);
      ~BasicModel();

      void draw();
      // return the model width(x-axis) [0.0, 1.0]
      float getWidth() const;
      // return the model height(y-axis) [0.0, 1.0]
      float getHeight() const;
      // return the model depth(z-axis) [0.0, 1.0]
      float getDepth() const;

   protected:
      // opengl display list ID
      GLuint id;
      //stl vector to store all the triangles in the mesh
      std::vector<Tri *> Triangles;
      //stl vector to store all the vertices in the mesh
      std::vector<Vec3f *> Vertices;
      //stl vector to store all the vertices normals in the mesh
      std::vector<Vec3f *> VerticesNormals;

      void parseFile(std::string filename);
      Tri* parseTri(std::string line);
      void drawTri(Tri* t);
};

#endif

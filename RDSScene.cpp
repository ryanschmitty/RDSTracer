/**
* RDSScene.cpp
* --------------------
* Implementation for a collection of
* useful objects for representing a scene.
* Author: Ryan Schmitt
*/

#include "RDSScene.h"

namespace RDST
{
   //Definitions for pure virtual destructors
   SceneObject::~SceneObject()
   {}
   Colored::~Colored()
   {}
   GeomObject::~GeomObject()
   {}

   //Adjoint function
   glm::mat3
   GeomObject::adjoint(const glm::mat3& m) const
   {
      float d00 = (m[1][1]*m[2][2]) - (m[2][1]*m[1][2]);
      float d01 = (m[0][1]*m[2][2]) - (m[2][1]*m[0][2]);
      float d02 = (m[0][1]*m[1][2]) - (m[1][1]*m[0][2]);
      float d10 = (m[1][0]*m[2][2]) - (m[2][0]*m[1][2]);
      float d11 = (m[0][0]*m[2][2]) - (m[2][0]*m[0][2]);
      float d12 = (m[0][0]*m[1][2]) - (m[1][0]*m[0][2]);
      float d20 = (m[1][0]*m[2][1]) - (m[2][0]*m[1][1]);
      float d21 = (m[0][0]*m[2][1]) - (m[2][0]*m[0][1]);
      float d22 = (m[0][0]*m[1][1]) - (m[1][0]*m[0][1]);
      return glm::mat3 (d00, -d01, d02, -d10, d11, -d12, d20, -d21, d22);
   }
}

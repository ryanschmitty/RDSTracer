/**
 * RDSSamplers.h
 * --------------------
 * A collection of useful sampling functions.
 * Author: Ryan Schmitt
 */

#ifndef __RDS_SAMPLERS_H__
#define __RDS_SAMPLERS_H__

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <cmath>
#include "RDSScene.h" //for PI... maybe I should make a separate header for those things.

namespace RDST
{
   class Samplers
   {
   private:
      /* Hidden ctors */
      explicit Samplers()
      {}
      explicit Samplers(const Samplers& rhs)
      {}
      ~Samplers()
      {}
   public:
      /**
       * Cosine weighted hemisphere sampling (i.e. more samples near the top).
       * from PBR v.2
       */
      static glm::vec3 CosineHemisphereSample(float u1, float u2) {
         glm::vec3 ret;
         float r, theta;
         float sx = 2.f*u1 - 1.f;
         float sy = 2.f*u2 - 1.f;
         
         //Map square samples to disk (Shirley is a genius)
         if (sx == 0.f && sy == 0.f) {
            ret.x = 0.f;
            ret.y = 0.f;
         }
         else {
            if (sx >= -sy) {
               if (sx > sy) {
                  //Handle first region
                  r = sx;
                  if (sy > 0.f) theta = sy/r;
                  else          theta = 8.f + sy/r;
               }
               else {
                  //Handle second region
                  r = sy;
                  theta = 2.f - sx/r;
               }
            }
            else {
               if (sx <= sy) {
                  //Handle third region
                  r = -sx;
                  theta = 4.f - sy/r;
               }
               else {
                  //Handle fourth region
                  r = -sy;
                  theta = 6.f + sx/r;
               }
            }
            theta *= PI / 4.f;
            ret.x = r * cosf(theta);
            ret.y = r * sinf(theta);
         }
         ret.z = sqrtf(glm::max(0.f, 1.f - ret.x*ret.x - ret.y*ret.y));
         return ret;
      }
   };
}

#endif
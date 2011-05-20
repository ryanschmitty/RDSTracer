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
         float sz = 2.f*u2 - 1.f;
         
         //Map square samples to disk (Shirley is a genius)
         if (sx == 0.f && sz == 0.f) {
            ret.x = 0.f;
            ret.z = 0.f;
         }
         else {
            if (sx >= -sz) {
               if (sx > sz) {
                  //Handle first region
                  r = sx;
                  if (sz > 0.f) theta = sz/r;
                  else          theta = 8.f + sz/r;
               }
               else {
                  //Handle second region
                  r = sz;
                  theta = 2.f - sx/r;
               }
            }
            else {
               if (sx <= sz) {
                  //Handle third region
                  r = -sx;
                  theta = 4.f - sz/r;
               }
               else {
                  //Handle fourth region
                  r = -sz;
                  theta = 6.f + sx/r;
               }
            }
            theta *= PI / 4.f;
            ret.x = r * cosf(theta);
            ret.z = r * sinf(theta);
         }
         ret.y = sqrtf(glm::max(0.f, 1.f - ret.x*ret.x - ret.z*ret.z));
         return ret;
      }
      static glm::vec3 BadCosineHemisphereSample(float u1, float u2)
      {
          float r = sqrtf(u1);
          float theta = 2 * PI * u2;
       
          float x = r * cosf(theta);
          float z = r * cosf(theta);
       
          return glm::vec3(x, sqrtf(1.f - u1), z);
      }
      /**
       * Uniform weighted hemisphere sampling (i.e. more samples near the top).
       * from PBR v.2
       */
      static glm::vec3 UniformHemisphereSample(float u1, float u2) {
         float r = sqrtf(1.0f - u1 * u1);
         float phi = 2 * PI * u2;

         return glm::vec3(cosf(phi) * r, u1, sinf(phi) * r);
      }
   };
}

#endif
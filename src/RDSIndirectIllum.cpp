/**
* File: RDSIndirectIllum.cpp
* --------------------
* Implementation for Indirect Illumination functions.
* Author: Ryan Schmitt
*/

#include "RDSIndirectIllum.h"

namespace RDST
{
   glm::vec3 IndirectIllumMonteCarlo(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
   {
      glm::vec3 indirectColor(0.f);
      if (recursionsLeft > 0) {
         int sqsamps = (int)sqrtf((float)scene.opts().monteCarloSamples);
         //Construct basis for uniform sample -> intersection point transform
         glm::vec3 k = glm::vec3(0,1,0);
         glm::vec3 tan = fabs(intrs.n.y) == 1.f ? glm::vec3(1,0,0) : glm::normalize(k - ((glm::dot(k, intrs.n))*intrs.n)); //Gram-Schmidt Process
         glm::vec3 bin = glm::cross(intrs.n, tan);
         glm::mat3 xform = glm::mat3(tan, intrs.n, bin);
         //For each sample
         for (int i=0; i<sqsamps; ++i) {
            for (int j=0; j<sqsamps; ++j) {
               float u1 = (float)i/sqsamps + unifRand()/sqsamps;
               float u2 = (float)j/sqsamps + unifRand()/sqsamps;
               //glm::vec3 unitSamp = Samplers::CosineHemisphereSample(u1,u2);
               //glm::vec3 unitSamp = Samplers::BadCosineHemisphereSample(u1,u2);
               glm::vec3 unitSamp = Samplers::UniformHemisphereSample(u1,u2);
               glm::vec3 sampleDir = glm::normalize(xform * unitSamp);
               Ray ray = Ray(sampleDir, intrs.p+RAY_EPSILON*sampleDir);
               Intersection* pIsect = Tracer::RaySceneIntersect(ray, scene);
               if (pIsect->hit) {
                  indirectColor += Tracer::ShadePoint(*pIsect, scene, recursionsLeft-1) * glm::vec3(intrs.surf.color) * glm::dot(intrs.n, ray.d);
               }
               delete pIsect;
            }
         }
         indirectColor /= (float)scene.opts().monteCarloSamples;
      }
      return indirectColor;
   }
}

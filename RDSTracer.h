
/**
 * File: RDSTracer.h
 * --------------------
 * The actual Ray Tracer!
 * Author: Ryan Schmitt
 */

#ifndef _RDS_TRACER_H_
#define _RDS_TRACER_H_

#define GLM_FORCE_INLINE

#include <vector>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include "RDSImage.h"
#include "RDSScene.h"
#include "POVRayParser.h"

namespace RDST
{
   /**
    * The actual ray tracing code!
    */
   class Tracer
   {
   private:
      /* Hidden ctors */
      explicit Tracer()
      {}
      explicit Tracer(const Tracer& rhs)
      {}
      ~Tracer()
      {}
   public:
      /* Ray tracing functions */
      static void RayTrace(const SceneDescription& scene, Image& image);
   private:
      /* Helper Functions */
      static std::vector<RayPtr> GenerateRays(const Camera& cam, const Image& image);
      static Intersection*       RayObjectsIntersect(Ray& ray, const std::vector<GeomObjectPtr>& objs);
      static glm::vec3           ShadePoint(const Intersection& intrs, const SceneDescription& scene, unsigned int numReflectionBounces);
      static glm::vec3           CalcReflection(Ray& reflectionRay, const SceneDescription& scene, int numReflections);
   };
}

#endif

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
      static void                ShadePixel(Pixel& p, const SceneDescription& scene, const Intersection& intrs);
      static glm::vec3           CalcReflection(Ray& reflectionRay, const std::vector<GeomObjectPtr>& objs, const Surface& currentSurface, glm::vec3& reflectionColor, int numReflections);
   };
}

#endif
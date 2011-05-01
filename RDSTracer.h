
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
      static glm::vec3           CalcDirectIllum(const Intersection& intrs, const SceneDescription& scene);
      static glm::vec3           CalcReflection(const Intersection& intrs, const SceneDescription& scene, unsigned int numReflections);
      static glm::vec3           refract(const glm::vec3& normal, const glm::vec3& incident, float n1, float n2);
      static glm::vec3           CalcRefraction(const Intersection& intrs, const SceneDescription& scene);
      static glm::vec3           ShadePoint(const Intersection& intrs, const SceneDescription& scene, unsigned int numReflections);
   };
}

#endif

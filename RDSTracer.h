
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
      static void RayTrace(const SceneDescription& scene, Image& image, bool antialias=false, int subsamples=1);
   private:
      /* Helper Functions */
      static RayPtrListPtr GenerateRays(const Camera& cam, int raysInX, int raysInY, bool jitter);
      static glm::vec3     TraceRay(Ray& ray, const SceneDescription& scene, unsigned int recursionsLeft);
      static Intersection* RayObjectsIntersect(Ray& ray, const std::vector<GeomObjectPtr>& objs);
      static glm::vec3     ShadePoint(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft);
      static glm::vec3     CalcDirectIllum(const Intersection& intrs, const SceneDescription& scene);
      static glm::vec3     CalcReflection(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft);
      static glm::vec3     CalcRefraction(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft);
      static int           VerifyNumSubsamples(int subsamples);
   };
}

#endif


/**
 * File: RDSTracer.h
 * --------------------
 * The actual Ray Tracer!
 * Author: Ryan Schmitt
 */

#ifndef _RDS_TRACER_H_
#define _RDS_TRACER_H_

#include <vector>
#include <glm/glm.hpp>
#include "RDSImage.h"
#include "RDSScene.h"
#include "POVRayParser.h"

namespace RDST
{
   /**
    * Intersection information.
    */
   class Intersection
   {
   public:
      explicit Intersection()
      : hit(false),
        p(glm::vec3(0.f))
      {}
      explicit Intersection(bool hit, const glm::vec3& hitPoint)
      : hit(hit),
        p(hitPoint)
      {}
      bool hit;
      glm::vec3 p;
   };

   /**
    * A ray with origin o and direction d.
    */
   class Ray
   {
   public:
      explicit Ray()
      : o(glm::vec3(0.f)),
        d(glm::vec3(0.f))
      {}
      explicit Ray(const glm::vec3& origin, const glm::vec3& direction)
      : o(origin),
        d(direction)
      {}
      glm::vec3 o, d;
   };

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
      static Intersection RaySphereIntersect(const Ray& ray, SpherePtr pSphere);
      static Intersection RayPlaneIntersect(const Ray& ray, PlanePtr pPlane);
   };
}

#endif
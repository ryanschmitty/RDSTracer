
/**
 * File: RDSTracer.h
 * --------------------
 * The actual Ray Tracer!
 * Author: Ryan Schmitt
 */

#ifndef _RDS_TRACER_H_
#define _RDS_TRACER_H_

#include <vector>
#include <boost/shared_ptr.hpp>
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
        t(FLT_MAX),
        p(glm::vec3(0.f))
      {}
      explicit Intersection(bool hit, float hitT, const glm::vec3& hitPoint)
      : hit(hit),
        t(hitT),
        p(hitPoint)
      {}
      bool hit;
      float t;
      glm::vec3 p;
   };

   /**
    * A ray with origin o and direction d.
    */
   class Ray
   {
   public:
      explicit Ray(const glm::vec3& direction,
                   const glm::vec3& origin = glm::vec3(0.f))
      : d(direction),
        o(origin),
        tCur(tMax)
      {}
      glm::vec3 d, o;
      float tCur;
      static float tMin; // = 0.f;
      static float tMax; // = FLT_MAX;
   };
   typedef boost::shared_ptr<Ray> RayPtr;

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
      static void ShadePixel(Pixel& p, const Camera& cam, const std::vector<PointLightPtr>& lights, const GeomObject& obj, const Intersection& intrs);
      static std::vector<RayPtr> GenerateRays(const Camera& cam, const Image& image);
      static Ray TransformRay(const Ray& ray, const glm::mat4& worldToObj);
      static Intersection RaySphereIntersect(const Ray& ray, const Sphere& sphere);
      static Intersection RayPlaneIntersect(const Ray& ray, const Plane& plane);
   };
}

#endif
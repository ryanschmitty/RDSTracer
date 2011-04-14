
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
    * Surface storage helper class
    */
   class Surface
   {
   public:
      explicit Surface(const glm::vec4& _color = glm::vec4(1.f), const Finish& _finish = Finish())
      : color(_color),
        finish(_finish)
      {}

      glm::vec4 color;
      Finish finish;
   };

   /**
    * Intersection information.
    */
   class Intersection
   {
   public:
      explicit Intersection()
      : hit(false),
        t(FLT_MAX),
        p(glm::vec3(0.f)),
        n(glm::vec3(0.f, 1.f, 0.f)),
        surf(Surface())
      {}
      explicit Intersection(bool hit, float hitT, const glm::vec3& hitPoint, const glm::vec3& normal, const Surface& surface)
      : hit(hit),
        t(hitT),
        p(hitPoint),
        n(normal),
        surf(surface)
      {}
      bool hit;
      float t;
      glm::vec3 p;
      glm::vec3 n;
      Surface surf;
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
      static std::vector<RayPtr> GenerateRays(const Camera& cam, const Image& image);
      static void                ShadePixel(Pixel& p, const SceneDescription& scene, const Intersection& intrs);
      static Intersection        RayObjectsIntersect(Ray& ray, const std::vector<GeomObjectPtr>& objs);
      static Ray                 TransformRay(const Ray& ray, const glm::mat4& worldToObj);
      static Intersection        RaySphereIntersect(const Ray& ray, const Sphere& sphere);
      static Intersection        RayPlaneIntersect(const Ray& ray, const Plane& plane);
   };
}

#endif
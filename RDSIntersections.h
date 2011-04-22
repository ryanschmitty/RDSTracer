/**
 * RDSIntersections.h
 * --------------------
 * A handful of intersection tests.
 * Author: Ryan Schmitt
 */

#ifndef __RDS_INTERSECTIONS_H__
#define __RDS_INTERSECTIONS_H__

#define GLM_FORCE_INLINE

#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>

namespace RDST
{
   //---------------------------------------------------------------------------
   // Required Forward Declarations
   //---------------------------------------------------------------------------
   class Ray;
   class Sphere;
   class Plane;
   class Triangle;
   class Finish;

   //---------------------------------------------------------------------------
   // HELPER CLASSES
   //---------------------------------------------------------------------------

   /**
    * Surface storage class
    */
   class Surface
   {
   public:
      explicit Surface(const glm::vec4& _color, boost::shared_ptr<Finish> _pFinish)
      : color(_color),
        pFinish(_pFinish)
      {}
      glm::vec4 color;
      boost::shared_ptr<Finish> pFinish;
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
   typedef boost::shared_ptr<Intersection> IntersectionPtr;
   typedef boost::shared_ptr<const Intersection> ConstIntersectionPtr;

   //---------------------------------------------------------------------------
   // INTERSECTION TEST FUNCTIONS.
   //---------------------------------------------------------------------------

   /**
    * Helper for transforming Rays.
    */
   static
   const boost::shared_ptr<const Ray>
   TransformRay(const boost::shared_ptr<const Ray> pRay, const glm::mat4& worldToObj)
   {
      glm::vec3 o = glm::vec3(worldToObj*glm::vec4(pRay->o,1.f));
      glm::vec3 dir = glm::vec3(worldToObj*glm::vec4(pRay->d,0.f));
      return boost::shared_ptr<const Ray>(new Ray(glm::normalize(dir), o));
   }

   /**
    * Sphere Intersection.
    */
   static
   const ConstIntersectionPtr
   RaySphereIntersect(const boost::shared_ptr<const Ray> pRay, const Sphere* const pSphere )
   {
      //Setup transformed ray
      boost::shared_ptr<const Ray> pxr = TransformRay(pRay, pSphere->getModelInverse());
      //Intersection Code
      glm::vec3 l = pSphere->getCenter() - pxr->o;
      float s = glm::dot(l, pxr->d);
      float ll = glm::dot(l, l);
      float rr = pSphere->getRadiusSquared();
      if (s < 0.f && ll > rr) return ConstIntersectionPtr(new Intersection()); //sphere is behind us and we're not inside
      float mm = ll-(s*s); //dist from sphere center projected onto ray to sphere center
      if (mm > rr) return ConstIntersectionPtr(new Intersection()); //ray misses (sphere center projected onto ray - sphere center > radius)
      float q = sqrtf(rr-mm);
      float t = 0.f;
      if (ll > rr) t = s-q; //we're outside the sphere so return first point
      else t = s+q;
      glm::vec3 n = pSphere->getNormalXform() * glm::normalize((pxr->o+(pxr->d*t))-pSphere->getCenter()); //make sure to normalize n after this.
      return ConstIntersectionPtr(new Intersection(true, t, pRay->o + (pRay->d*t), glm::normalize(n), Surface(pSphere->getColor(), pSphere->getFinish())));
   }

   /**
    * Plane Intersection.
    */
   static
   const ConstIntersectionPtr
   RayPlaneIntersect(const boost::shared_ptr<const Ray> pRay, const Plane* const pPlane)
   {
      //Setup transformed Ray
      boost::shared_ptr<const Ray> pxr = TransformRay(pRay, pPlane->getModelInverse());
      //Intersection code
      glm::vec3 n = pPlane->getNormal();
      float denom = glm::dot(pxr->d, n);
      if (denom == 0.f) return ConstIntersectionPtr(new Intersection()); //ray is parallel
      float t = -(glm::dot(n, pxr->o) - pPlane->getDistance()) / denom;
      if (t < 0.f) return ConstIntersectionPtr(new Intersection()); //ray intersected behind us
      if (denom > 0.f) n = -n; //flip normal if we're under the plane
      return ConstIntersectionPtr(new Intersection(true, t, pRay->o + (pRay->d*t), n, Surface(pPlane->getColor(), pPlane->getFinish())));
   }

   /**
    * Triangle Intersection.
    */
   static
   const ConstIntersectionPtr
   RayTriangleIntersect(const boost::shared_ptr<const Ray> pRay, const Triangle* const pTri)
   {
      //Setup transformed Ray
      boost::shared_ptr<const Ray> pxr = TransformRay(pRay, pTri->getModelInverse());
      //Intersection code (Essential Mathematics for Games & Interactive Applications)
      glm::vec3 e1 = pTri->getVertex1() - pTri->getVertex0();
      glm::vec3 e2 = pTri->getVertex2() - pTri->getVertex0();
      glm::vec3 p = glm::cross(pxr->d, e2);
      float a = glm::dot(e1, p);
      //if result zero, no intersection or infinite intersections
      //(ray parallel to triangle plane)
      if (a == 0.f) return ConstIntersectionPtr(new Intersection());
      //compute denominator
      float f = 1.f/a;
      //compute barycentric coordinates
      glm::vec3 s = pxr->o - pTri->getVertex0();
      float u = f*glm::dot(s, p);
      if (u < 0.f || u > 1.f) return ConstIntersectionPtr(new Intersection());
      glm::vec3 q = glm::cross(s, e1);
      float v = f*glm::dot(xr.d, q);
      if (v < 0.f || u+v > 1.f) return ConstIntersectionPtr(new Intersection());
      //compute line parameter
      float t = f*glm::dot(e2, q);
      return ConstIntersectionPtr(new Intersection(true, t, pRay->o+(pRay->d*t), pTri->getNormal(), Surface(pTri->getColor(), pTri->getFinish())));
   }
}

#endif
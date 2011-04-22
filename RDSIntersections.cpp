/**
 * RDSIntersections.cpp
 * --------------------
 * Intersections!
 * Author: Ryan Schmitt
 */

#define GLM_FORCE_INLINE

#include <glm/glm.hpp>
#include "RDSIntersections.h"
#include "RDSScene.h"

namespace RDST
{
   Ray TransformRay(const Ray& ray, const glm::mat4& worldToModel)
   {
      glm::vec3 o = glm::vec3(worldToModel*glm::vec4(ray.o,1.f));
      glm::vec3 dir = glm::vec3(worldToModel*glm::vec4(ray.d,0.f));
      return Ray(glm::normalize(dir), o);
   }

   /**
    * Plane Intersection
    */
   Intersection* RayPlaneIntersect(const Ray& ray, const Plane& plane)
   {
      //Setup transformed Ray
      Ray xr = ray; //TransformRay(ray, plane.getModelInverse());
      //Intersection code
      glm::vec3 n = plane.getNormal();
      float denom = glm::dot(xr.d, n);
      if (denom == 0.f) return NULL; //ray is parallel
      float t = -(glm::dot(n, xr.o) - plane.getDistance()) / denom;
      if (t < 0.f) return NULL; //ray intersected behind us
      if (denom > 0.f) n = -n; //flip normal if we're under the plane
      return new Intersection(true, t, ray.o + (ray.d*t), n, Surface(plane.getColor(), plane.getFinish()));
   }

   /**
    * Sphere Intersection
    */
   Intersection* RaySphereIntersect(const Ray& ray, const Sphere& sphere)
   {
      //Setup transformed ray
      Ray xr = ray; //TransformRay(ray, sphere.getModelInverse());
      //Intersection Code
      glm::vec3 l = sphere.getCenter() - xr.o;
      float s = glm::dot(l, xr.d);
      float ll = glm::dot(l, l);
      float rr = sphere.getRadiusSquared();
      if (s < 0.f && ll > rr) return NULL; //sphere is behind us and we're not inside
      float mm = ll-(s*s); //dist from sphere center projected onto ray to sphere center
      if (mm > rr) return NULL; //ray misses (sphere center projected onto ray - sphere center > radius)
      float q = sqrtf(rr-mm);
      float t = 0.f;
      if (ll > rr) t = s-q; //we're outside the sphere so return first point
      else t = s+q;
      glm::vec3 n = sphere.getNormalXform() * glm::normalize((xr.o+(xr.d*t))-sphere.getCenter()); //make sure to normalize n after this.
      return new Intersection(true, t, ray.o + (ray.d*t), glm::normalize(n), Surface(sphere.getColor(), sphere.getFinish()));
   }

   /**
    * Triangle Intersection
    */
   Intersection* RayTriangleIntersect(const Ray& ray, const Triangle& tri)
   {
      //Setup transformed Ray
      Ray xr = ray; //TransformRay(ray, tri.getModelInverse());
      //Intersection code (Essential Mathematics for Games & Interactive Applications)
      glm::vec3 e1 = tri.getVertex1() - tri.getVertex0();
      glm::vec3 e2 = tri.getVertex2() - tri.getVertex0();
      glm::vec3 p = glm::cross(xr.d, e2);
      float a = glm::dot(e1, p);
      //if result zero, no intersection or infinite intersections
      //(ray parallel to triangle plane)
      if (a == 0.f) return NULL;
      //compute denominator
      float f = 1.f/a;
      //compute barycentric coordinates
      glm::vec3 s = xr.o - tri.getVertex0();
      float u = f*glm::dot(s, p);
      if (u < 0.f || u > 1.f) return NULL;
      glm::vec3 q = glm::cross(s, e1);
      float v = f*glm::dot(xr.d, q);
      if (v < 0.f || u+v > 1.f) return NULL;
      //compute line parameter
      float t = f*glm::dot(e2, q);
      return new Intersection(true, t, ray.o+(ray.d*t), tri.getNormal(), Surface(tri.getColor(), tri.getFinish()));
   }
}

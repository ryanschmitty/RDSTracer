/**
* RDSScene.cpp
* --------------------
* Implementation for a collection of
* useful objects for representing a scene.
* Author: Ryan Schmitt
*/

#include "RDSScene.h"

namespace RDST
{
   //Definitions for pure virtual destructors
   SceneObject::~SceneObject()
   {}
   Colored::~Colored()
   {}

   //Adjoint function
   glm::mat3
   GeomObject::adjoint(const glm::mat3& m) const
   {
      float d00 = (m[1][1]*m[2][2]) - (m[2][1]*m[1][2]);
      float d01 = (m[0][1]*m[2][2]) - (m[2][1]*m[0][2]);
      float d02 = (m[0][1]*m[1][2]) - (m[1][1]*m[0][2]);
      float d10 = (m[1][0]*m[2][2]) - (m[2][0]*m[1][2]);
      float d11 = (m[0][0]*m[2][2]) - (m[2][0]*m[0][2]);
      float d12 = (m[0][0]*m[1][2]) - (m[1][0]*m[0][2]);
      float d20 = (m[1][0]*m[2][1]) - (m[2][0]*m[1][1]);
      float d21 = (m[0][0]*m[2][1]) - (m[2][0]*m[0][1]);
      float d22 = (m[0][0]*m[1][1]) - (m[1][0]*m[0][1]);
      return glm::mat3 (d00, -d01, d02, -d10, d11, -d12, d20, -d21, d22);
   }

   //Helper to transform Ray into Object space
   Ray
   GeomObject::transformRay(const Ray& ray) const
   {
      glm::vec3 o = glm::vec3(getModelInverse()*glm::vec4(ray.o,1.f));
      glm::vec3 dir = glm::vec3(getModelInverse()*glm::vec4(ray.d,0.f));
      return Ray(glm::normalize(dir), o);
   }

   //---------------------------------------------------------------------------
   //
   // INTERSECTION TEST CODE
   //
   //---------------------------------------------------------------------------

   //Sphere Intersection
   Intersection*
   Sphere::intersect(const Ray& ray) const
   {
      //Setup transformed ray
      Ray xr = ray; //transformRay(ray);
      //Intersection Code
      glm::vec3 l = getCenter() - xr.o;
      float s = glm::dot(l, xr.d);
      float ll = glm::dot(l, l);
      float rr = getRadiusSquared();
      if (s < 0.f && ll > rr) return NULL; //sphere is behind us and we're not inside
      float mm = ll-(s*s); //dist from sphere center projected onto ray to sphere center
      if (mm > rr) return NULL; //ray misses (sphere center projected onto ray - sphere center > radius)
      float q = sqrtf(rr-mm);
      float t = 0.f;
      if (ll > rr) t = s-q; //we're outside the sphere so return first point
      else t = s+q;
      glm::vec3 n = getNormalXform() * glm::normalize((xr.o+(xr.d*t))-getCenter()); //make sure to normalize n after this.
      return new Intersection(true, t, ray.o + (ray.d*t), glm::normalize(n), Surface(getColor(), getFinish()));
   }

   //Plane Intersection
   Intersection*
   Plane::intersect(const Ray& ray) const
   {
      //Setup transformed Ray
      Ray xr = ray; //transformRay(ray);
      //Intersection code
      glm::vec3 n = getNormal();
      float denom = glm::dot(xr.d, n);
      if (denom == 0.f) return NULL; //ray is parallel
      float t = -(glm::dot(n, xr.o) - getDistance()) / denom;
      if (t < 0.f) return NULL; //ray intersected behind us
      if (denom > 0.f) n = -n; //flip normal if we're under the plane
      return new Intersection(true, t, ray.o + (ray.d*t), n, Surface(getColor(), getFinish()));
   }

   //Triangle Intersection
   Intersection*
   Triangle::intersect(const Ray& ray) const
   {
      //Setup transformed Ray
      Ray xr = ray; //transformRay(ray);
      //Intersection code (Essential Mathematics for Games & Interactive Applications)
      glm::vec3 e1 = getVertex1() - getVertex0();
      glm::vec3 e2 = getVertex2() - getVertex0();
      glm::vec3 p = glm::cross(xr.d, e2);
      float a = glm::dot(e1, p);
      //if result zero, no intersection or infinite intersections
      //(ray parallel to triangle plane)
      if (a == 0.f) return NULL;
      //compute denominator
      float f = 1.f/a;
      //compute barycentric coordinates
      glm::vec3 s = xr.o - getVertex0();
      float u = f*glm::dot(s, p);
      if (u < 0.f || u > 1.f) return NULL;
      glm::vec3 q = glm::cross(s, e1);
      float v = f*glm::dot(xr.d, q);
      if (v < 0.f || u+v > 1.f) return NULL;
      //compute line parameter
      float t = f*glm::dot(e2, q);
      return new Intersection(true, t, ray.o+(ray.d*t), getNormal(), Surface(getColor(), getFinish()));
   }
}

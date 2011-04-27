/**
* RDSScene.cpp
* --------------------
* Implementation for a collection of
* useful objects for representing a scene.
* Author: Ryan Schmitt
*/

#include "RDSScene.h"
#include <algorithm>
#include <iostream>

namespace RDST
{
   //Definitions for pure virtual destructors
   SceneObject::~SceneObject()
   {}
   Colored::~Colored()
   {}
   GeomObject::~GeomObject()
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
      return Ray(dir, o);
   }

   //---------------------------------------------------------------------------
   //
   // INTERSECTION TEST CODE
   //
   //---------------------------------------------------------------------------

   //Box Intersection
   Intersection*
   Box::intersect(const Ray& ray) const
   {
      /*
      //SHORT AWESOME BOX TESTING
      //Setup transformed ray
      Ray xr = transformRay(ray);
      //Code
      glm::vec3 tmin = (getSmallCorner() - xr.o)/xr.d;
      glm::vec3 tmax = (getLargeCorner() - xr.o)/xr.d;
      glm::vec3 real_min = glm::min(tmin, tmax);
      glm::vec3 real_max = glm::max(tmin, tmax);
      float smallestMax = glm::min( glm::min(real_max.x, real_max.y), real_max.z);
      float largestMin = glm::max( glm::max(real_min.x, real_min.y), real_min.z);
      if (smallestMax < largestMin) return NULL;
      return new Intersection(true, largestMin, ray.o+(ray.d*largestMin), glm::vec3(0,1,0), Surface(getColor(), getFinish()));
      */
      //LONG OBNOXIOUS BOX TESTING
      //Setup transformed ray
      Ray xr = transformRay(ray);
      //Intersection code
      float maxS = 0.f;
      float minT = FLT_MAX; //ray.tCur;
      glm::vec3 min = getSmallCorner();
      glm::vec3 max = getLargeCorner();
      glm::vec3 n;

      // do x coordinate test (yz planes)
      float s, t;
      float recip = 1.f/xr.d.x;
      s  = (min.x - xr.o.x) * recip;
      t  = (max.x - xr.o.x) * recip;
      if (s > t) std::swap(s,t);
      //adjust min and max values
      if (s > maxS) {
         n = recip >= 0.f ? glm::vec3(-1.f, 0.f, 0.f) : glm::vec3(1.f, 0.f, 0.f);
         maxS = s;
      }
      if (t < minT) minT = t;
      //check for intersection failure
      if (maxS > minT) return NULL;

      // do y coordinate test (xz planes)
      recip = 1.f/xr.d.y;
      s = (min.y - xr.o.y) * recip;
      t = (max.y - xr.o.y) * recip;
      if (s > t) std::swap(s,t);
      //adjust min and max values
      if (s > maxS) {
         n = recip >= 0.f ? glm::vec3(0.f, -1.f, 0.f) : glm::vec3(0.f, 1.f, 0.f);
         maxS = s;
      }
      if (t < minT) minT = t;
      //check for intersection failure
      if (maxS > minT) return NULL;

      // do z coordinate test (xy planes)
      recip = 1.f/xr.d.z;
      s = (min.z - xr.o.z) * recip;
      t = (max.z - xr.o.z) * recip;
      if (s > t) std::swap(s,t);
      //adjust min and max values
      if (s > maxS) {
         n = recip >= 0.f ? glm::vec3(0.f, 0.f, -1.f) : glm::vec3(0.f, 0.f, 1.f);
         maxS = s;
      }
      if (t < minT) minT = t;
      //check for intersection failure
      if (maxS > minT) return NULL;

      n = glm::normalize(getNormalXform() * n);
      return new Intersection(true, maxS, ray.o+(ray.d*maxS), n, Surface(getColor(), getFinish()));
   }

   //Sphere Intersection
   Intersection*
   Sphere::intersect(const Ray& ray) const
   {
      //Setup transformed ray
      Ray xr = transformRay(ray);
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
      Ray xr = transformRay(ray);
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
      Ray xr = transformRay(ray);
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

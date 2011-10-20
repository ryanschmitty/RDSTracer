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
#define GLM_FORCE_INLINE
#include <glm/gtx/vector_query.hpp>

namespace RDST
{
   //Definitions for pure virtual destructors
   Colored::~Colored()
   {}
   GeomObject::~GeomObject()
   {}

   //Get stratefied samples
   boost::shared_ptr< std::vector<glm::vec3> >
   GeomObject::stratefiedSamples(bool rand, int numSamples) const
   {
      //Allocate memory
      boost::shared_ptr< std::vector<glm::vec3> > pPointList = boost::shared_ptr< std::vector<glm::vec3> >(new std::vector<glm::vec3>());
      if (numSamples <= 0) return pPointList;
      int numU1s = (int)sqrtf((float)numSamples);
      int numU2s = numSamples / numU1s;
      pPointList->reserve(numU1s * numU2s);
      //Generate samples
      for (int i=0; i<numU1s; ++i) {
         for (int j=0; j<numU2s; ++j) {
            //Add randomness or not.
            float u1, u2;
            if (rand) {
               u1 = (float)i/numU1s + unifRand()/numU1s;
               u2 = (float)j/numU2s + unifRand()/numU2s;
            }
            else {
               u1 = (float)i/numU1s;
               u2 = (float)j/numU2s;
            }
            pPointList->push_back(uniformSample(u1, u2));
         }
      }
      return pPointList;
   }

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
      bool inside = false;
      if (largestMin < 0.f) { // check for inside box
         inside = true;
         largestMin = smallestMax;
      }
      glm::vec3 n = largestMin == real_min.x || largestMin == real_max.x ? xr.d.x >= 0 ? glm::vec3(-1,0,0) : glm::vec3(1,0,0) :
                    largestMin == real_min.y || largestMin == real_max.y ? xr.d.y >= 0 ? glm::vec3(0,-1,0) : glm::vec3(0,1,0) :
                                                                           xr.d.z >= 0 ? glm::vec3(0,0,-1) : glm::vec3(0,0,1);
      return new Intersection(true, largestMin, ray.d, ray.o+(ray.d*largestMin), glm::normalize(getNormalXform()*n), Surface(getColor(), getFinish()), inside);
      /*
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
      return new Intersection(true, maxS, ray.d, ray.o+(ray.d*maxS), n, Surface(getColor(), getFinish()));
      */
   }

   //Sphere Intersection
   Intersection*
   Sphere::intersect(const Ray& ray) const
   {
      //Setup transformed ray
      Ray xr = transformRay(ray);
      ///*
      //Awesome Geometric Intersection Code
      glm::vec3 l = getCenter() - xr.o;
      float s = glm::dot(l, glm::normalize(xr.d));
      float ll = glm::dot(l, l);
      float rr = getRadiusSquared();
      if (s < 0.f && ll > rr) return NULL; //sphere is behind us and we're not inside
      float mm = ll-(s*s); //dist from sphere center projected onto ray to sphere center
      if (mm > rr) return NULL; //ray misses (sphere center projected onto ray - sphere center > radius)
      float q = sqrtf(rr-mm);
      float t = 0.f;
      float flipNormal = 1.f;
      bool inside = false;
      if (ll > rr) {
         t = (s-q) / glm::length(xr.d); //we're outside the sphere so return first point
      }
      else {
         t = (s+q) / glm::length(xr.d); //we're inside the sphere to return the second point
         flipNormal = -1.f; //and flip the normal;
         inside = true;
      }
      glm::vec3 n = glm::normalize((xr.o + xr.d*t)-getCenter());
      n = flipNormal * glm::normalize(getNormalXform() * n);
      return new Intersection(true, t, ray.d, ray.o + ray.d*t, n, Surface(getColor(), getFinish()), inside);
      //*/
      /*
      //Lame regular sphere intersection method (but it's correct)
      glm::vec3 centerToOrigin = xr.o-getCenter();
      float a = glm::dot(xr.d, xr.d);
      float b = glm::dot(xr.d, centerToOrigin);
      float c = glm::dot(centerToOrigin, centerToOrigin) - getRadiusSquared();
      float radicand = b*b - a*c;
      if (radicand < 0.f) return NULL;
      float t = 0.f;
      bool inside = false;
      float flipNormal = 1.f;
      if (radicand == 0.f) {
         t = -b / a;
      }
      else if (glm::length(centerToOrigin) > getRadius()) { //outside
         t = (-b - sqrtf(radicand)) / a;
      }
      else { //inside
         inside = true;
         flipNormal = -1.f;
         t = (-b + sqrtf(radicand)) / a;
      }
      glm::vec3 n = flipNormal * glm::normalize(getNormalXform() * ((xr.o+(xr.d*t))-getCenter()));
      return new Intersection(true, t, ray.d, ray.o + ray.d*t, n, Surface(getColor(), getFinish()), inside);
      */
   }

   //Sphere uniform unit sample point
   // from PBR v.2
   glm::vec3
   Sphere::unifUnitSample(float u1, float u2) const
   {
      float z = 1.f - 2.f*u1;
      float r = sqrtf(glm::max(0.f, 1.f - z*z));
      float phi = 2.f * PI * u2;
      float x = r * cosf(phi);
      float y = r * sinf(phi);
      return glm::normalize(glm::vec3(x, y, z));
   }

   //Sphere uniform world space sample point
   glm::vec3
   Sphere::uniformSample(float rand1, float rand2) const
   {
      glm::vec3 unitSample = unifUnitSample(rand1, rand2);
      glm::vec4 actualPoint = glm::vec4((unitSample*_radius) + _center, 1.f);
      return glm::vec3(getModelXform()*actualPoint);
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
      return new Intersection(true, t, ray.d, ray.o + (ray.d*t), glm::normalize(getNormalXform()*n), Surface(getColor(), getFinish()), false);
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
      return new Intersection(true, t, ray.d, ray.o+(ray.d*t), glm::normalize(getNormalXform()*getNormal()), Surface(getColor(), getFinish()), false);
   }

   //Triangle uniform world space sample point
   // from PBR v.2
   glm::vec3
   Triangle::uniformSample(float u1, float u2) const
   {
       float su1 = sqrtf(u1);
       float u = 1.f - su1;
       float v = u2 * su1;
       glm::vec3 p = u*_vert0 + v*_vert1 + (1-u-v)*_vert2;
       return glm::vec3(getModelXform()*glm::vec4(p,1.f));
   }
}

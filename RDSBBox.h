/**
 * File: RDSBBox.h
 * --------------------
 * Bounding Box
 * Author: Ryan Schmitt
 */

#ifndef _RDS_BBOX_H_
#define _RDS_BBOX_H_

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>

namespace RDST
{
   //---------------------------------------------------------------------------
   // Bounding Box Class
   //---------------------------------------------------------------------------
   struct BBox
   {
      //------------------------------------------------------------------------
      // Ctors
      //------------------------------------------------------------------------
      explicit BBox()
      : min(glm::vec3(FLT_MAX)),
        max(glm::vec3(-FLT_MAX))
      {}
      explicit BBox(const glm::vec3& p)
      : min(p),
        max(p)
      {}
      explicit BBox(const glm::vec3& p1, const glm::vec3& p2)
      : min(glm::min(p1,p2)),
        max(glm::max(p1,p2))
      {}

      //------------------------------------------------------------------------
      // Static Utility functions
      //------------------------------------------------------------------------
      //Create a box that contains b and p.
      static BBox Union(const BBox& b, const glm::vec3& p) {
         BBox ret = b;
         ret.min = glm::min(b.min, p);
         ret.max = glm::max(b.max, p);
         return ret;
      }
      //Create a box that contains b and b2.
      static BBox Union(const BBox& b, const BBox& b2) {
         BBox ret = b;
         ret.min = glm::min(b.min, b2.min);
         ret.max = glm::max(b.max, b2.max);
         return ret;
      }

      //------------------------------------------------------------------------
      // Utility functions
      //------------------------------------------------------------------------
      //grow the box
      void expand(float delta) {
         min -= glm::vec3(delta);
         max += glm::vec3(delta);
      }
      //do boxes overlap?
      bool overlaps(const BBox& b) const {
         return (max.x >= b.min.x && min.x <= b.max.x &&
                 max.y >= b.min.y && min.y <= b.max.y &&
                 max.z >= b.min.z && min.z <= b.max.z);
      }
      //is a point inside the box?
      bool inside(const glm::vec3& p) const {
         return (p.x >= min.x && p.x <= max.x &&
                 p.y >= min.y && p.y <= max.y &&
                 p.z >= min.z && p.z <= max.z);
      }
      //what is the boxes surface area?
      float surfaceArea() const {
         glm::vec3 d = max - min;
         return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
      }
      //what is the boxes volume?
      float volume() const {
         glm::vec3 d = max - min;
         return d.x * d.y * d.z;
      }
      //which axis has the max extent?
      int maximumExtent() const {
         glm::vec3 d = max - min;
         if (d.x > d.y && d.x > d.z)
            return 0; //x axis wins
         else if (d.y > d.z)
            return 1; //y axis wins
         else
            return 2; //z axis wins
      }

      //Does this ray intersect this box?
      bool intersect(const glm::vec3 o, const glm::vec3 d) {
         glm::vec3 tmin = (min - o)/d;
         glm::vec3 tmax = (max - o)/d;
         glm::vec3 real_min = glm::min(tmin, tmax);
         glm::vec3 real_max = glm::max(tmin, tmax);
         float smallestMax = glm::min( glm::min(real_max.x, real_max.y), real_max.z);
         float largestMin = glm::max( glm::max(real_min.x, real_min.y), real_min.z);
         return smallestMax >= largestMin;
      }

      //------------------------------------------------------------------------
      // Data Accessors
      //------------------------------------------------------------------------
      const glm::vec3& operator[](int i) const
      { return i < 1 ? min : max; }
      glm::vec3& operator[](int i)
      { return i < 1 ? min : max; }

      glm::vec3 min, max;
   };
}
#endif

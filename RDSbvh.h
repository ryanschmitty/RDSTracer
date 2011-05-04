/**
 * File: RDSbvh.h
 * --------------------
 * Bounding Volume Hierarchy
 * Author: Ryan Schmitt
 */

#ifndef _RDS_BVH_H_
#define _RDS_BVH_H_

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <vector>
#include "RDSBBox.h"
#include "RDSScene.h"

namespace RDST
{
   //---------------------------------------------------------------------------
   // Object Info class for BVH
   //---------------------------------------------------------------------------
   struct BVHObjectInfo
   {
      explicit BVHObjectInfo(int index, const BBox& b)
      : objectIndex(index),
        bounds(b)
      { centroid = 0.5f * b.min + 0.5f * b.max; }

      int objectIndex;
      glm::vec3 centroid;
      BBox bounds;
   };

   //---------------------------------------------------------------------------
   // BVH Functionality class
   //---------------------------------------------------------------------------
   class BVH
   {
   public:
      explicit BVH(const std::vector<GeomObjectPtr>& objs)
      : buildData(std::vector<BVHObjectInfo>())
      { initBuildData(objs); }

   private:
      void initBuildData(const std::vector<GeomObjectPtr>& objs);

      std::vector<BVHObjectInfo> buildData;
   };
}

#endif

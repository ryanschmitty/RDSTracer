/**
 * File: RDSbvh.cpp
 * --------------------
 * Bounding Volume Hierarchy implementation
 * Author: Ryan Schmitt
 */

#include "RDSbvh.h"

namespace RDST {
   void BVH::initBuildData(const std::vector<GeomObjectPtr>& objs)
   {
      buildData.reserve(objs.size());
      for (unsigned int i=0; i<objs.size(); ++i) {
         BBox bbox = objs[i]->getWorldBounds();
         buildData.push_back(BVHObjectInfo(i, bbox));
      }
   }
}
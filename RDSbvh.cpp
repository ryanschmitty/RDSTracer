/**
 * File: RDSbvh.cpp
 * --------------------
 * Bounding Volume Hierarchy implementation
 * Author: Ryan Schmitt
 */

#include "RDSbvh.h"

namespace RDST {
   void BVH::initBuildData()
   {
      buildData.reserve(pObjs->size());
      for (unsigned int i=0; i<pObjs->size(); ++i) {
         BBox bbox = (*pObjs)[i]->getWorldBounds();
         buildData.push_back(BVHObjectInfo(i, bbox));
      }
   }
}
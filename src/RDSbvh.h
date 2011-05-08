/**
 * File: RDSbvh.h
 * --------------------
 * Bounding Volume Hierarchy
 * from "PBR: from theory to implementation" second edition
 * Author: Ryan Schmitt
 */

#ifndef _RDS_BVH_H_
#define _RDS_BVH_H_

#if defined(_WIN32) || defined(_WIN64)
#define IS_WINDOWS
#elif defined(__linux__)
#define IS_LINUX
#endif

#if defined(IS_WINDOWS)
#define uint8_t unsigned __int8
#define uint32_t unsigned __int32
#elif defined(IS_LINUX)
#include <stdint.h>
#endif

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/assert.hpp>
#include "RDSBBox.h"
#include "RDSScene.h"
#include "ProgressBar.h"

namespace RDST
{
   //---------------------------------------------------------------------------
   // Object Info class for BVH
   //---------------------------------------------------------------------------
   struct BVHObjectInfo
   {
      explicit BVHObjectInfo(int index, const BBox& b)
      : primitiveNumber(index),
        bounds(b)
      { centroid = 0.5f * b.min + 0.5f * b.max; }

      int primitiveNumber;
      glm::vec3 centroid;
      BBox bounds;
   };

   //---------------------------------------------------------------------------
   // A BVH Node
   //---------------------------------------------------------------------------
   struct BVHNode
   {
      explicit BVHNode()
      { children[0] = children[1] = boost::shared_ptr<BVHNode>(); }

      void initLeaf(uint32_t first, uint32_t n, const BBox& b) {
         firstPrimOffset = first;
         nPrimitives = n;
         bounds = b;
      }
      void initInterior(uint32_t axis, boost::shared_ptr<BVHNode> c0, boost::shared_ptr<BVHNode> c1) {
         children[0] = c0;
         children[1] = c1;
         bounds = BBox::Union(c0->bounds, c1->bounds);
         splitAxis = axis;
         nPrimitives = 0;
      }

      BBox bounds;
      boost::shared_ptr<BVHNode> children[2];
      uint32_t splitAxis, firstPrimOffset, nPrimitives;
   };

   //---------------------------------------------------------------------------
   // Functor for BVHObjectInfo comparison (sorting)
   //---------------------------------------------------------------------------
   struct CompareToMid
   {
      CompareToMid(int d, float m) { dim = d; mid = m; }
      int dim;
      float mid;
      bool operator()(const BVHObjectInfo& a) const {
         return a.centroid[dim] < mid;
      }
   };

   //---------------------------------------------------------------------------
   // A BVH Node for Linear Storage
   //---------------------------------------------------------------------------
   struct LinearBVHNode
   {
      BBox bounds;
      union {
         uint32_t primitivesOffset; //used for leaf
         uint32_t secondChildOffset; //used for interior
      };
      uint8_t nPrimitives; // 0 means interior node
      uint8_t axis; // which axis was used for splitting
      uint8_t pad[2]; //ensure 32 byte total size for SPEEED
   };

   //---------------------------------------------------------------------------
   // BVH Functionality class
   //---------------------------------------------------------------------------
   class BVH
   {
   public:
      explicit BVH(boost::shared_ptr< std::vector<GeomObjectPtr> > pObjects);
      Intersection* intersect(Ray& ray) const;

   private:
      //HELPER FUNCTIONS
      boost::shared_ptr<BVHNode> recursiveBuild(std::vector<BVHObjectInfo>& buildData, uint32_t start, uint32_t end,
                                                uint32_t* totalNodes, std::vector<GeomObjectPtr>& orderedPrims);
      uint32_t flattenBVHTree(boost::shared_ptr<BVHNode> node, uint32_t* offset);

      //DATA
      boost::shared_ptr< std::vector<GeomObjectPtr> > pObjs;
      boost::shared_array<LinearBVHNode> nodes;
   };
}

#endif

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
#define uint8_t unsigned __int8
#elif defined(__linux__)
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
      : objectNumber(index),
        bounds(b)
      { centroid = 0.5f * b.min + 0.5f * b.max; }

      int objectNumber;
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

      void initLeaf(int first, int n, const BBox& b) {
         firstObjOffset = first;
         nObjects = n;
         bounds = b;
      }
      void initInterior(int axis, boost::shared_ptr<BVHNode> c0, boost::shared_ptr<BVHNode> c1) {
         children[0] = c0;
         children[1] = c1;
         bounds = BBox::Union(c0->bounds, c1->bounds);
         splitAxis = axis;
         nObjects = 0;
      }

      BBox bounds;
      boost::shared_ptr<BVHNode> children[2];
      int splitAxis, firstObjOffset, nObjects;
   };

   //---------------------------------------------------------------------------
   // Functors for BVHObjectInfo comparison (sorting)
   //---------------------------------------------------------------------------
   struct CompareToMid
   {
      CompareToMid(int d, float m)
      : dim(d), mid(m)
      {}
      int dim;
      float mid;
      bool operator()(const BVHObjectInfo& a) const {
         return a.centroid[dim] < mid;
      }
   };

   struct ComparePoints
   {
      ComparePoints(int d)
      : dim(d)
      {}
      int dim;
      bool operator()(const BVHObjectInfo &a, const BVHObjectInfo &b) const {
         return a.centroid[dim] < b.centroid[dim];
      }
   };

   struct CompareToBucket
   {
      CompareToBucket(int split, int num, int d, const BBox& b)
      : splitBucket(split), nBuckets(num), dim(d), centroidBounds(b)
      {}
      int splitBucket, nBuckets, dim;
      const BBox& centroidBounds;
      bool operator()(const BVHObjectInfo& p) const {
         int b = (int)(nBuckets * ((p.centroid[dim] - centroidBounds.min[dim]) / (centroidBounds.max[dim] - centroidBounds.min[dim])));
         if (b == nBuckets) b = nBuckets-1;
         return b <= splitBucket;
      }
   };

   //---------------------------------------------------------------------------
   // A BVH Node for Linear Storage
   //---------------------------------------------------------------------------
   struct LinearBVHNode
   {
      BBox bounds;
      union {
         int objectOffset; //used for leaf
         int secondChildOffset; //used for interior
      };
      uint8_t nObjects; // 0 means interior node
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
      //INTERNAL TYPES
      struct BucketInfo {
         BucketInfo() {count = 0;}
         int count;
         BBox bounds;
      };

      //HELPER FUNCTIONS
      boost::shared_ptr<BVHNode> recursiveBuild(std::vector<BVHObjectInfo>& buildData, int start, int end,
                                                int* totalNodes, std::vector<GeomObjectPtr>& orderedObjs);
      int flattenBVHTree(boost::shared_ptr<BVHNode> node, int* offset);
      void createLeaf(boost::shared_ptr<BVHNode> node, std::vector<BVHObjectInfo>& buildData,
                      int start, int end, const BBox& bbox, std::vector<GeomObjectPtr>& orderedObjs);
      

      //DATA
      boost::shared_ptr< std::vector<GeomObjectPtr> > pObjs;
      boost::shared_array<LinearBVHNode> nodes;
      static const int maxObjsInNode = 10; //Forces a split if objects in node > max
   };
}

#endif

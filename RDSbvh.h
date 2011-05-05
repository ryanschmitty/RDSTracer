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
#include <boost/shared_ptr.hpp>
#include "RDSBBox.h"
#include "RDSScene.h"

namespace RDST
{
   typedef unsigned int uint_t;

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

      void initLeaf(uint_t first, uint_t n, const BBox& b) {
         firstPrimOffset = first;
         nPrimitives = n;
         bounds = b;
      }
      void initInterior(uint_t axis, boost::shared_ptr<BVHNode> c0, boost::shared_ptr<BVHNode> c1) {
         children[0] = c0;
         children[1] = c1;
         bounds = BBox::Union(c0->bounds, c1->bounds);
         splitAxis = axis;
         nPrimitives = 0;
      }

      BBox bounds;
      boost::shared_ptr<BVHNode> children[2];
      uint_t splitAxis, firstPrimOffset, nPrimitives;
   };

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
   // BVH Functionality class
   //---------------------------------------------------------------------------
   class BVH
   {
   public:
      explicit BVH(boost::shared_ptr<std::vector<GeomObjectPtr>> pObjects)
      : totalNodes(0),
        root(boost::shared_ptr<BVHNode>()),
        buildData(std::vector<BVHObjectInfo>()),
        pObjs(pObjects)
      {
         initBuildData();
         buildBVH();
      }

      void initBuildData();
      void buildBVH() {
         totalNodes = 0;
         std::vector<GeomObjectPtr> orderedPrims;
         orderedPrims.reserve(pObjs->size());
         root = recursiveBuild(0, pObjs->size(), &totalNodes, orderedPrims);
         pObjs->swap(orderedPrims);
      }

      boost::shared_ptr<BVHNode> recursiveBuild(uint_t start, uint_t end, uint_t* totalNodes, std::vector<GeomObjectPtr>& orderedPrims) {
         (*totalNodes)++;
         boost::shared_ptr<BVHNode> node(new BVHNode());
         //Compute bounds of all primitives in BVH node
         BBox bbox;
         for (uint_t i=start; i<end; ++i) {
            bbox = BBox::Union(bbox, buildData[i].bounds);
         }
         uint_t nPrimitives = end - start;
         if (nPrimitives == 1) {
            //create leaf
            uint_t firstPrimOffset = orderedPrims.size();
            for (uint_t i=start; i<end; ++i) {
               uint_t primNum = buildData[i].primitiveNumber;
               orderedPrims.push_back((*pObjs)[primNum]);
            }
            node->initLeaf(firstPrimOffset, nPrimitives, bbox);
         }
         else {
            //Compute bound of primitive centroids, choose split dimension
            BBox centroidBounds;
            for (uint_t i=start; i<end; ++i) {
               centroidBounds = BBox::Union(centroidBounds, buildData[i].centroid);
            }
            int dim = centroidBounds.maximumExtent();
            //Partition primitives into two sets and build children
            uint_t mid = (start + end) / 2;
            if (centroidBounds.max[dim] == centroidBounds.min[dim]) {
               //create leaf
               uint_t firstPrimOffset = orderedPrims.size();
               for (uint_t i=start; i<end; ++i) {
                  uint_t primNum = buildData[i].primitiveNumber;
                  orderedPrims.push_back((*pObjs)[primNum]);
               }
               node->initLeaf(firstPrimOffset, nPrimitives, bbox);
               return node;
            }
            //partition primitives based on splitMethod
            float midPoint = .5f * (centroidBounds.min[dim] + centroidBounds.max[dim]);
            BVHObjectInfo* pMid = std::partition(&buildData[start], &buildData[end-1]+1, CompareToMid(dim, midPoint));
            mid = pMid - &buildData[0];
            node->initInterior(dim,
                               recursiveBuild(start, mid, totalNodes, orderedPrims),
                               recursiveBuild(mid, end, totalNodes, orderedPrims));
         }
         return node;
      }

      uint_t totalNodes;
      boost::shared_ptr<BVHNode> root;
      boost::shared_ptr<std::vector<GeomObjectPtr>> pObjs;
      std::vector<BVHObjectInfo> buildData;
   };
}

#endif

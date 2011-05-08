/**
* File: RDSbvh.cpp
* --------------------
* Bounding Volume Hierarchy implementation
* Author: Ryan Schmitt
*/

#include "RDSbvh.h"
#include <iostream>

namespace RDST
{
   //---------------------------------------------------------------------------
   // BVH Ctor
   //---------------------------------------------------------------------------
   BVH::BVH(boost::shared_ptr< std::vector<GeomObjectPtr> > pObjects)
      : pObjs(pObjects)
   {
      if (!pObjs) return;
      std::cout << "Building BVH...";
      //Initialize build data
      std::vector<BVHObjectInfo> buildData;
      buildData.reserve(pObjs->size());
      for (unsigned int i=0; i<pObjs->size(); ++i) {
         BBox bbox = (*pObjs)[i]->getWorldBounds();
         buildData.push_back(BVHObjectInfo(i, bbox));
      }
      //Recursively build BVH
      uint32_t totalNodes = 0;
      std::vector<GeomObjectPtr> orderedPrims;
      orderedPrims.reserve(pObjs->size());
      boost::shared_ptr<BVHNode> root = recursiveBuild(buildData, 0, pObjs->size(), &totalNodes, orderedPrims);
      pObjs->swap(orderedPrims);
      //Flatten for depth-first traversal algorithm
      nodes = boost::shared_array<LinearBVHNode>(new LinearBVHNode[totalNodes]());
      uint32_t offset = 0;
      flattenBVHTree(root, &offset);
      std::cout << "Done." << std::endl;
      BOOST_ASSERT(offset == totalNodes);
   }

   //---------------------------------------------------------------------------
   // Build a BVH tree recursively
   //---------------------------------------------------------------------------
   boost::shared_ptr<BVHNode> BVH::recursiveBuild(std::vector<BVHObjectInfo>& buildData, uint32_t start, uint32_t end, uint32_t* totalNodes, std::vector<GeomObjectPtr>& orderedPrims) {
      (*totalNodes)++;
      boost::shared_ptr<BVHNode> node(new BVHNode());
      //Compute bounds of all primitives for this call
      BBox bbox;
      for (uint32_t i=start; i<end; ++i) {
         bbox = BBox::Union(bbox, buildData[i].bounds);
      }
      uint32_t nPrimitives = end - start;
      if (nPrimitives == 1) {
         //create leaf
         uint32_t firstPrimOffset = orderedPrims.size();
         for (uint32_t i=start; i<end; ++i) {
            uint32_t primNum = buildData[i].primitiveNumber;
            orderedPrims.push_back((*pObjs)[primNum]);
         }
         node->initLeaf(firstPrimOffset, nPrimitives, bbox);
      }
      else {
         //create interior
         //Compute bound of primitive centroids, choose split dimension as axis with largest extent
         BBox centroidBounds;
         for (uint32_t i=start; i<end; ++i) {
            centroidBounds = BBox::Union(centroidBounds, buildData[i].centroid);
         }
         int dim = centroidBounds.maximumExtent();
         //Partition primitives into two sets and build children
         uint32_t mid = (start + end) / 2;
         if (centroidBounds.max[dim] == centroidBounds.min[dim]) {
            //create leaf
            uint32_t firstPrimOffset = orderedPrims.size();
            for (uint32_t i=start; i<end; ++i) {
               uint32_t primNum = buildData[i].primitiveNumber;
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
            recursiveBuild(buildData, start, mid, totalNodes, orderedPrims),
            recursiveBuild(buildData, mid, end, totalNodes, orderedPrims));
      }
      return node;
   }

   //---------------------------------------------------------------------------
   // Turn BVH Tree into a Linear Array suitable for Depth-first traversal
   //---------------------------------------------------------------------------
   uint32_t BVH::flattenBVHTree(boost::shared_ptr<BVHNode> node, uint32_t* offset) {
      LinearBVHNode* linearNode = &nodes[*offset];
      linearNode->bounds = node->bounds;
      uint32_t myOffset = (*offset)++;
      if (node->nPrimitives > 0) {
         linearNode->primitivesOffset = node->firstPrimOffset;
         linearNode->nPrimitives = node->nPrimitives;
      }
      else {
         linearNode->axis = node->splitAxis;
         linearNode->nPrimitives = 0;
         flattenBVHTree(node->children[0], offset);
         linearNode->secondChildOffset = flattenBVHTree(node->children[1], offset);
      }
      return myOffset;
   }

   //---------------------------------------------------------------------------
   // Fast Ray/BBox intersection for BVH traversal
   //---------------------------------------------------------------------------
   static inline bool FastRayBoxIntersection(const BBox& bounds, const Ray& ray, const glm::vec3& invDir, const uint32_t dirIsNeg[3]) {
      //check for ray intersection against x and y slabs
      float tmin =  (bounds[  dirIsNeg[0]].x - ray.o.x) * invDir.x;
      float tmax =  (bounds[1-dirIsNeg[0]].x - ray.o.x) * invDir.x;
      float tymin = (bounds[  dirIsNeg[1]].y - ray.o.y) * invDir.y;
      float tymax = (bounds[1-dirIsNeg[1]].y - ray.o.y) * invDir.y;
      if ((tmin > tymax) || (tymin > tmax))
         return false;
      if (tymin > tmin) tmin = tymin;
      if (tymax < tmax) tmax = tymax;
      //check for ray intersection against z slab
      float tzmin = (bounds[  dirIsNeg[2]].z - ray.o.z) * invDir.z;
      float tzmax = (bounds[1-dirIsNeg[2]].z - ray.o.z) * invDir.z;
      if ((tmin > tzmax) || (tzmin > tmax))
         return false;
      if (tzmin > tmin) tmin = tzmin;
      if (tzmax < tmax) tmax = tzmax;
      return (tmin < ray.tMax) && (tmax > ray.tMin);
   }

   //---------------------------------------------------------------------------
   // Intersect with linear BVH
   //---------------------------------------------------------------------------
   Intersection* BVH::intersect(Ray& ray) const {
      Intersection* pIsect = new Intersection();
      if (!nodes) return pIsect;
      glm::vec3 origin = ray.o + (ray.tMin * ray.d);
      glm::vec3 invDir(1.f / ray.d.x, 1.f / ray.d.y, 1.f / ray.d.z);
      uint32_t dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };
      //follow ray through BVH nodes to find primitive intersections
      uint32_t todoOffset = 0, nodeNum = 0;
      uint32_t todo[64];
      while (true) {
         const LinearBVHNode* node = &nodes[nodeNum];
         //Check ray against BVH node
         if (RDST::FastRayBoxIntersection(node->bounds, ray, invDir, dirIsNeg)) {
            if (node->nPrimitives > 0) {
               //intersect ray with primitives in leaf BVH node
               for (uint32_t i=0; i < node->nPrimitives; ++i) {
                  Intersection* pIntrs = (*pObjs)[node->primitivesOffset+i]->intersect(ray);
                  if (pIntrs != NULL) {
                     if ((pIntrs->t < ray.tCur) && (pIntrs->t < ray.tMax) && (pIntrs->t > ray.tMin)) {
                        ray.tCur = pIntrs->t;
                        delete pIsect;
                        pIsect = pIntrs;
                     }
                     else {
                        delete  pIntrs;
                     }
                  }
               }
               if (todoOffset == 0) break;
               nodeNum = todo[--todoOffset];
            }
            else {
               //put far BVH node on todo stack, advance to near node
               if (dirIsNeg[node->axis]) {
                  todo[todoOffset++] = nodeNum + 1;
                  nodeNum = node->secondChildOffset;
               }
               else {
                  todo[todoOffset++] = node->secondChildOffset;
                  nodeNum = nodeNum + 1;
               }
            }
         }
         else {
            if (todoOffset == 0) break;
            nodeNum = todo[--todoOffset];
         }
      }
      return pIsect;
   }
}

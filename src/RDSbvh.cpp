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
      int totalNodes = 0;
      std::vector<GeomObjectPtr> orderedObjs;
      orderedObjs.reserve(pObjs->size());
      boost::shared_ptr<BVHNode> root = recursiveBuild(buildData, 0, pObjs->size(), &totalNodes, orderedObjs);
      pObjs->swap(orderedObjs);
      //Flatten for depth-first traversal algorithm
      nodes = boost::shared_array<LinearBVHNode>(new LinearBVHNode[totalNodes]());
      int offset = 0;
      flattenBVHTree(root, &offset);
      std::cout << "Done." << std::endl;
      BOOST_ASSERT(offset == totalNodes);
   }

   //---------------------------------------------------------------------------
   // Build a BVH tree recursively
   //---------------------------------------------------------------------------
   boost::shared_ptr<BVHNode> BVH::recursiveBuild(std::vector<BVHObjectInfo>& buildData, int start, int end, int* totalNodes, std::vector<GeomObjectPtr>& orderedObjs) {
      (*totalNodes)++;
      boost::shared_ptr<BVHNode> node(new BVHNode());
      //Compute bounds of all primitives for this call
      BBox bbox;
      for (int i=start; i<end; ++i) {
         bbox = BBox::Union(bbox, buildData[i].bounds);
      }
      int nObjects = end - start;
      //Create Leaf Node
      if (nObjects == 1) {
         createLeaf(node, buildData, start, end, bbox, orderedObjs);
      }
      //Create Interior Node
      else {
         //Compute bound of primitive centroids, choose split dimension as axis with largest extent
         BBox centroidBounds;
         for (int i=start; i<end; ++i) {
            centroidBounds = BBox::Union(centroidBounds, buildData[i].centroid);
         }
         int dim = centroidBounds.maximumExtent();
         // if centroidBounds has zero volume, create a leaf
         if (centroidBounds.max[dim] == centroidBounds.min[dim]) {
            createLeaf(node, buildData, start, end, bbox, orderedObjs);
            return node;
         }
         //Build children using SAH, unless there's <= 4 objects, then just split in half
         int mid;
         if (nObjects <= 4) {
            mid = (start + end) / 2;
            std::nth_element(&buildData[start], &buildData[mid], &buildData[end-1]+1, ComparePoints(dim));
         }
         else {
            //Allocate BucketInfo structs for SAH partition buckets
            const int nBuckets = 12;
            BucketInfo buckets[nBuckets];
            //Init BucketInfo stucts
            for (int i=start; i<end; ++i) {
               int b = (int)(nBuckets * ((buildData[i].centroid[dim] - centroidBounds.min[dim]) / (centroidBounds.max[dim] - centroidBounds.min[dim])));
               if (b == nBuckets) b = nBuckets-1;
               buckets[b].count++;
               buckets[b].bounds = BBox::Union(buckets[b].bounds, buildData[i].bounds);
            }
            //Compute costs for splitting after each bucket (i.e. after bucket 0, after bucket 1, ..., after bucket n-1) (can't split after last bucket, that's not splitting...)
            float cost[nBuckets-1];
            for (int i=0; i<nBuckets-1; ++i) {
               BBox b0, b1;
               int count0 = 0, count1 = 0;
               for (int j=0; j<=i; ++j) {
                  b0 = BBox::Union(b0, buckets[j].bounds);
                  count0 += buckets[j].count;
               }
               for (int j=i+1; j<nBuckets; ++j) {
                  b1 = BBox::Union(b1, buckets[j].bounds);
                  count1 += buckets[j].count;
               }
               //cost = traverse_cost + (count*isect_cost*Pa) + (count*isect_cost*Pb), where Px is the surface area ratio (i.e. probability a ray will hit child)
               cost[i] = 0.125f + (count0*b0.surfaceArea() + count1*b1.surfaceArea()) / bbox.surfaceArea();
            }
            //Find minimum cost split by bucket #
            float minCost = cost[0];;
            int minCostSplit = 0;
            for (int i=0; i<nBuckets-1; ++i) {
               if (cost[i] < minCost) {
                  minCost = cost[i];
                  minCostSplit = i;
               }
            }
            //Either split after selected min cost bucket, or create a leaf (if it's cheaper)
            if (nObjects > maxObjsInNode || minCost < nObjects) {
               BVHObjectInfo* pMid = std::partition(&buildData[start], &buildData[end-1]+1, CompareToBucket(minCostSplit, nBuckets, dim, centroidBounds));
               mid = pMid - &buildData[0];
            }
            else {
               createLeaf(node, buildData, start, end, bbox, orderedObjs);
               return node;
            }
         }
         node->initInterior(dim,
            recursiveBuild(buildData, start, mid, totalNodes, orderedObjs),
            recursiveBuild(buildData, mid, end, totalNodes, orderedObjs));
      }
      return node;
   }

   //---------------------------------------------------------------------------
   // Utility for creating a Leaf BVHNode
   //---------------------------------------------------------------------------
   inline void BVH::createLeaf(boost::shared_ptr<BVHNode> node, std::vector<BVHObjectInfo>& buildData, int start, int end, const BBox& bbox, std::vector<GeomObjectPtr>& orderedObjs) {
      int firstObjOffset = orderedObjs.size();
      for (int i=start; i<end; ++i) {
         int objNdx = buildData[i].objectNumber;
         orderedObjs.push_back((*pObjs)[objNdx]);
      }
      node->initLeaf(firstObjOffset, end-start, bbox);
   }

   //---------------------------------------------------------------------------
   // Turn BVH Tree into a Linear Array suitable for Depth-first traversal
   //---------------------------------------------------------------------------
   int BVH::flattenBVHTree(boost::shared_ptr<BVHNode> node, int* offset) {
      LinearBVHNode* linearNode = &nodes[*offset];
      linearNode->bounds = node->bounds;
      int myOffset = (*offset)++;
      if (node->nObjects > 0) {
         //create leaf linear node
         linearNode->primitivesOffset = node->firstObjOffset;
         linearNode->nObjects = node->nObjects;
      }
      else {
         //create interior linear node
         linearNode->axis = node->splitAxis;
         linearNode->nObjects = 0;
         //immediately push first child, delay second child until first child tree is done.
         flattenBVHTree(node->children[0], offset);
         linearNode->secondChildOffset = flattenBVHTree(node->children[1], offset);
      }
      return myOffset;
   }

   //---------------------------------------------------------------------------
   // Fast Ray/BBox intersection for BVH traversal
   //---------------------------------------------------------------------------
   static inline bool FastRayBoxIntersection(const BBox& bounds, const Ray& ray, const glm::vec3& invDir, const int dirIsNeg[3]) {
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
      int dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };
      //follow ray through BVH nodes to find primitive intersections
      int todoOffset = 0, nodeNum = 0;
      int todo[64]; //todo stack w/ todoOffset as the next free element in the stack (above the top)
      while (true) {
         const LinearBVHNode* node = &nodes[nodeNum];
         //Check ray against BVH node
         if (RDST::FastRayBoxIntersection(node->bounds, ray, invDir, dirIsNeg)) {
            if (node->nObjects > 0) { //leaf! Do actual intersections
               //intersect ray with primitives in leaf BVH node
               for (int i=0; i < node->nObjects; ++i) {
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
            else { //interior, just process children
               //put far BVH node on todo stack, advance to near node
               if (dirIsNeg[node->axis]) {
                  //if dir is neg, then 1st child is far
                  todo[todoOffset++] = nodeNum + 1;
                  nodeNum = node->secondChildOffset;
               }
               else {
                  //if dir is pos, then 2nd child is far
                  todo[todoOffset++] = node->secondChildOffset;
                  nodeNum = nodeNum + 1;
               }
            }
         }
         else { //Did not hit current BBox; pop one off the stack to process, unless the stack is empty (then stop instead)
            if (todoOffset == 0) break;
            nodeNum = todo[--todoOffset];
         }
      }
      return pIsect;
   }
}

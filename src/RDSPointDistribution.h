/**
* RDSPointDistribution.h
* --------------------
* Functions to distribute points on geometry
* Author: Ryan Schmitt
*/

#ifndef __RDS_POINT_DISTRIBUTION_H__
#define __RDS_POINT_DISTRIBUTION_H__

#define ABS(x) (x < 0 ? -(x) : (x))

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <boost/shared_ptr.hpp>
#include "RandUtil.h"
#include "RDSScene.h"
#include "RDSTracer.h"

namespace RDST
{
    /*
     * Function declarations.
     */
    void Normalise(glm::vec3 *,float, glm::vec3);
    float Distance(glm::vec3,glm::vec3);
    void FindClosestPoints(int*, int*, float*, boost::shared_ptr< std::vector<glm::vec3> >);
    void FindClosestPoints(int*, int*, float*, boost::shared_ptr< std::vector<glm::vec4> >);

    /*
     * Generate sample points for a triangle.
     */
    boost::shared_ptr< std::vector<glm::vec3> >
    GenerateDistributedPoints(int numPoints, const RDST::Triangle& tri, int maxIterations, float* minDist)
    {
       int minp1, minp2;
       float mind;
       int n = 2*numPoints;
       int numU1s = (int)sqrtf((float)n);
       int numU2s = n / numU1s;

       //Allocate memory (smart pointer to a vec3 array)
       boost::shared_ptr< std::vector<glm::vec3> > pPointVec = boost::shared_ptr< std::vector<glm::vec3> >(new std::vector<glm::vec3>());
       pPointVec->reserve(numU1s*numU2s);

       // Create the initial random cloud
       glm::vec3 v0 = tri.getVertex0();
       glm::vec3 v1 = tri.getVertex1();
       glm::vec3 v2 = tri.getVertex2();
       glm::vec3 v01 = v1 - v0;
       glm::vec3 v02 = v2 - v0;
       for (int i=0; i<numU1s; ++i) {
          for (int j=0; j<numU2s; ++j) {
             float u1 = (float)i/numU1s + unifRand()/numU1s;
             float u2 = (float)j/numU2s + unifRand()/numU2s;
             glm::vec3 p;
             // Inside or outside the triangle?
             if (u1 + u2 < 1) //inside
                p = v0 + u1*v01 + u2*v02;
             else //outside, so project it back
                p = v0 + (1.f-u1)*v01 + (1.f-u2)*v02;

             p = glm::vec3(tri.getModelXform()*glm::vec4(p,1.f));
             pPointVec->push_back(p);
          }
       }

       // Prune the points
       while ((int)pPointVec->size() > numPoints) {

          FindClosestPoints(&minp1, &minp2, &mind, pPointVec);

          //Remove one offending point
          pPointVec->erase(pPointVec->begin()+minp1);
       }

       //Record min distance
       FindClosestPoints(&minp1, &minp2, &mind, pPointVec);
       // For disk surfels, we'd use mind as a radius, however, for triangles
       // we need to solve for the correct distance from the center to each
       // equilateral vertex such that the surface area is preserved.
       // Note: we also multiply by 2 to cover any gaps.
       *minDist = 2.f*sqrt((3.14159265359*mind*mind) / 1.29903810568);

       return pPointVec;
    }

    void
    GenerateSurfels(std::vector<GeomObjectPtr>& surfels, const SceneDescription& scene, Triangle& tri) {
       // Use raw triangles and average color of 3 vertex
//       float oneThird = 1.f/3.f;
//       glm::vec3 v0 = tri.getVertex0(), v1 = tri.getVertex1(), v2 = tri.getVertex2();
//       glm::vec3 center(oneThird * (v0.x + v1.x + v2.x), oneThird * (v0.y + v1.y + v2.y), oneThird * (v0.z + v1.z + v2.z));
//       Intersection intrs;
//       intrs.n = tri.getNormal();
//       intrs.p = v0;
//       intrs.incDir = -intrs.n;
//       intrs.surf = Surface(tri.getColor(), tri.getFinish());
//       glm::vec3 color1 = Tracer::CalcDirectIllum(intrs, scene, 0);
//       color1 = glm::clamp(color1, 0.f, FLT_MAX);
//       intrs.p = v1;
//       glm::vec3 color2 = Tracer::CalcDirectIllum(intrs, scene, 0);
//       color2 = glm::clamp(color2, 0.f, FLT_MAX);
//       intrs.p = v2;
//       glm::vec3 color3 = Tracer::CalcDirectIllum(intrs, scene, 0);
//       color3 = glm::clamp(color3, 0.f, FLT_MAX);
//       surfels.push_back(TrianglePtr(new Triangle(v0, v1, v2, glm::vec4((color1+color2+color3)*oneThird,1.f), glm::mat4(1.f), tri.getFinish())));
       float minDist = 0.f;
       boost::shared_ptr< std::vector<glm::vec3> > pPoints = GenerateDistributedPoints(500, tri, 100000, &minDist);
       std::vector<glm::vec3>::const_iterator cit = pPoints->begin();
       for (; cit != pPoints->end(); ++cit) {
          glm::vec3 n = tri.getNormal();
          glm::vec3 tan = glm::normalize(tri.getVertex0()-tri.getVertex1());
          glm::vec3 v0,v1,v2;
          float randDegreeOffset = unifRand(0.f, 359.f);
          v0 = glm::vec3(*cit) + minDist*glm::rotate(tan, randDegreeOffset, n);
          v1 = glm::vec3(*cit) + minDist*glm::rotate(tan, randDegreeOffset + 120.f, n);
          v2 = glm::vec3(*cit) + minDist*glm::rotate(tan, randDegreeOffset + 240.f, n);
             Intersection intrs;
             intrs.n = n;
             intrs.p = *cit;
             intrs.incDir = -n;
             intrs.surf = Surface(tri.getColor(), tri.getFinish());
             glm::vec3 color = Tracer::CalcDirectIllum(intrs, scene, 0);
             color = glm::clamp(color, 0.f, FLT_MAX);
          surfels.push_back(TrianglePtr(new Triangle(v0, v1, v2, glm::vec4(color,1.f), glm::mat4(1.f), tri.getFinish())));
//          surfels.push_back(DiskPtr(new Disk(*cit, n, minDist, sphere.getColor(), glm::mat4(1.f), sphere.getFinish())));
       }
    }

    /*
     * Generate sample points for a box
     */
    boost::shared_ptr< std::vector<glm::vec4> >
    GenerateDistributedPoints(int numPoints, const RDST::Box& b, float* minDist)
    {
        int n = 2*numPoints;
        int minp1, minp2;
        float mind;
        int pointsPerSide = n / 6;
        int pointsPerDim = (int)sqrtf((float)pointsPerSide);
        n = pointsPerDim * pointsPerDim * 6;

        //Allocate memory (smart pointer to a vec3 array)
        boost::shared_ptr< std::vector<glm::vec4> > pPointVec = boost::shared_ptr< std::vector<glm::vec4> >(new std::vector<glm::vec4>());
        pPointVec->reserve(n);

        for (int side = 0; side < 6; ++side) {
            glm::vec3 start = b.getSmallCorner();
            glm::vec3 end = b.getLargeCorner();
            for (int uStep = 0; uStep < pointsPerDim; ++uStep) {
                for (int vStep = 0; vStep < pointsPerDim; ++vStep) {
                    float alpha = ((float)uStep+RDST::unifRand()) / (pointsPerDim);
                    float beta  = ((float)vStep+RDST::unifRand()) / (pointsPerDim);
                    float x,y,z;
                    if (side == 0) {
                        //small z plane, normal = <0, 0, -1>
                        x = (1.f-alpha)*start.x + alpha*end.x;
                        y = (1.f-beta) *start.y + beta *end.y;
                        z = start.z;
                    }
                    else if (side == 2) {
                        //large z plane, normal = <0, 0, 1>
                        x = (1.f-alpha)*start.x + alpha*end.x;
                        y = (1.f-beta) *start.y + beta *end.y;
                        z = end.z;
                    }
                    else if (side == 3) {
                        //small x plane, normal = <-1, 0, 0>
                        x = start.x;
                        y = (1.f-alpha)*start.y + alpha*end.y;
                        z = (1.f-beta) *start.z + beta *end.z;
                    }
                    else if (side == 1) {
                        //large x plane, normal = <1, 0, 0>
                        x = end.x;
                        y = (1.f-alpha)*start.y + alpha*end.y;
                        z = (1.f-beta) *start.z + beta *end.z;
                    }
                    else if (side == 4) {
                        //small y plane, normal = <0, -1, 0>
                        x = (1.f-alpha)*start.x + alpha*end.x;
                        y = start.y;
                        z = (1.f-beta) *start.z + beta *end.z;
                    }
                    else /*if (side == 5)*/ {
                        //large y plane, normal <0, 1, 0>
                        x = (1.f-alpha)*start.x + alpha*end.x;
                        y = end.y;
                        z = (1.f-beta) *start.z + beta *end.z;
                    }
                    glm::vec4 finalPoint = glm::vec4(glm::vec3(b.getModelXform()*glm::vec4(x,y,z,1.f)), side);
                    pPointVec->push_back(finalPoint);
                }
            }
        }

        // Prune the points
        while ((int)pPointVec->size() > numPoints) {

            FindClosestPoints(&minp1, &minp2, &mind, pPointVec);

            //Remove one offending point
            pPointVec->erase(pPointVec->begin()+minp1);
        }

        //Record min distance
        FindClosestPoints(&minp1, &minp2, &mind, pPointVec);
        // For disk surfels, we'd use mind as a radius, however, for triangles
        // we need to solve for the correct distance from the center to each
        // equilateral vertex such that the surface area is preserved.
        // Note: we also multiply by 2 to cover any gaps.
        *minDist = 2.f*sqrt((3.14159265359*mind*mind) / 1.29903810568);

        return pPointVec;
    }

    void
    GenerateSurfels(std::vector<GeomObjectPtr>& surfels, const SceneDescription& scene, Box& box) {
       float minDist = 0.f;
       boost::shared_ptr< std::vector<glm::vec4> > pPoints = GenerateDistributedPoints(500, box, &minDist);
       std::vector<glm::vec4>::const_iterator cit = pPoints->begin();
       for (; cit != pPoints->end(); ++cit) {
          glm::vec3 n;
          if      (cit->w == 0)
             n = glm::vec3( 0, 0,-1);
          else if (cit->w == 2)
             n = glm::vec3( 0, 0, 1);
          else if (cit->w == 3)
             n = glm::vec3(-1, 0, 0);
          else if (cit->w == 1)
             n = glm::vec3( 1, 0, 0);
          else if (cit->w == 4)
             n = glm::vec3( 0,-1, 0);
          else /* (cit->w == 5) */
             n = glm::vec3( 0, 1, 0);
          n = glm::normalize(box.getNormalXform() * n);
          glm::vec3 k = glm::vec3(0,1,0);
          glm::vec3 tan = fabs(n.y) == 1.f ? glm::vec3(1,0,0) : glm::normalize(k - ((glm::dot(k, n))*n)); //Gram-Schmidt Process
          glm::vec3 v0,v1,v2;
          float randDegreeOffset = unifRand(0.f, 359.f);
          v0 = glm::vec3(*cit) + minDist*glm::rotate(tan, randDegreeOffset, n);
          v1 = glm::vec3(*cit) + minDist*glm::rotate(tan, randDegreeOffset + 120.f, n);
          v2 = glm::vec3(*cit) + minDist*glm::rotate(tan, randDegreeOffset + 240.f, n);
             Intersection intrs;
             intrs.n = n;
             intrs.p = glm::vec3(*cit);
             intrs.incDir = -n;
             intrs.surf = Surface(box.getColor(), box.getFinish());
             glm::vec3 color = Tracer::CalcDirectIllum(intrs, scene, 0);
//             printf("%f %f %f\n", color.r, color.g, color.b);
             color = glm::clamp(color, 0.f, FLT_MAX);
          surfels.push_back(TrianglePtr(new Triangle(v0, v1, v2, glm::vec4(color,1.f), glm::mat4(1.f), box.getFinish())));
//          surfels.push_back(DiskPtr(new Disk(glm::vec3(*cit), n, minDist, box.getColor(), glm::mat4(1.f), box.getFinish())));
       }
    }

    /*
     * Generate points for a Sphere.
     */
    boost::shared_ptr< std::vector<glm::vec3> >
    GenerateDistributedPoints(int numPoints, const RDST::Sphere& sphere, int maxIterations, float* minDist)
    {
        static const int MULTIPLIER = 2;
        int n;
        int minp1,minp2;
        float r,mind;
        glm::mat4 xform;
        glm::vec3 c,p1,p2;
        boost::shared_ptr< std::vector<glm::vec3> > p = boost::shared_ptr< std::vector<glm::vec3> >(new std::vector<glm::vec3>());

        //Prep vars, enforce minimum points (and maximum)
        n = numPoints;
        r = sphere.getRadius();
        c = sphere.getCenter();
        xform = sphere.getModelXform();

        /* Create the initial random cloud */
        n = MULTIPLIER*n;
        int numU1s = (int)sqrtf((float)n);
        int numU2s = n / numU1s;
        for (int i=0; i<numU1s; ++i) {
            for (int j=0; j<numU2s; ++j) {
                float u1 = (float)i/numU1s + RDST::unifRand()/numU1s;
                float u2 = (float)j/numU2s + RDST::unifRand()/numU2s;
                glm::vec3 v = sphere.uniformSample(u1, u2);
                p->push_back(v);
            }
        }
        n = numU1s * numU2s; //ensure accurate count and reserve enough space
        p->reserve(MULTIPLIER*n);

        // Prune the points
        while ((int)p->size() > numPoints) {

            FindClosestPoints(&minp1, &minp2, &mind, p);

            //Remove one offending point
            p->erase(p->begin()+minp1);
        }

        //Record min distance
        FindClosestPoints(&minp1, &minp2, &mind, p);
        // For disk surfels, we'd use mind as a radius, however, for triangles
        // we need to solve for the correct distance from the center to each
        // equilateral vertex such that the surface area is preserved.
        // Note: we also multiply by 2 to cover any gaps.
        *minDist = 2.f*sqrt((3.14159265359*mind*mind) / 1.29903810568);

        return p;
    }

    void
    GenerateSurfels(std::vector<GeomObjectPtr>& surfels, const SceneDescription& scene, Sphere& sphere) {
       float minDist = 0.f;
       boost::shared_ptr< std::vector<glm::vec3> > pPoints = GenerateDistributedPoints(500, sphere, 100000, &minDist);
       std::vector<glm::vec3>::const_iterator cit = pPoints->begin();
       for (; cit != pPoints->end(); ++cit) {
          glm::vec3 n = glm::normalize( sphere.getNormalXform() * glm::normalize(*cit-sphere.getCenter()) );
          glm::vec3 k = glm::vec3(0,1,0);
          glm::vec3 tan = fabs(n.y) == 1.f ? glm::vec3(1,0,0) : glm::normalize(k - ((glm::dot(k, n))*n)); //Gram-Schmidt Process
          glm::vec3 v0,v1,v2;
          float randDegreeOffset = unifRand(0.f, 359.f);
          v0 = glm::vec3(*cit) + minDist*glm::rotate(tan, randDegreeOffset, n);
          v1 = glm::vec3(*cit) + minDist*glm::rotate(tan, randDegreeOffset + 120.f, n);
          v2 = glm::vec3(*cit) + minDist*glm::rotate(tan, randDegreeOffset + 240.f, n);
             Intersection intrs;
             intrs.n = n;
             intrs.p = *cit;
             intrs.incDir = -n;
             intrs.surf = Surface(sphere.getColor(), sphere.getFinish());
             glm::vec3 color = Tracer::CalcDirectIllum(intrs, scene, 0);
             color = glm::clamp(color, 0.f, FLT_MAX);
          surfels.push_back(TrianglePtr(new Triangle(v0, v1, v2, glm::vec4(color,1.f), glm::mat4(1.f), sphere.getFinish())));
//          surfels.push_back(DiskPtr(new Disk(*cit, n, minDist, sphere.getColor(), glm::mat4(1.f), sphere.getFinish())));
       }
    }

    void Normalise(glm::vec3* p, float r, glm::vec3 center)
    {
        float l;

        float x = p->x - center.x;
        float y = p->y - center.y;
        float z = p->z - center.z;
        l = r / sqrtf(x*x + y*y + z*z);
        p->x *= l;
        p->y *= l;
        p->z *= l;
        p->x += center.x;
        p->y += center.y;
        p->z += center.z;
    }

    float Distance(glm::vec3 p1, glm::vec3 p2)
    {
        glm::vec3 p;

        p.x = p1.x - p2.x;
        p.y = p1.y - p2.y;
        p.z = p1.z - p2.z;
        return(sqrtf(p.x*p.x + p.y*p.y + p.z*p.z));
    }

    void FindClosestPoints(int* minp1, int* minp2, float* mind, boost::shared_ptr< std::vector<glm::vec3> > p)
    {
        float d;
        *minp1 = 0;
        *minp2 = 1;
        *mind = Distance((*p)[*minp1],(*p)[*minp2]);
        for (int i=0;i<(int)p->size()-1;i++) {
            for (int j=i+1;j<(int)p->size();j++) {
                if ((d = Distance((*p)[i],(*p)[j])) < *mind) {
                    *mind = d;
                    *minp1 = i;
                    *minp2 = j;
                }
            }
        }
    }

    void FindClosestPoints(int* minp1, int* minp2, float* mind, boost::shared_ptr< std::vector<glm::vec4> > p)
    {
        float d;
        *minp1 = 0;
        *minp2 = 1;
        *mind = Distance(glm::vec3((*p)[*minp1]),glm::vec3((*p)[*minp2]));
        for (int i=0;i<(int)p->size()-1;i++) {
            for (int j=i+1;j<(int)p->size();j++) {
                if ((d = Distance(glm::vec3((*p)[i]),glm::vec3((*p)[j]))) < *mind) {
                    *mind = d;
                    *minp1 = i;
                    *minp2 = j;
                }
            }
        }
    }

} // end namespace RDST
#endif

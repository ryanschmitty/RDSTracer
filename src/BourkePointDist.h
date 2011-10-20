/*
Create N points on a sphere aproximately equi-distant from each other
Basically, N points are randomly placed on the sphere and then moved
around until then moved around until the minimal distance between the
closed two points is minimaised.
Paul Bourke, July 1996
*/

#ifndef __BOURKE_POINT_DISTRIBUTION_H__
#define __BOURKE_POINT_DISTRIBUTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include "RandUtil.h"
#include "RDSScene.h"

namespace Bourke
{

#define ABS(x) (x < 0 ? -(x) : (x))

void Normalise(glm::vec3 *,float, glm::vec3);
float Distance(glm::vec3,glm::vec3);
bool SameSide(glm::vec3, glm::vec3, glm::vec3, glm::vec3);
void PointInTriangle(glm::vec3*, glm::vec3, glm::vec3, glm::vec3);
void FindClosestPoints(int*, int*, float*, boost::shared_ptr< std::vector<glm::vec3> >);

/*
 * Generate sample points for a triangle.
 */
boost::shared_ptr< std::vector<glm::vec3> >
generateDistributedPoints(int numPoints, const RDST::Triangle& tri, int maxIterations, float* minDist)
{
    int minp1, minp2;
    float mind;
    int n = 4*numPoints;
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
            float u1 = (float)i/numU1s + RDST::unifRand()/numU1s;
            float u2 = (float)j/numU2s + RDST::unifRand()/numU2s;
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
    while (pPointVec->size() > numPoints) {

        FindClosestPoints(&minp1, &minp2, &mind, pPointVec);

        //Remove one offending point
        pPointVec->erase(pPointVec->begin()+minp1);
    }

    //Record min distance
    FindClosestPoints(&minp1, &minp2, &mind, pPointVec);
    *minDist = 2.f*mind;

    return pPointVec;
}

bool SameSide(glm::vec3 p1,glm::vec3 p2, glm::vec3 a, glm::vec3 b) {
    glm::vec3 cp1 = glm::cross(b-a, p1-a);
    glm::vec3 cp2 = glm::cross(b-a, p2-a);
    return glm::dot(cp1, cp2) >= 0;
}

void PointInTriangle(glm::vec3* p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    //if not on the right side of a line, project it back onto it.
    //P' = A + {(AB • AP) / || AB ||²} AB;
    glm::vec3 point, line, pLine;
    float length;
    if (!SameSide(*p,a, b,c)) {
        length = glm::length(c-b);
        *p = b + ((glm::dot(c-b,*p-b) / (length*length))*(c-b));
    }
    if (!SameSide(*p,b, a,c)) {
        length = glm::length(c-a);
        *p = a + ((glm::dot(c-a,*p-a) / (length*length))*(c-a));
    }
    if (!SameSide(*p,c, a,b)) {
        length = glm::length(b-a);
        *p = a + ((glm::dot(b-a,*p-a) / (length*length))*(b-a));
    }
}

/*
 * Generate sample points for a box
 */
boost::shared_ptr< std::vector<glm::vec3> >
generateDistributedPoints(int numPoints, const RDST::Box& b, float* minDist)
{
    int n = 3*numPoints;
    int minp1, minp2;
    float mind, maxd, d;
    int pointsPerSide = n / 6;
    int pointsPerDim = sqrtf(pointsPerSide);
    n = pointsPerDim * pointsPerDim * 6;

    //Allocate memory (smart pointer to a vec3 array)
    boost::shared_ptr< std::vector<glm::vec3> > pPointVec = boost::shared_ptr< std::vector<glm::vec3> >(new std::vector<glm::vec3>());
    pPointVec->reserve(n);

    for (int side = 0; side < 6; ++side) {
        glm::vec3 start = glm::vec3(b.getModelXform()*glm::vec4(b.getSmallCorner(),1.f));
        glm::vec3 end = glm::vec3(b.getModelXform()*glm::vec4(b.getLargeCorner(),1.f));
        for (int uStep = 0; uStep < pointsPerDim; ++uStep) {
            for (int vStep = 0; vStep < pointsPerDim; ++vStep) {
                float alpha = ((float)uStep+RDST::unifRand()) / (pointsPerDim);
                float beta  = ((float)vStep+RDST::unifRand()) / (pointsPerDim);
//                float alpha = (float)uStep / (pointsPerDim);
//                float beta  = (float)vStep / (pointsPerDim);
                float x,y,z;
                if (side == 0) {
                    //small z plane
                    x = (1.f-alpha)*start.x + alpha*end.x;
                    y = (1.f-beta) *start.y + beta *end.y;
                    z = start.z;
                }
                else if (side == 2) {
                    //large z plane
                    x = (1.f-alpha)*start.x + alpha*end.x;
                    y = (1.f-beta) *start.y + beta *end.y;
                    z = end.z;
                }
                else if (side == 3) {
                    //small x plane
                    x = start.x;
                    y = (1.f-alpha)*start.y + alpha*end.y;
                    z = (1.f-beta) *start.z + beta *end.z;
                }
                else if (side == 1) {
                    //large x plane
                    x = end.x;
                    y = (1.f-alpha)*start.y + alpha*end.y;
                    z = (1.f-beta) *start.z + beta *end.z;
                }
                else if (side == 4) {
                    //small y plane
                    x = (1.f-alpha)*start.x + alpha*end.x;
                    y = start.y;
                    z = (1.f-beta) *start.z + beta *end.z;
                }
                else /*if (side == 5)*/ {
                    //large y plane
                    x = (1.f-alpha)*start.x + alpha*end.x;
                    y = end.y;
                    z = (1.f-beta) *start.z + beta *end.z;
                }
                pPointVec->push_back(glm::vec3(x,y,z));
            }
        }
    }

    // Prune the points
    while (pPointVec->size() > numPoints) {

        FindClosestPoints(&minp1, &minp2, &mind, pPointVec);

        //Remove one offending point
        pPointVec->erase(pPointVec->begin()+minp1);
    }

    //Record min distance
    FindClosestPoints(&minp1, &minp2, &mind, pPointVec);
    *minDist = 2.f*mind;

    return pPointVec;
}

/*
 * Called with these arguments: the number of points to distribute,
 * the sphere, the maximum number of iterations to perform, and a
 * pointer for the minimum distance between two points.
 */
boost::shared_ptr< std::vector<glm::vec3> >
generateDistributedPoints(int numPoints, const RDST::Sphere& sphere, int maxIterations, float* minDist)
{
    static const int MULTIPLIER = 4;
    int i,j,n;
    int minp1,minp2;
    float r,d,mind,maxd;
    glm::mat4 xform;
    glm::vec3 c,p1,p2;
    boost::shared_ptr< std::vector<glm::vec3> > p = boost::shared_ptr< std::vector<glm::vec3> >(new std::vector<glm::vec3>());
    p->reserve(MULTIPLIER*n);

    //Prep vars, enforce minimum points (and maximum)
    if ((n = numPoints) < 10)
        n = 10;
    else if (n > 1000)
        n = 1000;
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
    n = numU1s * numU2s; //ensure accurate number

    // Prune the points
    while (p->size() > numPoints) {

        FindClosestPoints(&minp1, &minp2, &mind, p);

        //Remove one offending point
        p->erase(p->begin()+minp1);
    }

    FindClosestPoints(&minp1, &minp2, &mind, p);

    //Grab minimum distance
    *minDist = 2.f*mind;

    return p;
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
    for (int i=0;i<p->size()-1;i++) {
        for (int j=i+1;j<p->size();j++) {
            if ((d = Distance((*p)[i],(*p)[j])) < *mind) {
                *mind = d;
                *minp1 = i;
                *minp2 = j;
            }
        }
    }
}

} // end namespace Bourke
#endif

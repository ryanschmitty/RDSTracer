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

/*
 * Generate sample points for a triangle.
 */
boost::shared_ptr< std::vector<glm::vec3> >
generateDistributedPoints(int numPoints, const RDST::Triangle& tri, int maxIterations, float* minDist)
{
    int numU1s = (int)sqrtf((float)numPoints);
    int numU2s = numPoints / numU1s;

    //Allocate memory (smart pointer to a vec3 array)
    boost::shared_ptr< std::vector<glm::vec3> > pPointVec = boost::shared_ptr< std::vector<glm::vec3> >(new std::vector<glm::vec3>());
    pPointVec->reserve(numU1s*numU2s);

    // Create the initial random cloud
    for (int i=0; i<numU1s; ++i) {
        for (int j=0; j<numU2s; ++j) {
            float u1 = (float)i/numU1s + RDST::unifRand()/numU1s;
            float u2 = (float)j/numU2s + RDST::unifRand()/numU2s;
            pPointVec->push_back(tri.uniformSample(u1, u2));
        }
    }

    // Push two points apart
    int counter=0;
    int minp1,minp2;
    float r,d,mind,maxd;
    int n = pPointVec->size();
    glm::vec3 p1,p2;
    while (counter < maxIterations) {

        /* Find the closest two points */
        minp1 = 0;
        minp2 = 1;
        mind = Distance((*pPointVec)[minp1],(*pPointVec)[minp2]);
        maxd = mind;
        for (int i=0;i<n-1;i++) {
            for (int j=i+1;j<n;j++) {
                if ((d = Distance((*pPointVec)[i],(*pPointVec)[j])) < mind) {
                    mind = d;
                    minp1 = i;
                    minp2 = j;
                }
                if (d > maxd)
                    maxd = d;
            }
        }

        /*
        Move the two minimal points apart, in this case by 1%
        but should really vary this for refinement
        */
        p1 = (*pPointVec)[minp1];
        p2 = (*pPointVec)[minp2];
        (*pPointVec)[minp2].x = p1.x + 1.01f * (p2.x - p1.x);
        (*pPointVec)[minp2].y = p1.y + 1.01f * (p2.y - p1.y);
        (*pPointVec)[minp2].z = p1.z + 1.01f * (p2.z - p1.z);
        (*pPointVec)[minp1].x = p1.x - 0.01f * (p2.x - p1.x);
        (*pPointVec)[minp1].y = p1.y - 0.01f * (p2.y - p1.y);
        (*pPointVec)[minp1].z = p1.z - 0.01f * (p2.z - p1.z);
        PointInTriangle(&(*pPointVec)[minp1], tri.getVertex0(), tri.getVertex1(), tri.getVertex2());
        PointInTriangle(&(*pPointVec)[minp2], tri.getVertex0(), tri.getVertex1(), tri.getVertex2());
        *minDist = Distance((*pPointVec)[minp1], (*pPointVec)[minp2]);

        counter++;
    }

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
    int pointsPerSide = numPoints / 6;
    int pointsPerDim = sqrtf(pointsPerSide);

    //Allocate memory (smart pointer to a vec3 array)
    boost::shared_ptr< std::vector<glm::vec3> > pPointVec = boost::shared_ptr< std::vector<glm::vec3> >(new std::vector<glm::vec3>());
    pPointVec->reserve(pointsPerDim * pointsPerDim * 6);

    int count = 0;
    for (int side = 0; side < 6; ++side) {
        glm::vec3 start = b.getSmallCorner();
        glm::vec3 end = b.getLargeCorner();
        for (int uStep = 0; uStep < pointsPerDim; ++uStep) {
            for (int vStep = 0; vStep < pointsPerDim; ++vStep) {
                float alpha = (float)uStep / (pointsPerDim-1);
                float beta  = (float)vStep / (pointsPerDim-1);
                ++count;
                if (side == 0) {
                    //small z plane
                    pPointVec->push_back( glm::vec3(
                        (1.f-alpha)*start.x + alpha*end.x,
                        (1.f-beta) *start.y + beta *end.y,
                        start.z
                    ));
                }
                else if (side == 2) {
                    //large z plane
                    pPointVec->push_back( glm::vec3(
                        (1.f-alpha)*start.x + alpha*end.x,
                        (1.f-beta) *start.y + beta *end.y,
                        end.z
                    ));
                }
                else if (side == 3) {
                    //small x plane
                    pPointVec->push_back( glm::vec3(
                        start.x,
                        (1.f-alpha)*start.y + alpha*end.y,
                        (1.f-beta) *start.z + beta *end.z
                    ));
                }
                else if (side == 1) {
                    //large x plane
                    pPointVec->push_back( glm::vec3(
                        end.x,
                        (1.f-alpha)*start.y + alpha*end.y,
                        (1.f-beta) *start.z + beta *end.z
                    ));
                }
                else if (side == 4) {
                    //small y plane
                    pPointVec->push_back( glm::vec3(
                        (1.f-alpha)*start.x + alpha*end.x,
                        start.y,
                        (1.f-beta) *start.z + beta *end.z
                    ));
                }
                else /*if (side == 5)*/ {
                    //large y plane
                    pPointVec->push_back( glm::vec3(
                        (1.f-alpha)*start.x + alpha*end.x,
                        end.y,
                        (1.f-beta) *start.z + beta *end.z
                    ));
                }
            }
        }
    }

    printf("box count: %d\n",count);

    *minDist = Distance((*pPointVec)[1], (*pPointVec)[0]);

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
    static const int MULTIPLIER = 5;
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

    while (p->size() > numPoints) {

        /* find the two closest points */
        minp1 = 0;
        minp2 = 1;
        mind = Distance((*p)[minp1],(*p)[minp2]);
        maxd = mind;
        for (i=0;i<p->size()-1;i++) {
            for (j=i+1;j<p->size();j++) {
                if ((d = Distance((*p)[i],(*p)[j])) < mind) {
                    mind = d;
                    minp1 = i;
                    minp2 = j;
                }
                if (d > maxd)
                    maxd = d;
            }
        }

        //Remove one offending point
        p->erase(p->begin()+minp1);
    }

    //Grab minimum distance
    *minDist = 0.02f; //mind*2.f;
    printf("mindist: %f\n", mind);
    printf("maxdist: %f\n", maxd);
    printf("numPoints: %u\n", (unsigned int)p->size());

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

} // end namespace Bourke
#endif

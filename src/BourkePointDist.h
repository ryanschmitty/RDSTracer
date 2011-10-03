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

namespace Bourke
{

   #define ABS(x) (x < 0 ? -(x) : (x))

   void Normalise(glm::vec3 *,float);
   float Distance(glm::vec3,glm::vec3);

   /*
   Called with three arguments, the number of points to distribute, the
   radius of the sphere, and the maximum number of iterations to perform.
   Each has a minimum of 3, 0.001, and 100, respectively.
   */
   boost::shared_ptr< std::vector<glm::vec3> >
   generateDistributedPoints(int numPoints, glm::vec3 center, float radius, int maxIterations, float* minDist)
   {
      int i,j,n;
      int counter = 0,countmax = 100;
      int minp1,minp2;
      float r,d,mind,maxd;
      glm::vec3 p[1000],p1,p2;

      //Enforce minimums (and maximum)
      if ((n = numPoints) < 2)
         n = 3;
      else if (n > 1000)
         n = 1000;
      if ((r = radius) < 0.001)
         r = 0.001f;
      if ((countmax = maxIterations) < 100)
         countmax = 100;

      /* Create the initial random cloud */
      int numU1s = (int)sqrtf((float)n);
      int numU2s = n / numU1s;
      //Generate samples
      for (int i=0; i<numU1s; ++i) {
         for (int j=0; j<numU2s; ++j) {
            float u1 = (float)i/numU1s + RDST::unifRand()/numU1s;
            float u2 = (float)j/numU2s + RDST::unifRand()/numU2s;
               float z = 1.f - 2.f*u1;
               float ra = sqrtf(glm::max(0.f, 1.f - z*z));
               float phi = 2.f * 3.14159265f * u2;
               float x = ra * cosf(phi);
               float y = ra * sinf(phi);
           glm::vec3 v(x, y, z);
           Normalise(&v, r);
           p[(i*numU2s) + j] = v;
         }
      }
      n = numU1s * numU2s;

      while (counter < countmax) {

         /* Find the closest two points */
         minp1 = 0;
         minp2 = 1;
         mind = Distance(p[minp1],p[minp2]);
         maxd = mind;
         for (i=0;i<n-1;i++) {
            for (j=i+1;j<n;j++) {
               if ((d = Distance(p[i],p[j])) < mind) {
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
         p1 = p[minp1];
         p2 = p[minp2];
         p[minp2].x = p1.x + 1.01f * (p2.x - p1.x);
         p[minp2].y = p1.y + 1.01f * (p2.y - p1.y);
         p[minp2].z = p1.z + 1.01f * (p2.z - p1.z);
         p[minp1].x = p1.x - 0.01f * (p2.x - p1.x);
         p[minp1].y = p1.y - 0.01f * (p2.y - p1.y);
         p[minp1].z = p1.z - 0.01f * (p2.z - p1.z);
         Normalise(&p[minp1],r);
         Normalise(&p[minp2],r);
         *minDist = Distance(p[minp1], p[minp2]);

         counter++;
      }

      /* Write out the points in your favorite format */
      //Allocate memory (smart pointer to a vec3 array)
      boost::shared_ptr< std::vector<glm::vec3> > pPointVec = boost::shared_ptr< std::vector<glm::vec3> >(new std::vector<glm::vec3>());
      pPointVec->reserve(n);
      for (int i=0; i<numPoints; ++i) {
         pPointVec->push_back(glm::vec3(p[i].x + center.x, p[i].y + center.y, p[i].z + center.z));
      }
      return pPointVec;
   }

   void Normalise(glm::vec3* p, float r)
   {
      float l;

      l = r / sqrtf(p->x*p->x + p->y*p->y + p->z*p->z);
      p->x *= l;
      p->y *= l;
      p->z *= l;
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
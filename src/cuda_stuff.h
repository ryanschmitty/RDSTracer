/**
 * cuda_stuff.h
 * --------------------
 * Code for the CUDA stuffs
 * Author: Ryan Schmitt
 */

#ifndef __CUDA_STUFF_H__
#define __CUDA_STUFF_H__

#include <cmath>

namespace RDST
{
   /**
    * Struct the represents a sphere.
    */
   struct cuda_sphere_t
   {
      float x, y, z; //Center location in world space
      float rr; //radius squared
   };

   /**
    * Struct that represents a ray
    */
   struct cuda_ray_t
   {
      float ox, oy, oz; //origin x,y,z
      float dx, dy, dz; //direction x,y,z
   };

   /**
    * Struct that represents an intersection
    */
   struct cuda_intersection_t
   {
      cuda_intersection_t()
      : objIndx(-1),
        t(FLT_MAX)
      {}
      int objIndx; //index of object that was hit, default -1
      float t; //defines intersection point along an implicit line (i.e. ray)
   };
}

#endif
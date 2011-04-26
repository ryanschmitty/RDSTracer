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
   struct vec3
   {
       vec3(float _x, float _y, float _z)
       : x(_x), y(_y), z(_z) {}
       float x, y, z;
   };

   /**
    * Struct the represents a sphere.
    */
   struct cuda_sphere_t
   {
      vec3 c; //Center location in world space
      float rr; //radius squared
   };

   /**
    * Struct that represents a ray
    */
   struct cuda_ray_t
   {
      vec3 o; //origin x,y,z
      vec3 d; //direction x,y,z
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

/**
 * cuda_stuff.h
 * --------------------
 * Code for the CUDA stuffs
 * Author: Ryan Schmitt
 */

#include<vector>

#ifndef __CUDA_STUFF_H__
#define __CUDA_STUFF_H__

namespace RDST
{
    struct vec3
    {
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
        int objIndx; //index of object that was hit, default -1
        float t; //defines intersection point along an implicit line (i.e. ray)
    };


    typedef std::vector<cuda_sphere_t> sphere_vec;
    typedef std::vector<cuda_ray_t> ray_vec;
    typedef std::vector<cuda_intersection_t> intersection_vec;
    intersection_vec cuda_ray_trace(const sphere_vec &, const ray_vec &, int width, int height);
}

#endif
#include <stdio.h>
#include <cmath>
#include "cuda_stuff.h"

__device__ float dot(vec3 v1, vec3 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

//Sphere Intersection
__device__ float intersect(cuda_sphere_t s, cuda_ray_t r) {
    //Intersection Code
    vec3 l = vec3(s.c.x - r.o.x, s.c.y - r.o.y, s.c.z - r.o.z);
    float s = dot(l, r.d);
    float ll = dot(l, l);
    float rr = c.rr;
    if (s < 0.f && ll > rr) return -1; //sphere is behind us and we're not inside
    float mm = ll-(s*s); //dist from sphere center projected onto ray to sphere center
    if (mm > rr) return -1; //ray misses (sphere center projected onto ray - sphere center > radius)
    float q = sqrtf(rr-mm);
    float t = 0.f;
    if (ll > rr) t = s-q; //we're outside the sphere so return first point
    else t = s+q;
    
    return t;
}

__global__ void RayTraceKernel(cuda_sphere_t spheres[], int spheresSize, cuda_ray_t rays[], cuda_intersection_t intrs[], int width, int height) {
    int row = threadIdx.y + blockIdx.y * blockDim.y;
    int col = threadIdx.x + blockIdx.x * blockDim.x;

    if (row >= height || col >= width) {
        return;
    }

    int idx = (row * width) + col;

    cuda_intersection_t inter();

    for (int i = 0; i < spheresSize; i++) {
        float newT = intersect(spheres[i], rays[idx]);
        if (newT >= 0 && newT < inter.t) {
            inter.t = newT;
            inter.objIndx = i;
        } 
    }

    intrs[idx] = inter;
}

#include <stdio.h>
#include <cmath>
#include <cfloat>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include "cuda_ray_tracer.h"

using namespace RDST;

__device__ float dot(vec3 v1, vec3 v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

//Sphere Intersection
__device__ float intersect(cuda_sphere_t sp, cuda_ray_t r) {
    //Intersection Code
    vec3 l;
    l.x = sp.c.x - r.o.x;
    l.y = sp.c.y - r.o.y;
    l.z = sp.c.z - r.o.z;
    float s = dot(l, r.d);
    float ll = dot(l, l);
    float rr = sp.rr;
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

    cuda_intersection_t inter;
    inter.objIndx = -1;
    inter.t = FLT_MAX;

    for (int i = 0; i < spheresSize; i++) {
        float newT = intersect(spheres[i], rays[idx]);
        if (newT >= 0 && newT < inter.t) {
            inter.t = newT;
            inter.objIndx = i;
        } 
    }

    intrs[idx] = inter;
}

__host__ intersection_vec RDST::cuda_ray_trace(const sphere_vec &spheres, const ray_vec &rays, int width, int height) {
    thrust::device_vector<cuda_sphere_t> dSpheres(spheres.begin(),
            spheres.end());
    thrust::device_vector<cuda_ray_t> dRays(rays.begin(),
            rays.end());
    thrust::device_vector<cuda_intersection_t> dIntersects(rays.size());

    cuda_sphere_t *sPtr = thrust::raw_pointer_cast(&(*dSpheres.begin()));
    cuda_ray_t *rPtr = thrust::raw_pointer_cast(&(*dRays.begin()));
    cuda_intersection_t *iPtr = thrust::raw_pointer_cast(&(*dIntersects.begin()));
    // Run kernel on spheres
    dim3 dimBlock(16, 16);
    dim3 dimGrid(512, 512);
    RayTraceKernel<<<dimGrid, dimBlock>>>(sPtr, spheres.size(), rPtr, iPtr,
            width, height);

    intersection_vec iVec(dIntersects.size());
    thrust::copy(dIntersects.begin(), dIntersects.end(), iVec.begin());
    return iVec;
}

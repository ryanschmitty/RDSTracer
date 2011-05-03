#include <stdio.h>
#include <cmath>
#include <cfloat>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include "cuda_ray_tracer.h"

using namespace RDST;

__device__ float dot(vec3 &v1, vec3 &v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

//Sphere Intersection
__device__ float intersect(cuda_sphere_t &sp, cuda_ray_t &r) {
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

__global__ void SphereIntersectKernel(cuda_sphere_t spheres[], int spheresSize,
        cuda_ray_t rays[], cuda_intersection_t intrs[], int rayCount) {
    extern __shared__ cuda_sphere_t shared[];

    if ((blockIdx.x * blockDim.x + blockIdx.y * blockDim.y * gridDim.x * blockDim.x) > rayCount)
        return;

    int rayPos = threadIdx.y * gridDim.x * blockDim.x + threadIdx.x + blockIdx.x * blockDim.x + blockIdx.y * blockDim.y * gridDim.x * blockDim.x;

    cuda_intersection_t inter;
    inter.objIndx = -1;
    inter.t = FLT_MAX;

    int shPos = threadIdx.y * blockDim.x + threadIdx.x;

    int count = spheresSize / (blockDim.x * blockDim.y);
    int i = 0;
    for (; i < count; i++) {
        shared[shPos] = spheres[blockDim.x * blockDim.y * i + shPos];
        __syncthreads();
        for (int k = 0; k < blockDim.x * blockDim.y; k++) {
            float newT = intersect(shared[k], rays[rayPos]);
            if (newT >= 0 && newT < inter.t) {
                inter.t = newT;
                inter.objIndx = i * blockDim.x * blockDim.y + k;
            }
        }
        __syncthreads();
    }

    int spill = spheresSize % (blockDim.x * blockDim.y);
    if (shPos < spill)
        shared[shPos] = spheres[blockDim.x * blockDim.y * i + shPos];
    __syncthreads();
    for (int k = 0; k < spill; k++) {
        float newT = intersect(shared[k], rays[rayPos]);
        if (newT >= 0 && newT < inter.t) {
            inter.t = newT;
            inter.objIndx = i * blockDim.x * blockDim.y + k;
        }
    }

    if (rayPos < rayCount)
        intrs[rayPos] = inter;
}

__host__ intersection_vec RDST::cuda_sphere_intersect(const sphere_vec &spheres, const ray_vec &rays, int width, int height) {
    static dim3 dimBlock(16, 16);
    static dim3 dimGrid(60, 45);
    static int batchSize = dimBlock.x * dimBlock.y * dimGrid.x * dimGrid.y;
    int count = (width * height) / batchSize;
    int spill = (width * height) % batchSize;
    intersection_vec iVec(rays.size());

    thrust::device_vector<cuda_sphere_t> dSpheres(spheres.begin(), spheres.end());
    cuda_sphere_t *sPtr = thrust::raw_pointer_cast(&(*dSpheres.begin()));

    ray_vec::const_iterator rVecI = rays.begin();
    intersection_vec::iterator iVecI = iVec.begin();
    for (int i = 0 ; i < count; i++) {
        thrust::device_vector<cuda_ray_t> dRays(rVecI, rVecI + batchSize);

        thrust::device_vector<cuda_intersection_t> dIntersects(batchSize);
        cuda_ray_t *rPtr = thrust::raw_pointer_cast(&(*dRays.begin()));
        cuda_intersection_t *iPtr = thrust::raw_pointer_cast(&(*dIntersects.begin()));

        SphereIntersectKernel<<<dimGrid, dimBlock, sizeof(cuda_sphere_t) * dimBlock.x * dimBlock.y>>>(sPtr,
                spheres.size(), rPtr, iPtr, batchSize);

        thrust::copy(dIntersects.begin(), dIntersects.end(), iVecI);
        rVecI += batchSize;
        iVecI += batchSize;
    }

    if (spill) {
        thrust::device_vector<cuda_ray_t> dRays(rVecI, rVecI + spill);

        thrust::device_vector<cuda_intersection_t> dIntersects(spill);
        cuda_ray_t *rPtr = thrust::raw_pointer_cast(&(*dRays.begin()));
        cuda_intersection_t *iPtr = thrust::raw_pointer_cast(&(*dIntersects.begin()));

        SphereIntersectKernel<<<dimGrid, dimBlock, sizeof(cuda_sphere_t) * dimBlock.x * dimBlock.y>>>(sPtr,
                spheres.size(), rPtr, iPtr, spill);

        thrust::copy(dIntersects.begin(), dIntersects.end(), iVecI);
    }

    return iVec;
}

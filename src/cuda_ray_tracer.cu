#include <stdio.h>
#include <cmath>
#include <cfloat>
#include <thrust/device_vector.h>
#include <thrust/copy.h>
#include <thrust/fill.h>
#include "cuda_ray_tracer.h"

using namespace RDST;

__device__ inline float dot(vec3 &v1, vec3 &v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

__device__ inline vec3 cross(vec3 &v1, vec3 &v2) {
    vec3 r;
    r.x = v1.y * v2.z - v1.z * v2.y;
    r.y = v1.z * v2.x - v1.x * v2.z;
    r.z = v1.x * v2.y - v1.y * v2.x;
    return r;
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
    float mm = ll-(s*s); //dist from sphere center projected onto ray to sphere center
    if (mm > rr) return -1; //ray misses (sphere center projected onto ray - sphere center > radius)
    float q = sqrtf(rr-mm);
    return s-q; //we're outside the sphere so return first point
}

#define OBJNDX (blockDim.x * blockDim.y * i + shPos)
#define OBJNDX2 (OBJNDX + blockDim.x * blockDim.y)
#define OBJNDX3 (OBJNDX2 + blockDim.x * blockDim.y)
__global__ void SphereIntersectKernel(cuda_sphere_t spheres[], int spheresSize,
        cuda_ray_t rays[], cuda_intersection_t intrs[], int rayCount) {
    extern __shared__ cuda_sphere_t sShared[];
    cuda_sphere_t *s1 = sShared;
    cuda_sphere_t *s2 = s1 + (blockDim.x * blockDim.y);
    cuda_sphere_t *s3 = s2 + (blockDim.x * blockDim.y);

    if ((blockIdx.x * blockDim.x + blockIdx.y * blockDim.y * gridDim.x * blockDim.x) > rayCount)
        return;

    int rayPos = threadIdx.y * gridDim.x * blockDim.x + threadIdx.x + blockIdx.x * blockDim.x + blockIdx.y * blockDim.y * gridDim.x * blockDim.x;

    cuda_intersection_t inter = intrs[rayPos * (rayPos < rayCount)];
    cuda_ray_t ray = rays[rayPos * (rayPos < rayCount)];

    int shPos = threadIdx.y * blockDim.x + threadIdx.x;

    int count = spheresSize / (blockDim.x * blockDim.y);
    int i = 0;
    for (; i < (count + 1); i += 3) {
        s1[shPos] = spheres[OBJNDX * (OBJNDX < spheresSize)];
        s2[shPos] = spheres[OBJNDX2 * (OBJNDX2 < spheresSize)];
        s3[shPos] = spheres[OBJNDX3 * (OBJNDX3 < spheresSize)];

        __syncthreads();
        for (int k = 0; k < blockDim.x * blockDim.y; k++) {
            float newT = intersect(s1[k], ray);
            if (newT >= 0 && newT < inter.t) {
                inter.t = newT;
                inter.objIndx = i * blockDim.x * blockDim.y + k;
                inter.type = _SPHERE;
            }
            newT = intersect(s2[k], ray);
            if (newT >= 0 && newT < inter.t) {
                inter.t = newT;
                inter.objIndx = (i + 1) * blockDim.x * blockDim.y + k;
                inter.type = _SPHERE;
            }
            newT = intersect(s3[k], ray);
            if (newT >= 0 && newT < inter.t) {
                inter.t = newT;
                inter.objIndx = (i + 2) * blockDim.x * blockDim.y + k;
                inter.type = _SPHERE;
            }
            __syncthreads();
        }
    }

    if (rayPos < rayCount)
        intrs[rayPos] = inter;
}
#undef OBJNDX
#undef OBJNDX2
#undef OBJNDX3

//Triangle Intersection
#define VEC_DIFF(vr, v1, v2)    \
{                               \
    vr.x = v1.x - v2.x;         \
    vr.y = v1.y - v2.y;         \
    vr.z = v1.z - v2.z;         \
}
__device__ float intersect(cuda_triangle_t &tr, cuda_ray_t &r) {
    vec3 e1;
    VEC_DIFF(e1, tr.v1, tr.v0);
    vec3 e2;
    VEC_DIFF(e2, tr.v2, tr.v0);
    vec3 p = cross(r.d, e2);
    float a = dot(e1, p);
    if (a == 0.f) return -1;
    float f = 1.f/a;
    vec3 s;
    VEC_DIFF(s, r.o, tr.v0);
    float u = f * dot(s, p);
    if (u < 0.f || u > 1.f) return -1;
    vec3 q = cross(s, e1);
    float v = f * dot(r.d, q);
    if (v < 0.f || u+v > 1.f) return -1;
    return f * dot(e2, q);
}
#undef VEC_DIFF

#define OBJNDX (blockDim.x * blockDim.y * i + shPos)
#define OBJNDX2 (OBJNDX + blockDim.x * blockDim.y)
__global__ void TriangleIntersectKernel(cuda_triangle_t triangles[], int triangleSize,
        cuda_ray_t rays[], cuda_intersection_t intrs[], int rayCount) {
    extern __shared__ cuda_triangle_t tShared[];
    cuda_triangle_t *t1 = tShared;
    cuda_triangle_t *t2 = t1 + (blockDim.x * blockDim.y);

    if ((blockIdx.x * blockDim.x + blockIdx.y * blockDim.y * gridDim.x * blockDim.x) > rayCount)
        return;

    int rayPos = threadIdx.y * gridDim.x * blockDim.x + threadIdx.x + blockIdx.x * blockDim.x + blockIdx.y * blockDim.y * gridDim.x * blockDim.x;

    cuda_intersection_t inter = intrs[rayPos * (rayPos < rayCount)];
    cuda_ray_t ray = rays[rayPos * (rayPos < rayCount)];

    int shPos = threadIdx.y * blockDim.x + threadIdx.x;

    int count = triangleSize / (blockDim.x * blockDim.y);
    int i = 0;
    for (; i < (count + 1); i += 2) {
        t1[shPos] = triangles[OBJNDX * (OBJNDX < triangleSize)];
        t2[shPos] = triangles[OBJNDX2 * (OBJNDX2 < triangleSize)];

        __syncthreads();
        for (int k = 0; k < blockDim.x * blockDim.y; k++) {
            float newT = intersect(t1[k], ray);
            if (newT >= 0 && newT < inter.t) {
                inter.t = newT;
                inter.objIndx = i * blockDim.x * blockDim.y + k;
                inter.type = _TRIANGLE;
            }
            newT = intersect(t2[k], ray);
            if (newT >= 0 && newT < inter.t) {
                inter.t = newT;
                inter.objIndx = (i + 1) * blockDim.x * blockDim.y + k;
                inter.type = _TRIANGLE;
            }
            __syncthreads();
        }
    }

    if (rayPos < rayCount)
        intrs[rayPos] = inter;
}
#undef OBJNDX
#undef OBJNDX2

__host__ intersection_vec RDST::cuda_intersect(const sphere_vec &spheres,
        const triangle_vec &triangles, const ray_vec &rays, int width, int height) {
    static dim3 dimBlock(16, 16);
    static dim3 dimGrid(40, 30);
    static int batchSize = dimBlock.x * dimBlock.y * dimGrid.x * dimGrid.y;
    int count = (width * height) / batchSize;
    int spill = (width * height) % batchSize;
    intersection_vec iVec(rays.size());

    cuda_intersection_t inter;
    inter.objIndx = -1;
    inter.t = FLT_MAX;
    inter.type = _NONE;

    thrust::device_vector<cuda_sphere_t> dSpheres(spheres.begin(), spheres.end());
    cuda_sphere_t *sPtr = thrust::raw_pointer_cast(&(*dSpheres.begin()));

    thrust::device_vector<cuda_triangle_t> dTriangles(triangles.begin(), triangles.end());
    cuda_triangle_t *tPtr = thrust::raw_pointer_cast(&(*dTriangles.begin()));

    ray_vec::const_iterator rVecI = rays.begin();
    intersection_vec::iterator iVecI = iVec.begin();
    for (int i = 0 ; i < count; i++) {
        thrust::device_vector<cuda_ray_t> dRays(rVecI, rVecI + batchSize);

        thrust::device_vector<cuda_intersection_t> dIntersects(batchSize);
        thrust::fill(dIntersects.begin(), dIntersects.end(), inter);

        cuda_ray_t *rPtr = thrust::raw_pointer_cast(&(*dRays.begin()));
        cuda_intersection_t *iPtr = thrust::raw_pointer_cast(&(*dIntersects.begin()));

        if (spheres.size())
            SphereIntersectKernel<<<dimGrid, dimBlock, 3 * sizeof(cuda_sphere_t) * dimBlock.x * dimBlock.y>>>(sPtr,
                    spheres.size(), rPtr, iPtr, batchSize);
        if (triangles.size())
            TriangleIntersectKernel<<<dimGrid, dimBlock, 2 * sizeof(cuda_triangle_t) * dimBlock.x * dimBlock.y>>>(tPtr,
                    triangles.size(), rPtr, iPtr, batchSize);

        thrust::copy(dIntersects.begin(), dIntersects.end(), iVecI);
        rVecI += batchSize;
        iVecI += batchSize;
    }

    if (spill) {
        thrust::device_vector<cuda_ray_t> dRays(rVecI, rVecI + spill);

        thrust::device_vector<cuda_intersection_t> dIntersects(spill);
        thrust::fill(dIntersects.begin(), dIntersects.end(), inter);

        cuda_ray_t *rPtr = thrust::raw_pointer_cast(&(*dRays.begin()));
        cuda_intersection_t *iPtr = thrust::raw_pointer_cast(&(*dIntersects.begin()));

        if (spheres.size())
            SphereIntersectKernel<<<dimGrid, dimBlock, 3 * sizeof(cuda_sphere_t) * dimBlock.x * dimBlock.y>>>(sPtr,
                    spheres.size(), rPtr, iPtr, spill);
        if (triangles.size())
            TriangleIntersectKernel<<<dimGrid, dimBlock, 2 * sizeof(cuda_triangle_t) * dimBlock.x * dimBlock.y>>>(tPtr,
                    triangles.size(), rPtr, iPtr, spill);

        thrust::copy(dIntersects.begin(), dIntersects.end(), iVecI);
    }

    return iVec;
}

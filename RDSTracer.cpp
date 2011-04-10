
/**
 * File: RDSTracer.cpp
 * --------------------
 * The actual Ray Tracer Implementation!
 * Author: Ryan Schmitt
 */

#include "RDSTracer.h"

namespace RDST
{
   void Tracer::RayTrace(const SceneDescription& scene, Image& image)
   {
   }

   Intersection Tracer::RaySphereIntersect(const Ray& ray, SpherePtr pSphere)
   { return Intersection(); }
   Intersection Tracer::RayPlaneIntersect(const Ray& ray, PlanePtr pPlane)
   { return Intersection(); }
}
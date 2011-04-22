/**
 * RDSIntersections.h
 * --------------------
 * Intersections!
 * Author: Ryan Schmitt
 */

#ifndef __RDS_INTERSECTIONS_H__
#define __RDS_INTERSECTIONS_H__

namespace RDST
{
   //Forward Declarations
   class Intersection;
   class Plane;
   class Ray;
   class Sphere;
   class Triangle;

   //Helper Functions
   Ray TransformRay(const Ray& ray, const glm::mat4& worldToModel);

   //Intersection Functions
   Intersection* RayPlaneIntersect(const Ray& ray, const Plane& plane);
   Intersection* RaySphereIntersect(const Ray& ray, const Sphere& sphere);
   Intersection* RayTriangleIntersect(const Ray& ray, const Triangle& tri);
}

#endif
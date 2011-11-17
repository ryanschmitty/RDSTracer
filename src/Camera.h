//------------------------------------------------------------------------------
// Ryan Schmitt & Ian Stewart
// 9/30/08
// 
//
// Description:
// Basic Stationary Camera Class
//------------------------------------------------------------------------------

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "AABoundingBox.h"
#include "vec3.h"

class Camera
{
   public:
      Vec3f loc;
      Vec3f dir;
      Vec3f up;
      float fov;
      float zNear, zFar;
      AABoundingBox bounds;

      Camera(float setFOV, float setX, float setY, float setZ,
             float dx,     float dy,   float dz,
             float upx,    float upy,  float upz,
             float setzNear, float setzFar)
      {
         loc = Vec3f(setX, setY, setZ);
         dir = Vec3f(dx, dy, dz);
         up = Vec3f(upx, upy, upz);
         fov = setFOV;
         zNear = setzNear;
         zFar = setzFar;
         bounds = AABoundingBox(&loc, 2.f, 2.f, 2.f ); //Set the size of the camera for collisions
      }
};

#endif

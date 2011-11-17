//------------------------------------------------------------------------------
// Ryan Schmitt & Ian Stewart
// 9/30/08
// 
//
// Description:
// Camera capable of moving around a scene.
//------------------------------------------------------------------------------

#ifndef __FLYING_CAMERA_H__
#define __FLYING_CAMERA_H__

#include "camera.h"
#include <math.h>
#include <algorithm>

#define PI 3.14159265

class FlyingCamera : public Camera
{
   private:
      float pitch;
      float yaw;

   public:
      int xFactor;
      int yFactor;
      int zFactor;

      FlyingCamera(float setFOV, float setX, float setY, float setZ,
                   float setzNear, float setzFar) :
         Camera(setFOV, setX, setY, setZ,
                0.f, 0.f, -1.f,
                0, 1.f,  0,
                setzNear, setzFar),
         yaw(atan2f(dir.z, dir.x) * 180 / PI),
         pitch(atan2f(dir.y, sqrtf((dir.x * dir.x) + (dir.z * dir.z))) * 180 / PI),
         xFactor(1), yFactor(1), zFactor(1)
      {
      }

      //------------------------------------------------------------------------
      // Allow movement in the x, y, and z axes?

      void allowX(bool val)
      {
         xFactor = val?1:0;
      }

      void allowY(bool val)
      {
         yFactor = val?1:0;
      }

      void allowZ(bool val)
      {
         zFactor = val?1:0;
      }

      //------------------------------------------------------------------------
      // Movement Functions

      void moveForward(float delta)
      {
         loc.x += delta*dir.x*xFactor;
         loc.y += delta*dir.y*yFactor;
         loc.z += delta*dir.z*zFactor;
      }

      void moveBackward(float delta)
      {
         loc.x -= delta*dir.x*xFactor;
         loc.y -= delta*dir.y*yFactor;
         loc.z -= delta*dir.z*zFactor;
      }

      void moveLeft(float delta)
      {
         Vec3f v = dir.cross(up);
         loc.x -= delta*v.x;
         loc.y -= delta*v.y;
         loc.z -= delta*v.z;
      }

      void moveRight(float delta)
      {
         Vec3f v = dir.cross(up);
         loc.x += delta*v.x;
         loc.y += delta*v.y;
         loc.z += delta*v.z;
      }

      //------------------------------------------------------------------------
      // Adjust the pitch and yaw

      void setPitch(float delta)
      {
         //Clamp to 90 and -90
         pitch = std::max(-89.f, std::min(89.f, pitch+delta));
         updateDir();
      }

      void setYaw(float delta) {
         yaw += delta;
         updateDir();
      }

   protected:

      //------------------------------------------------------------------------
      // Update the forward direction based on pitch and yaw.

      void updateDir()
      {
         dir.x = cos(yaw*PI/180)*cos(pitch*PI/180);
         dir.y = sin(pitch*PI/180);
         dir.z = sin((yaw)*PI/180)*cos(pitch*PI/180);
      }

};

#endif

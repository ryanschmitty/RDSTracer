//------------------------------------------------------------------------------
// Ryan Schmitt & Ian Stewart
// 9/30/08
// 
//
// Description:
// Axis Aligned Bounding Box
//------------------------------------------------------------------------------

#ifndef __AA_BOUNDING_BOX_H__
#define __AA_BOUNDING_BOX_H__

#include <GLUT/glut.h>
#include "vec3.h"

class AABoundingBox
{

public:

   AABoundingBox() {};
   AABoundingBox(Vec3f *in_loc, float in_width, float in_height, float in_depth) :
      loc(in_loc), halfWidth(in_width/2), halfHeight(in_height/2), halfDepth(in_depth/2) {};
   ~AABoundingBox() {};

   Vec3f getCenter() const;
   float getMaxX() const;
   float getMinX() const;
   float getMaxY() const;
   float getMinY() const;
   float getMaxZ() const;
   float getMinZ() const;

   float getWidth() const;
   void setWidth(float);
   float getHeight() const;
   void setHeight(float);
   float getDepth() const;
   void setDepth(float);

   // collision test for the XZ coordinate ignoring Y
   bool intersectsXZ(AABoundingBox*) const;

   void draw();

protected:

   //POINTER to this bounding box's BasicModel location
   Vec3f *loc;

   float halfWidth;
   float halfHeight;
   float halfDepth;

};

#endif

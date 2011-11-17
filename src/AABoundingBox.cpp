#include "AABoundingBox.h"

//-----------------------------------------------------------------------------
// Center point of the AA Bounding Box
Vec3f AABoundingBox::getCenter() const
{
   return Vec3f(loc->x, loc->y, loc->z);
}

//-----------------------------------------------------------------------------
// Max/min x value
float AABoundingBox::getMaxX() const
{
   return loc->x + halfWidth;
}
float AABoundingBox::getMinX() const
{
   return loc->x - halfWidth;
}

//-----------------------------------------------------------------------------
// Max/min y value
float AABoundingBox::getMaxY() const
{
   return loc->y + halfHeight;
}
float AABoundingBox::getMinY() const
{
   return loc->y - halfHeight;
}

//-----------------------------------------------------------------------------
// Max/min z value
float AABoundingBox::getMaxZ() const
{
   return loc->z + halfDepth;
}
float AABoundingBox::getMinZ() const
{
   return loc->z - halfDepth;
}

//-----------------------------------------------------------------------------
// Width getter/setter
float AABoundingBox::getWidth() const
{
   return halfWidth*2.0;
}
void AABoundingBox::setWidth(float wid)
{
   halfWidth = wid/2.0;
}

//-----------------------------------------------------------------------------
// Height getter/setter
float AABoundingBox::getHeight() const
{
   return halfHeight*2.0;
}
void AABoundingBox::setHeight(float hei)
{
   halfHeight = hei/2.0;
}

//-----------------------------------------------------------------------------
// Depth getter/setter
float AABoundingBox::getDepth() const
{
   return halfDepth*2.0;
}
void AABoundingBox::setDepth(float dep)
{
   halfDepth = dep/2;
}
//-----------------------------------------------------------------------------
// collision test for the XZ coordinate ignoring Y
bool AABoundingBox::intersectsXZ(AABoundingBox *b) const
{
   // overlapping X-coords?
   if (getMaxX() >= b->getMinX() && b->getMaxX() >= getMinX() && getMaxZ() >= b->getMinZ() && b->getMaxZ() >= getMinZ()) {
      // no need to check y-coords
      return true; // HIT
   }
   return false;
}

//-----------------------------------------------------------------------------
// Draws the BoundingBox
void AABoundingBox::draw()
{
   glPushMatrix();

      // TOP
      glBegin(GL_LINE_STRIP);
         glVertex3f(getMaxX(), getMaxY(), getMaxZ());
         glVertex3f(getMinX(), getMaxY(), getMaxZ());
         glVertex3f(getMinX(), getMaxY(), getMinZ());
         glVertex3f(getMaxX(), getMaxY(), getMinZ());
         glVertex3f(getMaxX(), getMaxY(), getMaxZ());
      glEnd();

      // BOTTOM
      glBegin(GL_LINE_STRIP);
         glVertex3f(getMaxX(), getMinY(), getMaxZ());
         glVertex3f(getMinX(), getMinY(), getMaxZ());
         glVertex3f(getMinX(), getMinY(), getMinZ());
         glVertex3f(getMaxX(), getMinY(), getMinZ());
         glVertex3f(getMaxX(), getMinY(), getMaxZ());
      glEnd();

      glBegin(GL_LINES);
         //BACK RIGHT
         glVertex3f(getMaxX(), getMaxY(), getMaxZ());
         glVertex3f(getMaxX(), getMinY(), getMaxZ());
         //BACK LEFT
         glVertex3f(getMinX(), getMaxY(), getMaxZ());
         glVertex3f(getMinX(), getMinY(), getMaxZ());
         //FRONT LEFT
         glVertex3f(getMinX(), getMaxY(), getMinZ());
         glVertex3f(getMinX(), getMinY(), getMinZ());
         //FRONT RIGHT
         glVertex3f(getMaxX(), getMaxY(), getMinZ());
         glVertex3f(getMaxX(), getMinY(), getMinZ());
      glEnd();

   glPopMatrix();
}


/**
 * RDSScene.h
 * --------------------
 * A collection of useful objects
 * for representing a scene.
 * Author: Ryan Schmitt
 */

#ifndef __RDS_SCENE_H__
#define __RDS_SCENE_H__

#define GLM_FORCE_INLINE

#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include "RDSBBox.h"
#include "RandUtil.h"

namespace RDST
{
   static const float PI = 3.14159265f;
   //---------------------------------------------------------------------------
   //
   // PARENT CLASSES
   //
   //---------------------------------------------------------------------------

   //---------------------------------------------------------------------------
   // Class the provides color
   //---------------------------------------------------------------------------
   class Colored
   {
   public:
      explicit Colored(const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f))
      : _color(color)
      {}
      virtual ~Colored() = 0;
      
      //Color
      const glm::vec4& getColor() const
      { return _color; }
      void setColor(const glm::vec4& color)
      { _color = color; }

   private:
      glm::vec4 _color; //rgba
   };
   typedef boost::shared_ptr<Colored> ColoredPtr;
   typedef boost::shared_ptr<const Colored> ConstColoredPtr;

   //---------------------------------------------------------------------------
   //
   // NON-GEOMETRIC OBJECT SCENE CLASSES
   //
   //---------------------------------------------------------------------------

   //---------------------------------------------------------------------------
   // Camera class
   //---------------------------------------------------------------------------
   class Camera
   {
   public:
      explicit Camera(const glm::vec3& position = glm::vec3(0.f, 0.f, 0.f),
                      const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f),
                      const glm::vec3& right = glm::vec3(1.333f, 0.f, 0.f),
                      const glm::vec3& dir = glm::vec3(0.f, 0.f, -1.f))
      : _position(position),
        _dir(glm::normalize(dir))
      {
         //WARN: be wary of looking straight up or straight down, it will end in disaster!
         glm::vec3 k(0.f, 1.f, 0.f);
         _up = glm::normalize(k - ((glm::dot(k, _dir))*_dir)); //Gram-Schmidt Orthogonalization
         _right = -glm::cross(_up, _dir)*glm::length(right);
      }

      //Position
      const glm::vec3& getPos() const
      { return _position; }
      void setPos(const glm::vec3& position)
      { _position = position; }

      //Up Vector
      const glm::vec3& getUp() const
      { return _up; }
      void setUp(const glm::vec3& up)
      { _up = glm::normalize(up); }

      //Right Vector
      const glm::vec3& getRight() const
      { return _right; }
      void setRight(const glm::vec3& right)
      { _right = right; }

      //Direction Vector
      const glm::vec3& getDir() const
      { return _dir; }
      void setLookAt(const glm::vec3& dir)
      { _dir = glm::normalize(dir); }

   private:
      glm::vec3 _position;
      glm::vec3 _up;
      glm::vec3 _right;
      glm::vec3 _dir;
   };
   typedef boost::shared_ptr<Camera> CameraPtr;
   typedef boost::shared_ptr<const Camera> ConstCameraPtr;

   //---------------------------------------------------------------------------
   // Point Light class
   //---------------------------------------------------------------------------
   class PointLight: public Colored
   {
   public:
      explicit PointLight(const glm::vec3& position = glm::vec3(0.f),
                          const glm::vec4& color = glm::vec4(1.f))
      : Colored(color),
        _position(position)
      {}

      //Position
      const glm::vec3& getPos() const
      { return _position; }
      void setPos(const glm::vec3& position)
      { _position = position; }

   private:
      glm::vec3 _position;
   };
   typedef boost::shared_ptr<PointLight> PointLightPtr;
   typedef boost::shared_ptr<const PointLight> ConstPointLightPtr;
   typedef boost::shared_ptr< std::vector<PointLightPtr> > PointLightPtrListPtr;

   //---------------------------------------------------------------------------
   //
   // GEOMETRIC OBJECT PROPERTY CLASSES
   //
   //---------------------------------------------------------------------------

   //---------------------------------------------------------------------------
   // Surface Finish class
   //---------------------------------------------------------------------------
   class Finish
   {
   public:
      explicit Finish(float ambient = 0.f,
                      float diffuse = 0.f,
                      float emissive = 0.f,
                      float specular = 0.f,
                      float roughness = 0.f,
                      float reflection = 0.f,
                      float refraction = 0.f,
                      float indexOfRefraction = 0.f)
      : _ambient(ambient),
        _diffuse(diffuse),
        _emissive(emissive),
        _specular(specular),
        _roughness(roughness),
        _reflection(reflection),
        _refraction(refraction),
        _ior(indexOfRefraction)
      {}

      //Ambient
      float getAmbient() const
      { return _ambient; }
      void setAmbient(float ambient)
      { _ambient = ambient; }

      //Diffuse
      float getDiffuse() const
      { return _diffuse; }
      void setDiffuse(float diffuse)
      { _diffuse = diffuse; }

      //Emissive
      float getEmissive() const
      { return _emissive; }
      void setEmissive(float emissive)
      { _emissive = emissive; }

      //Specular
      float getSpecular() const
      { return _specular; }
      void setSpecular(float specular)
      { _specular = specular; }

      //Roughness
      float getRoughness() const
      { return _roughness; }
      void setRoughness(float roughness)
      { _roughness = roughness; }

      //Reflection
      float getReflection() const
      { return _reflection; }
      void setReflection(float reflection)
      { _reflection = reflection; }

      //Refraction
      float getRefraction() const
      { return _refraction; }
      void setRefraction(float refraction)
      { _refraction = refraction; }

      //Index of Refraction
      float getIndexOfRefraction() const
      { return _ior; }
      void setIndexOfRefraction(float ior)
      { _ior = ior; }

   private:
      float _ambient;
      float _diffuse;
      float _emissive;
      float _specular;
      float _roughness; //size of specular highlight
      float _reflection;
      float _refraction;
      float _ior; //index of refraction
   };

   //---------------------------------------------------------------------------
   //
   // INTERSECTION CLASSES
   //
   //---------------------------------------------------------------------------
   
   //---------------------------------------------------------------------------
   // Ray class
   //---------------------------------------------------------------------------
   class Ray
   {
   public:
      explicit Ray(const glm::vec3& direction,
                   const glm::vec3& origin = glm::vec3(0.f),
                   float minT = 0.f,
                   float maxT = FLT_MAX,
                   float _weight = 0.f)
      : d(direction),
        o(origin),
        tCur(FLT_MAX),
        tMin(minT),
        tMax(maxT),
        weight(_weight)
      {}
      glm::vec3 d, o;
      float tCur;
      float tMin;
      float tMax;
      float weight;
   };
   typedef boost::shared_ptr<Ray> RayPtr;
   typedef boost::shared_ptr<const Ray> ConstRayPtr;
   typedef boost::shared_ptr< std::vector<Ray> > RayListPtr;

   //---------------------------------------------------------------------------
   // Surface class combines Finish and Color
   //---------------------------------------------------------------------------
   class Surface
   {
   public:
      explicit Surface(const glm::vec4& _color = glm::vec4(1.f), const Finish& _finish = Finish())
      : color(_color),
        finish(_finish)
      {}
      glm::vec4 color;
      Finish finish;
   };

   //---------------------------------------------------------------------------
   // Intersection information container class
   //---------------------------------------------------------------------------
   class Intersection
   {
   public:
      explicit Intersection()
      : hit(false),
        t(FLT_MAX),
        p(glm::vec3(0.f)),
        n(glm::vec3(0.f)),
        incDir(glm::vec3(0.f)),
        surf(Surface()),
        inside(false)
      {}
      explicit Intersection(bool hit, float hitT, const glm::vec3& incomingDir, const glm::vec3& hitPoint, const glm::vec3& normal, const Surface& surface, bool rayInsideObject)
      : hit(hit),
        t(hitT),
        p(hitPoint),
        n(normal),
        incDir(incomingDir),
        surf(surface),
        inside(rayInsideObject)
      {}
      bool hit;
      float t;
      glm::vec3 p;
      glm::vec3 n;
      glm::vec3 incDir;
      Surface surf;
      bool inside;
   };
   typedef boost::shared_ptr<Intersection> IntersectionPtr;
   typedef boost::shared_ptr<const Intersection> ConstIntersectionPtr;
   typedef boost::shared_ptr< std::vector<IntersectionPtr> > IntersectionPtrListPtr;

   //---------------------------------------------------------------------------
   //
   // GEOMETRIC OBJECT CLASSES
   //
   //---------------------------------------------------------------------------

   //---------------------------------------------------------------------------
   // Abstract Geometric Object class
   //---------------------------------------------------------------------------
   class GeomObject: public Colored
   {
   public:
      explicit GeomObject(const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f),
                          const glm::mat4& modelXform = glm::mat4(1.f),
                          const Finish& finish = Finish())
      : Colored(color),
        _modelXform(modelXform),
        _inverse(glm::inverse(modelXform)),
        _transposedAdjoint(glm::transpose(adjoint(glm::mat3(modelXform)))),
        _finish(finish),
        _bbox(BBox())
      {}
      virtual ~GeomObject() = 0;

      //Model Transformations
      const glm::mat4& getModelXform() const
      { return _modelXform; }
      const glm::mat4& getModelInverse() const
      { return _inverse; }
      const glm::mat3& getNormalXform() const
      { return _transposedAdjoint; }
      void setModelXform(const glm::mat4& modelXform) {
         _modelXform = modelXform;
         _inverse = glm::inverse(modelXform);
         _transposedAdjoint = glm::transpose(adjoint(glm::mat3(modelXform)));
      }
      Ray transformRay(const Ray& ray) const;

      //Finish
      const Finish& getFinish() const
      { return _finish; }
      void setFinish(const Finish& finish)
      { _finish = finish; }

      //Intersection
      virtual Intersection* intersect(const Ray& ray) const = 0;

      //Bounds
      const BBox& getWorldBounds() const
      { return _bbox; }
      BBox& getWorldBounds()
      { return _bbox; }

      //Surface Area
      virtual float getSurfaceArea() const = 0;

      //Uniform Sample Point
      virtual glm::vec3 uniformSample(float u1, float u2) const
      { return glm::vec3(0.f); }
      
      //Stratefied Sample Points
      boost::shared_ptr< std::vector<glm::vec3> > stratefiedSamples(bool rand, int numSamples) const;

   private:
      //functions
      glm::mat3 adjoint(const glm::mat3& m) const;
      //data
      glm::mat4 _modelXform;
      glm::mat4 _inverse;
      glm::mat3 _transposedAdjoint;
      Finish    _finish;
   protected:
      BBox      _bbox;
   };
   typedef boost::shared_ptr<GeomObject> GeomObjectPtr;
   typedef boost::shared_ptr<const GeomObject> ConstGeomObjectPtr;
   typedef boost::shared_ptr< std::vector<GeomObjectPtr> > GeomObjectPtrListPtr;

   //---------------------------------------------------------------------------
   // Box class
   //---------------------------------------------------------------------------
   class Box: public GeomObject
   {
   public:
      explicit Box(const glm::vec3& smallCorner = glm::vec3(-1.f, -1.f, -1.f),
                   const glm::vec3& largeCorner = glm::vec3(1.f, 1.f, 1.f),
                   const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f),
                   const glm::mat4& modelXform = glm::mat4(1.f),
                   const Finish& finish = Finish())

      : GeomObject(color, modelXform, finish),
        _smCorner(glm::min(smallCorner, largeCorner)),
        _lgCorner(glm::max(smallCorner, largeCorner))
      {
         glm::vec4 corner1 = modelXform*glm::vec4(_smCorner,1.f);
         glm::vec4 corner2 = modelXform*glm::vec4(_lgCorner.x, _smCorner.y, _smCorner.z, 1.f);
         glm::vec4 corner3 = modelXform*glm::vec4(_lgCorner.x, _smCorner.y, _lgCorner.z, 1.f);
         glm::vec4 corner4 = modelXform*glm::vec4(_smCorner.x, _smCorner.y, _smCorner.z, 1.f);
         glm::vec4 corner5 = modelXform*glm::vec4(_smCorner.x, _lgCorner.y, _lgCorner.z, 1.f);
         glm::vec4 corner6 = modelXform*glm::vec4(_smCorner.x, _lgCorner.y, _smCorner.z, 1.f);
         glm::vec4 corner7 = modelXform*glm::vec4(_lgCorner.x, _lgCorner.y, _smCorner.z, 1.f);
         glm::vec4 corner8 = modelXform*glm::vec4(_lgCorner.x, _lgCorner.y, _lgCorner.z, 1.f);
         _bbox = BBox(glm::vec3(corner1));
         _bbox.include(glm::vec3(corner2));
         _bbox.include(glm::vec3(corner3));
         _bbox.include(glm::vec3(corner4));
         _bbox.include(glm::vec3(corner5));
         _bbox.include(glm::vec3(corner6));
         _bbox.include(glm::vec3(corner7));
         _bbox.include(glm::vec3(corner8));
      }

      //Corners of a box (corner 1 must be less than corner 2 in all elements)
      const glm::vec3& getSmallCorner() const
      { return _smCorner; }
      const glm::vec3& getLargeCorner() const
      { return _lgCorner; }

      void setDimensions(const glm::vec3& smallCorner, const glm::vec3& largeCorner) {
         _smCorner = smallCorner; _lgCorner = largeCorner;
         _bbox = BBox(glm::vec3(getModelXform()*glm::vec4(smallCorner,1.f)), glm::vec3(getModelXform()*glm::vec4(largeCorner,1.f)));
      }

      //Intersection
      Intersection* intersect(const Ray& ray) const;

      //Surface Area
      float getSurfaceArea() const {
         glm::vec3 d = _lgCorner - _smCorner;
         return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
      }

   private:
      glm::vec3 _smCorner;
      glm::vec3 _lgCorner;
   };
   typedef boost::shared_ptr<Box> BoxPtr;
   typedef boost::shared_ptr<const Box> ConstBoxPtr;

   //---------------------------------------------------------------------------
   // Cone class
   //---------------------------------------------------------------------------
   class Cone: public GeomObject
   {
   public:
      explicit Cone(const glm::vec3& end1 = glm::vec3(0.f, 0.f, 0.f),
                    float radius1 = 1.f,
                    const glm::vec3& end2 = glm::vec3(0.f, 1.f, 0.f),
                    float radius2 = 0.f,
                    const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f),
                    const glm::mat4& modelXform = glm::mat4(1.f),
                    const Finish& finish = Finish())
      : GeomObject(color, modelXform, finish),
        _end1(end1),
        _end2(end2),
        _radius1(radius1),
        _radius2(radius2)
      {}

      //Center for ends
      const glm::vec3& getEnd1() const
      { return _end1; }
      const glm::vec3& getEnd2() const
      { return _end2; }

      //radii
      float getRadius1() const
      { return _radius1; }
      float getRadius2() const
      { return _radius2; }

      void setDimensions(const glm::vec3& end1, float radius1, const glm::vec3& end2, float radius2)
      { _end1 = end1; _end2 = end2; _radius1 = radius1; _radius2 = radius2; }

      //Intersection
      Intersection* intersect(const Ray& ray) const
      { return NULL; }

      //Surface Area
      float getSurfaceArea() const
      { return 0.f; }

   private:
      glm::vec3 _end1; //center for end 1
      glm::vec3 _end2; //center for end 2
      float     _radius1;
      float     _radius2;
   };
   typedef boost::shared_ptr<Cone> ConePtr;
   typedef boost::shared_ptr<const Cone> ConstConePtr;

   //---------------------------------------------------------------------------
   // Plane class
   //---------------------------------------------------------------------------
   class Plane: public GeomObject
   {
   public:
      explicit Plane(const glm::vec3& normal = glm::vec3(0.f, 1.f, 0.f),
                     float distance = 0.f,
                     const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f),
                     const glm::mat4& modelXform = glm::mat4(1.f),
                     const Finish& finish = Finish())
      : GeomObject(color, modelXform, finish),
        _normal(normal),
        _distance(distance)
      {}

      //Normal and Distance define a plane
      const glm::vec3& getNormal() const
      { return _normal; }
      float getDistance() const
      { return _distance; }

      void setDimensions(const glm::vec3& normal, float distance)
      { _normal = normal; _distance = distance; }

      //Intersection
      Intersection* intersect(const Ray& ray) const;

      float getSurfaceArea() const
      { return FLT_MAX; }

   private:
      glm::vec3 _normal;
      float     _distance;
   };
   typedef boost::shared_ptr<Plane> PlanePtr;
   typedef boost::shared_ptr<const Plane> ConstPlanePtr;
   typedef boost::shared_ptr< std::vector<PlanePtr> > PlanePtrListPtr;

   //---------------------------------------------------------------------------
   // Sphere class
   //---------------------------------------------------------------------------
   class Sphere: public GeomObject
   {
   public:
      explicit Sphere(const glm::vec3& center = glm::vec3(0.f, 0.f, 0.f),
                      float radius = 1.f,
                      const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f),
                      const glm::mat4& modelXform = glm::mat4(1.f),
                      const Finish& finish = Finish())
      : GeomObject(color, modelXform, finish),
        _center(center),
        _radius(radius),
        _radiusSquared(radius*radius)
      {
         Box b = Box(center+glm::vec3(-radius), center+glm::vec3(radius), glm::vec4(1.f), modelXform);
         _bbox = b.getWorldBounds();
      }

      //Center and radius define a sphere
      const glm::vec3& getCenter() const
      { return _center; }
      float getRadius() const
      { return _radius; }
      float getRadiusSquared() const
      { return _radiusSquared; }

      void setDimensions(const glm::vec3& center, float radius) {
         _center = center; _radius = radius; _radiusSquared = radius*radius;
        _bbox = BBox(glm::vec3(getModelXform()*glm::vec4(center,1.f))); _bbox.expand(radius);
      }

      //Intersection
      Intersection* intersect(const Ray& ray) const;

      //Surface Area
      float getSurfaceArea() const
      { return 4.f * PI * _radiusSquared; }

      //Uniform Sample Point
      glm::vec3 uniformSample(float u1, float u2) const;

   private:
      glm::vec3 unifUnitSample(float u1, float u2) const;
      glm::vec3 _center;
      float     _radius, _radiusSquared;
   };
   typedef boost::shared_ptr<Sphere> SpherePtr;
   typedef boost::shared_ptr<const Sphere> ConstSpherePtr;

   //---------------------------------------------------------------------------
   // Triangle class
   //---------------------------------------------------------------------------
   class Triangle: public GeomObject
   {
   public:
      explicit Triangle(const glm::vec3& vertex0 = glm::vec3(0.f, 0.f, 1.f),
                        const glm::vec3& vertex1 = glm::vec3(0.f, 1.f, 0.f),
                        const glm::vec3& vertex2 = glm::vec3(1.f, 0.f, 0.f),
                        const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f),
                        const glm::mat4& modelXform = glm::mat4(1.f),
                        const Finish& finish = Finish())
      : GeomObject(color, modelXform, finish),
        _vert0(vertex0),
        _vert1(vertex1),
        _vert2(vertex2),
        _normal(glm::normalize(glm::cross(vertex1-vertex0, vertex2-vertex0)))
      {
         glm::vec4 max = glm::max(glm::max(modelXform*glm::vec4(vertex0, 1.f), modelXform*glm::vec4(vertex1,1.f)), modelXform*glm::vec4(vertex2,1.f));
         glm::vec4 min = glm::min(glm::min(modelXform*glm::vec4(vertex0, 1.f), modelXform*glm::vec4(vertex1,1.f)), modelXform*glm::vec4(vertex2,1.f));
         _bbox = BBox(glm::vec3(min), glm::vec3(max));
      }

      //Vertices
      const glm::vec3& getVertex0() const
      { return _vert0; }
      const glm::vec3& getVertex1() const
      { return _vert1; }
      const glm::vec3& getVertex2() const
      { return _vert2; }

      const glm::vec3& getNormal() const
      { return _normal; }

      void setDimensions(const glm::vec3& vertex0, const glm::vec3& vertex1, const glm::vec3& vertex2)
      {
         _vert0 = vertex0; _vert1 = vertex1; _vert2 = vertex2; _normal = glm::normalize(glm::cross(_vert1-_vert0, _vert2-_vert0));
         glm::vec3 max = glm::max(glm::max(vertex0, vertex1), vertex2);
         glm::vec3 min = glm::min(glm::min(vertex0, vertex1), vertex2);
         _bbox = BBox(glm::vec3(getModelXform()*glm::vec4(min,1.f)), glm::vec3(getModelXform()*glm::vec4(max,1.f)));
      }

      //Intersection
      Intersection* intersect(const Ray& ray) const;

      //Surface Area
      float getSurfaceArea() const
      { return 0.5f * glm::length(glm::cross(_vert1-_vert0, _vert2-_vert0)); }

      //Uniform Sample Point
      glm::vec3 uniformSample(float rand1, float rand2) const;

   private:
      glm::vec3 _vert0;
      glm::vec3 _vert1;
      glm::vec3 _vert2;
      glm::vec3 _normal;
   };
   typedef boost::shared_ptr<Triangle> TrianglePtr;
   typedef boost::shared_ptr<const Triangle> ConstTrianglePtr;

} // end namespace RDST

#endif

/**
 * RDSScene.h
 * --------------------
 * A collection of useful objects
 * for representing a scene.
 * Author: Ryan Schmitt
 */

#ifndef __RDS_SCENE_H__
#define __RDS_SCENE_H__

#include <vector>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>

namespace RDST
{
   /**
    * Abstract Base Object class for EVERYTHING
    */
   //Type enum
   enum OBJ_TYPE {CAMERA, LIGHT, BOX, CONE, PLANE, SPHERE, TRIANGLE};
   class SceneObject
   {
   public:
      explicit SceneObject()
      {}
      virtual ~SceneObject() = 0;

      //Abstract type identifier function
      virtual OBJ_TYPE getType() const = 0;
   };
   typedef boost::shared_ptr<SceneObject> SceneObjectPtr;

   /**
    * Abstract Base Colored Object class
    */
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

   /**
    * Camera Storage Class.
    * Note: Normalizes on set for Up and LookAt vectors.
    */
   class Camera: public SceneObject
   {
   public:
      explicit Camera(const glm::vec3& position = glm::vec3(0.f, 0.f, 0.f),
                      const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f),
                      const glm::vec3& right = glm::vec3(1.333f, 0.f, 0.f),
                      const glm::vec3& dir = glm::vec3(0.f, 0.f, -1.f))
      : SceneObject(),
        _position(position),
        _up(glm::normalize(up)),
        _right(right),
        _dir(glm::normalize(dir))
      {}

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

      OBJ_TYPE getType() const
      { return CAMERA; }

   private:
      glm::vec3 _position;
      glm::vec3 _up;
      glm::vec3 _right;
      glm::vec3 _dir;
   };
   typedef boost::shared_ptr<Camera> CameraPtr;

   /**
    * Point Light Storage Class
    */
   class PointLight: public Colored, public SceneObject
   {
   public:
      explicit PointLight(const glm::vec3& position = glm::vec3(0.f),
                          const glm::vec4& color = glm::vec4(1.f))
      : Colored(color),
        SceneObject(),
        _position(position)
      {}

      //Position
      const glm::vec3& getPos() const
      { return _position; }
      void setPos(const glm::vec3& position)
      { _position = position; }

      OBJ_TYPE getType() const
      { return LIGHT; }

   private:
      glm::vec3 _position;
   };
   typedef boost::shared_ptr<PointLight> PointLightPtr;

   /**
    * Surface Finish Storage Class
    */
   class Finish
   {
   public:
      explicit Finish(float ambient = 0.f,
                      float diffuse = 0.f,
                      float specular = 0.f,
                      float roughness = 0.f,
                      float reflection = 0.f,
                      float refraction = 0.f,
                      float indexOfRefraction = 0.f)
      : _ambient(ambient),
        _diffuse(diffuse),
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
      float _specular;
      float _roughness; //size of specular highlight
      float _reflection;
      float _refraction;
      float _ior; //index of refraction
   };

   /**
    * Abstract Base Geometric Object Storage Class
    */
   class GeomObject: public Colored, public SceneObject
   {
   public:
      explicit GeomObject(const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f),
                          const glm::mat4& modelXform = glm::mat4(1.f),
                          const Finish& finish = Finish())
      : Colored(color),
        _modelXform(modelXform),
        _finish(finish)
      {}
      virtual ~GeomObject() = 0;

      //Model Transformations
      const glm::mat4& getModelXform() const
      { return _modelXform; }
      const glm::mat3& getTransposedInverse() const
      { return _transposedInverse; }
      const glm::mat3& getTransposedAdjoint() const
      { return _transposedAdjoint; }
      void setModelXform(const glm::mat4& modelXform) {
         _modelXform = modelXform;
         _transposedInverse = glm::transpose(glm::inverse(glm::mat3(modelXform)));
         _transposedAdjoint = glm::transpose(Adjoint(glm::mat3(modelXform)));
      }

      //Finish
      const Finish& getFinish() const
      { return _finish; }
      void setFinish(const Finish& finish)
      { _finish = finish; }

   private:
      static glm::mat3 Adjoint(const glm::mat3& m) {
         float d00 = (m[1][1]*m[2][2]) - (m[2][1]*m[1][2]);
         float d01 = (m[0][1]*m[2][2]) - (m[2][1]*m[0][2]);
         float d02 = (m[0][1]*m[1][2]) - (m[1][1]*m[0][2]);
         float d10 = (m[1][0]*m[2][2]) - (m[2][0]*m[1][2]);
         float d11 = (m[0][0]*m[2][2]) - (m[2][0]*m[0][2]);
         float d12 = (m[0][0]*m[1][2]) - (m[1][0]*m[0][2]);
         float d20 = (m[1][0]*m[2][1]) - (m[2][0]*m[1][1]);
         float d21 = (m[0][0]*m[2][1]) - (m[2][0]*m[0][1]);
         float d22 = (m[0][0]*m[1][1]) - (m[1][0]*m[0][1]);
         return glm::mat3 (d00, -d01, d02, -d10, d11, -d12, d20, -d21, d22);
      }

      glm::mat4 _modelXform;
      glm::mat3 _transposedInverse;
      glm::mat3 _transposedAdjoint;
      Finish    _finish;
   };
   typedef boost::shared_ptr<GeomObject> GeomObjectPtr;

   /**
    * Box Geometric Object Storage Class
    */
   class Box: public GeomObject
   {
   public:
      explicit Box(const glm::vec3& smallCorner = glm::vec3(-1.f, -1.f, -1.f),
                   const glm::vec3& largeCorner = glm::vec3(1.f, 1.f, 1.f),
                   const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f),
                   const glm::mat4& modelXform = glm::mat4(1.f),
                   const Finish& finish = Finish())

      : GeomObject(color, modelXform, finish),
        _smCorner(smallCorner),
        _lgCorner(largeCorner)
      {}

      //Corners of a box (corner 1 must be less than corner 2 in all elements)
      const glm::vec3& getSmallCorner() const
      { return _smCorner; }
      const glm::vec3& getLargeCorner() const
      { return _lgCorner; }

      void setDimensions(const glm::vec3& smallCorner, const glm::vec3& largeCorner)
      { _smCorner = smallCorner; _lgCorner = largeCorner; }

      OBJ_TYPE getType() const
      { return BOX; }

   private:
      glm::vec3 _smCorner;
      glm::vec3 _lgCorner;
   };
   typedef boost::shared_ptr<Box> BoxPtr;

   /**
    * Cone Geometric Object Storage Class
    */
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

      OBJ_TYPE getType() const
      { return CONE; }

   private:
      glm::vec3 _end1; //center for end 1
      glm::vec3 _end2; //center for end 2
      float     _radius1;
      float     _radius2;
   };
   typedef boost::shared_ptr<Cone> ConePtr;

   /**
    * Plane Geometric Object Storage Class
    */
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

      OBJ_TYPE getType() const
      { return PLANE; }

   private:
      glm::vec3 _normal;
      float     _distance;
   };
   typedef boost::shared_ptr<Plane> PlanePtr;

   /**
    * Sphere Geometric Object Storage Class
    */
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
      {}

      //Center and radius define a sphere
      const glm::vec3& getCenter() const
      { return _center; }
      float getRadius() const
      { return _radius; }
      float getRadiusSquared() const
      { return _radiusSquared; }

      void setDimensions(const glm::vec3& center, float radius)
      { _center = center; _radius = radius; _radiusSquared = radius*radius; }

      OBJ_TYPE getType() const
      { return SPHERE; }

   private:
      glm::vec3 _center;
      float     _radius, _radiusSquared;
   };
   typedef boost::shared_ptr<Sphere> SpherePtr;

   /**
    * Triangle Geometric Object Storage Class
    */
   class Triangle: public GeomObject
   {
   public:
      explicit Triangle(const glm::vec3& vertex1 = glm::vec3(0.f, 0.f, 1.f),
                        const glm::vec3& vertex2 = glm::vec3(0.f, 1.f, 0.f),
                        const glm::vec3& vertex3 = glm::vec3(1.f, 0.f, 0.f),
                        const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f),
                        const glm::mat4& modelXform = glm::mat4(1.f),
                        const Finish& finish = Finish())
      : GeomObject(color, modelXform, finish),
        _vert1(vertex1),
        _vert2(vertex2),
        _vert3(vertex3)
      {}

      //Vertices
      const glm::vec3& getVertex1() const
      { return _vert1; }
      const glm::vec3& getVertex2() const
      { return _vert2; }
      const glm::vec3& getVertex3() const
      { return _vert3; }

      void setDimensions(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3)
      { _vert1 = vertex1; _vert2 = vertex2; _vert3 = vertex3; }

      OBJ_TYPE getType() const
      { return TRIANGLE; }

   private:
      glm::vec3 _vert1;
      glm::vec3 _vert2;
      glm::vec3 _vert3;
   };
   typedef boost::shared_ptr<Triangle> TrianglePtr;

   /**
    * Package class for all data required for a scene.
    */
   class SceneDescription
   {
   public:
      explicit SceneDescription()
      : pCam(boost::shared_ptr<Camera>()),
      lights(std::vector<PointLightPtr>()),
      objs(std::vector<GeomObjectPtr>())
      {}
      explicit SceneDescription(CameraPtr pCamera, std::vector<PointLightPtr> lights, std::vector<GeomObjectPtr> geometryObjects)
      : pCam(pCamera),
        lights(lights),
        objs(geometryObjects)
      {}
      CameraPtr pCam;
      std::vector<PointLightPtr> lights;
      std::vector<GeomObjectPtr> objs;
   };
} // end namespace RDST

#endif
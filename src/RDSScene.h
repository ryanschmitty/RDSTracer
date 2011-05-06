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

#include <vector>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>

namespace RDST
{
   //Object Type enum
   enum OBJ_TYPE {CAMERA, LIGHT, BOX, CONE, PLANE, SPHERE, TRIANGLE};

   //---------------------------------------------------------------------------
   //
   // PARENT CLASSES
   //
   //---------------------------------------------------------------------------

   /**
    * Abstract Base Object class for EVERYTHING
    */
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
   typedef boost::shared_ptr<const SceneObject> ConstSceneObjectPtr;

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
   typedef boost::shared_ptr<const Colored> ConstColoredPtr;

   //---------------------------------------------------------------------------
   //
   // NON-GEOMETRIC OBJECT SCENE CLASSES
   //
   //---------------------------------------------------------------------------

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
   typedef boost::shared_ptr<const Camera> ConstCameraPtr;

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
   typedef boost::shared_ptr<const PointLight> ConstPointLightPtr;

   //---------------------------------------------------------------------------
   //
   // GEOMETRIC OBJECT PROPERTY CLASSES
   //
   //---------------------------------------------------------------------------

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

   //---------------------------------------------------------------------------
   //
   // INTERSECTION CLASSES
   //
   //---------------------------------------------------------------------------
   
   /**
    * A ray with origin o and direction d.
    */
   class Ray
   {
   public:
      explicit Ray(const glm::vec3& direction,
                   const glm::vec3& origin = glm::vec3(0.f))
      : d(direction),
        o(origin),
        tCur(FLT_MAX),
        tMin(0.f),
        tMax(FLT_MAX)
      {}
      glm::vec3 d, o;
      float tCur;
      float tMin;
      float tMax;
   };
   typedef boost::shared_ptr<Ray> RayPtr;
   typedef boost::shared_ptr<const Ray> ConstRayPtr;

   /**
    * Surface storage helper class
    */
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

   /**
    * Intersection information.
    */
   class Intersection
   {
   public:
      explicit Intersection()
      : hit(false),
        t(FLT_MAX),
        p(glm::vec3(0.f)),
        n(glm::vec3(0.f, 1.f, 0.f)),
        surf(Surface())
      {}
      explicit Intersection(bool hit, float hitT, const glm::vec3& hitPoint, const glm::vec3& normal, const Surface& surface)
      : hit(hit),
        t(hitT),
        p(hitPoint),
        n(normal),
        surf(surface)
      {}
      bool hit;
      float t;
      glm::vec3 p;
      glm::vec3 n;
      Surface surf;
   };
   typedef boost::shared_ptr<Intersection> IntersectionPtr;
   typedef boost::shared_ptr<const Intersection> ConstIntersectionPtr;

   //---------------------------------------------------------------------------
   //
   // GEOMETRIC OBJECT CLASSES
   //
   //---------------------------------------------------------------------------

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
        _inverse(glm::inverse(modelXform)),
        _transposedAdjoint(glm::transpose(adjoint(glm::mat3(modelXform)))),
        _finish(finish)
      {}
      virtual ~GeomObject() {}

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

   private:
      //functions
      glm::mat3 adjoint(const glm::mat3& m) const;
      //data
      glm::mat4 _modelXform;
      glm::mat4 _inverse;
      glm::mat3 _transposedAdjoint;
      Finish    _finish;
   };
   typedef boost::shared_ptr<GeomObject> GeomObjectPtr;
   typedef boost::shared_ptr<const GeomObject> ConstGeomObjectPtr;

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

      //Intersection
      Intersection* intersect(const Ray& ray) const
      { return NULL; }

   private:
      glm::vec3 _smCorner;
      glm::vec3 _lgCorner;
   };
   typedef boost::shared_ptr<Box> BoxPtr;
   typedef boost::shared_ptr<const Box> ConstBoxPtr;

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

      //Intersection
      Intersection* intersect(const Ray& ray) const
      { return NULL; }

   private:
      glm::vec3 _end1; //center for end 1
      glm::vec3 _end2; //center for end 2
      float     _radius1;
      float     _radius2;
   };
   typedef boost::shared_ptr<Cone> ConePtr;
   typedef boost::shared_ptr<const Cone> ConstConePtr;

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

      //Intersection
      Intersection* intersect(const Ray& ray) const;

   private:
      glm::vec3 _normal;
      float     _distance;
   };
   typedef boost::shared_ptr<Plane> PlanePtr;
   typedef boost::shared_ptr<const Plane> ConstPlanePtr;

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

      //Intersection
      Intersection* intersect(const Ray& ray) const;

   private:
      glm::vec3 _center;
      float     _radius, _radiusSquared;
   };
   typedef boost::shared_ptr<Sphere> SpherePtr;
   typedef boost::shared_ptr<const Sphere> ConstSpherePtr;

   /**
    * Triangle Geometric Object Storage Class
    */
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
      {}

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
      { _vert0 = vertex0; _vert1 = vertex1; _vert2 = vertex2; _normal = glm::normalize(glm::cross(_vert1-_vert0, _vert2-_vert0)); }

      OBJ_TYPE getType() const
      { return TRIANGLE; }

      //Intersection
      Intersection* intersect(const Ray& ray) const;

   private:
      glm::vec3 _vert0;
      glm::vec3 _vert1;
      glm::vec3 _vert2;
      glm::vec3 _normal;
   };
   typedef boost::shared_ptr<Triangle> TrianglePtr;
   typedef boost::shared_ptr<const Triangle> ConstTrianglePtr;

   /**
    * Package class for all data required for a scene.
    */
   class SceneDescription
   {
   public:
      explicit SceneDescription()
      : _pCam(boost::shared_ptr<Camera>()),
        _pLights(boost::shared_ptr<std::vector<PointLightPtr> >()),
        _pObjs(boost::shared_ptr<std::vector<GeomObjectPtr> >()),
        _pSpheres(boost::shared_ptr<std::vector<SpherePtr> >()),
        _pTriangles(boost::shared_ptr<std::vector<TrianglePtr> >())
      {}
      explicit SceneDescription(CameraPtr pCamera, boost::shared_ptr<std::vector<PointLightPtr> > lights, boost::shared_ptr<std::vector<GeomObjectPtr> > geometryObjects, boost::shared_ptr<std::vector<SpherePtr> > sphereObjects, boost::shared_ptr<std::vector<TrianglePtr> > triangles)
      : _pCam(pCamera),
        _pLights(lights),
        _pObjs(geometryObjects),
        _pSpheres(sphereObjects),
        _pTriangles(triangles)
      {}

      //Mutable
      void setCam(CameraPtr pCamera)
      { _pCam = pCamera; }
      void setLights(boost::shared_ptr<std::vector<PointLightPtr> > lights)
      { _pLights = lights; }
      void setObjs(boost::shared_ptr<std::vector<GeomObjectPtr> > objs)
      { _pObjs = objs; }
      void setSpheres(boost::shared_ptr<std::vector<SpherePtr> > spheres)
      { _pSpheres = spheres; }
      void setTriangles(boost::shared_ptr<std::vector<TrianglePtr> > triangles)
      { _pTriangles = triangles; }

      //Non-mutable
      const Camera& cam() const
      { return *_pCam; }
      const std::vector<PointLightPtr>& lights() const
      { return *_pLights; }
      const std::vector<GeomObjectPtr>& objs() const
      { return *_pObjs; }
      const std::vector<SpherePtr>& spheres() const
      { return *_pSpheres; }
      const std::vector<TrianglePtr>& triangles() const
      { return *_pTriangles; }

   private:
      CameraPtr _pCam;
      boost::shared_ptr<std::vector<PointLightPtr> > _pLights;
      boost::shared_ptr<std::vector<GeomObjectPtr> > _pObjs;
      boost::shared_ptr<std::vector<SpherePtr> > _pSpheres;
      boost::shared_ptr<std::vector<TrianglePtr> > _pTriangles;
   };
} // end namespace RDST

#endif

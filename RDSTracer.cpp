
/**
 * File: RDSTracer.cpp
 * --------------------
 * The actual Ray Tracer Implementation!
 * Author: Ryan Schmitt
 */

#include "RDSTracer.h"
#include <iostream>
#include <iomanip>
#include <boost/foreach.hpp>

namespace RDST
{
   /* Definition for static variables */
   float Ray::tMin = 0.f;
   float Ray::tMax = FLT_MAX;

   /* FUNCTIONS */

   void Tracer::RayTrace(const SceneDescription& scene, Image& image)
   {
      //Create rays
      std::vector<RayPtr> rays(GenerateRays(*scene.pCam, image));
      //For each ray
      for (unsigned int rayi=0; rayi < rays.size(); ++rayi) { //note to self: using int for-loop here so I can use it to reference a pixel as well as a ray.
         Intersection intrs = RayObjectsIntersect(*rays[rayi], scene.objs);
         //Shade on hit
         if (intrs.hit) {
            ShadePixel(image.get(rayi), *scene.pCam, scene.lights, intrs);
         }
      }
   }

   std::vector<RayPtr> Tracer::GenerateRays(const Camera& cam, const Image& image)
   {
      std::vector<RayPtr> rays;
      int h = image.getHeight();
      int w = image.getWidth();
      for (int y=0; y<h; y++) {
         for (int x=0; x<w; x++) {
            //Normalize screen coordinates: [-1, 1]
            float u = (((x+0.5f)/w)-0.5f) * 2.f;
            float v = (((y+0.5f)/h)-0.5f) * 2.f;
            //Create Ray
            glm::vec3 rayOrigin(0.f,0.f,0.f); //view space
            glm::vec3 rayDir(u,v,1.f);
            glm::mat4 matViewWorld(glm::vec4(cam.getRight(),0.f), glm::vec4(cam.getUp(),0.f), 1.f*glm::vec4(cam.getDir(),0.f), glm::vec4(cam.getPos(),1.f));
            rayOrigin = glm::vec3(matViewWorld * glm::vec4(rayOrigin, 1.f)); //convert to world space
            rayDir = glm::normalize(glm::vec3(matViewWorld * glm::vec4(rayDir, 0.f)));
            rays.push_back(RayPtr(new Ray(rayDir, rayOrigin)));
         }
      }
      return rays;
   }

   void Tracer::ShadePixel(Pixel& p, const Camera& cam, const std::vector<PointLightPtr>& lights, const Intersection& intrs)
   {
      PointLight& light(*lights.at(0));
      glm::vec3 ambient(intrs.surf.finish.getAmbient() * intrs.surf.color * light.getColor());
      glm::vec3 l(glm::normalize(light.getPos()-intrs.p));
      float dif = glm::max(0.f, glm::dot(intrs.n, l));
      glm::vec3 diffuse(dif * intrs.surf.finish.getDiffuse() * intrs.surf.color * light.getColor());
      glm::vec4 src(ambient + diffuse,1.f);
      //glm::vec4 dst(p.get());
      //dst = (src*src.a) + (dst*(1-src.a)); //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
      p.set(src);
   }
   
   Intersection Tracer::RayObjectsIntersect(Ray& ray, const std::vector<GeomObjectPtr>& objs)
   {
      Intersection retIntrs; //defaults to hit=false
      //Intersect loop over all objects to find the closest hit
      BOOST_FOREACH (GeomObjectPtr pGO, objs) {
         Intersection intrs;
         OBJ_TYPE type = pGO->getType();
         if (type == RDST::SPHERE) {
            intrs = RaySphereIntersect(ray, *boost::dynamic_pointer_cast<Sphere, GeomObject>(pGO));
         }
         else if(type == RDST::PLANE) {
            intrs = RayPlaneIntersect(ray, *boost::dynamic_pointer_cast<Plane, GeomObject>(pGO));
         }
         //Check for closer, valid, hit
         if (intrs.hit &&
             intrs.t < ray.tCur &&
             intrs.t < Ray::tMax &&
             intrs.t > Ray::tMin) {
               ray.tCur = intrs.t; //set new current t
               retIntrs = intrs; //it's closer; grab it!
         }
      }
      return retIntrs;
   }

   Ray Tracer::TransformRay(const Ray& ray, const glm::mat4& worldToObj)
   {
      glm::vec3 o = glm::vec3(worldToObj*glm::vec4(ray.o,1.f));
      glm::vec3 dir = glm::vec3(worldToObj*glm::vec4(ray.d,0.f)); //TODO: should I normalize?
      return Ray(glm::normalize(dir), o);
   }

   Intersection Tracer::RaySphereIntersect(const Ray& ray, const Sphere& sphere)
   {
      //Setup transformed ray
      Ray xr = TransformRay(ray, glm::inverse(sphere.getModelXform()));
      //Intersection Code
      glm::vec3 l = sphere.getCenter() - xr.o;
      float s = glm::dot(l, xr.d);
      float ll = glm::dot(l, l);
      float rr = sphere.getRadiusSquared();
      if (s < 0.f && ll > rr) return Intersection(); //sphere is behind us and we're not inside
      float mm = ll-(s*s); //dist from sphere center projected onto ray to sphere center
      if (mm > rr) return Intersection(); //ray misses (sphere center projected onto ray - sphere center > radius)
      float q = sqrtf(rr-mm);
      float t = 0.f;
      if (ll > rr) t = s-q; //we're outside the sphere so return first point
      else t = s+q;
      glm::vec3 n(sphere.getModelXform() * glm::vec4((xr.o+(xr.d*t))-sphere.getCenter(), 0.f));
      return Intersection(true, t, ray.o + (ray.d*t), glm::normalize(n), Surface(sphere.getColor(), sphere.getFinish()));
   }

   Intersection Tracer::RayPlaneIntersect(const Ray& ray, const Plane& plane)
   {
      //Setup transformed Ray
      Ray xr = TransformRay(ray, glm::inverse(plane.getModelXform()));
      //Intersection code
      glm::vec3 n = plane.getNormal();
      if (glm::dot(xr.d, n) >= 0.f) return Intersection(); //ray either will hit on -t or be parallel
      float t = -1*fabs(glm::dot(n,xr.o)+plane.getDistance()) / glm::dot(n,xr.d);
      return Intersection(true, t, ray.o + (ray.d*t), plane.getNormal(), Surface(plane.getColor(), plane.getFinish()));
   }
}
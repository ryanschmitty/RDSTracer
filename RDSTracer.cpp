
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
      //Create rays
      std::vector<RayPtr> rays(GenerateRays(scene.cam(), image));
      //For each ray
      for (unsigned int rayi=0; rayi < rays.size(); ++rayi) { //note to self: using int for-loop here so I can use it to reference a pixel as well as a ray.
         Intersection intrs = RayObjectsIntersect(*rays[rayi], scene.objs());
         //Shade on hit
         if (intrs.hit) {
            ShadePixel(image.get(rayi), scene, intrs);
         }
      }
   }

   std::vector<RayPtr> Tracer::GenerateRays(const Camera& cam, const Image& image)
   {
      std::vector<RayPtr> rays;
      float h = image.getHeight();
      float w = image.getWidth();
      float r = glm::length(cam.getRight())*0.5f;
      float l = -r;
      float t = glm::length(cam.getUp())*0.5f;
      float b = -t;
      for (int y=0; y<h; y++) {
         for (int x=0; x<w; x++) {
            //Get view coords
            float u = l+((r-l)*(x+0.5f)/w);
            float v = b+((t-b)*(y+0.5f)/h);
            //Create Ray
            glm::vec3 rayOrigin(0.f,0.f,0.f); //view space
            glm::vec3 rayDir(u,v,1.f);
            glm::mat4 matViewWorld(glm::vec4(glm::normalize(cam.getRight()),0.f), glm::vec4(cam.getUp(),0.f), glm::vec4(cam.getDir(),0.f), glm::vec4(cam.getPos(),1.f));
            rayOrigin = glm::vec3(matViewWorld * glm::vec4(rayOrigin, 1.f)); //convert to world space
            rayDir = glm::normalize(glm::vec3(matViewWorld * glm::vec4(rayDir, 0.f)));
            rays.push_back(RayPtr(new Ray(rayDir, rayOrigin)));
         }
      }
      return rays;
   }

   void Tracer::ShadePixel(Pixel& p, const SceneDescription& scene, const Intersection& intrs)
   {
      //Required Vars
      PointLight& light = *scene.lights().at(0);
      //Ambient
      glm::vec3 ambient(intrs.surf.finish.getAmbient() * intrs.surf.color * light.getColor());
      //Diffuse and Specular
      glm::vec3 pointToLight = light.getPos()-intrs.p;
      Ray shadowRay = Ray(glm::normalize(pointToLight), intrs.p+(0.01f*intrs.n)); //Note to self: needed to move the shadow ray off the origin object a bit
      shadowRay.tMax = glm::length(pointToLight);
      glm::vec3 diffuse(0.f);
      glm::vec3 specular(0.f);
      if (!RayObjectsIntersect(shadowRay, scene.objs()).hit) {
         //diffuse calcs
         glm::vec3 l = glm::normalize(light.getPos()-intrs.p);
         float diff = glm::max(0.f, glm::dot(intrs.n, l));
         diffuse = glm::vec3(diff * intrs.surf.finish.getDiffuse() * intrs.surf.color * light.getColor());
         //specular calcs
         glm::vec3 v = glm::normalize(scene.cam().getPos()-intrs.p);
         glm::vec3 h = glm::normalize(l+v);
         float spec = glm::max(0.f, glm::dot(intrs.n, h));
         specular = glm::vec3(powf(spec,1.f/intrs.surf.finish.getRoughness()) * intrs.surf.color * intrs.surf.finish.getSpecular() * light.getColor());
      }
      //Put it all together and blend
      glm::vec4 src(ambient + diffuse + specular,1.f);
      glm::vec4 dst = p.rgba();
      dst = (src*src.a) + (dst*(1-src.a)); //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
      p.set(dst);
   }

   Intersection Tracer::RayObjectsIntersect(Ray& ray, const std::vector<GeomObjectPtr>& objs)
   {
      Intersection retIntrs; //defaults to hit=false
      //Intersect loop over all objects to find the closest hit
      std::vector<GeomObjectPtr>::const_iterator cit = objs.begin();
      for (; cit != objs.end(); ++cit) {
         Intersection intrs;
         OBJ_TYPE type = (*cit)->getType();
         if (type == RDST::SPHERE) {
            intrs = RaySphereIntersect(ray, *boost::dynamic_pointer_cast<Sphere, GeomObject>(*cit));
         }
         else if (type == RDST::PLANE) {
            intrs = RayPlaneIntersect(ray, *boost::dynamic_pointer_cast<Plane, GeomObject>(*cit));
         }
         else if (type == RDST::TRIANGLE) {
            intrs = RayTriangleIntersect(ray, *boost::dynamic_pointer_cast<Triangle, GeomObject>(*cit));
         }
         //Check for closer, valid, hit
         if (intrs.hit &&
            intrs.t < ray.tCur &&
            intrs.t < ray.tMax &&
            intrs.t > ray.tMin) {
               ray.tCur = intrs.t; //set new current t
               retIntrs = intrs; //it's closer; grab it!
         }
      }
      return retIntrs;
   }

   Ray Tracer::TransformRay(const Ray& ray, const glm::mat4& worldToObj)
   {
      glm::vec3 o = glm::vec3(worldToObj*glm::vec4(ray.o,1.f));
      glm::vec3 dir = glm::vec3(worldToObj*glm::vec4(ray.d,0.f));
      return Ray(glm::normalize(dir), o);
   }

   Intersection Tracer::RaySphereIntersect(const Ray& ray, const Sphere& sphere)
   {
      //Setup transformed ray
      Ray xr = TransformRay(ray, sphere.getModelInverse());
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
      glm::vec3 n = sphere.getNormalXform() * glm::normalize((xr.o+(xr.d*t))-sphere.getCenter()); //make sure to normalize n after this.
      return Intersection(true, t, ray.o + (ray.d*t), glm::normalize(n), Surface(sphere.getColor(), sphere.getFinish()));
   }

   Intersection Tracer::RayPlaneIntersect(const Ray& ray, const Plane& plane)
   {
      //Setup transformed Ray
      Ray xr = TransformRay(ray, plane.getModelInverse());
      //Intersection code
      glm::vec3 n = plane.getNormal();
      if (glm::dot(xr.d, n) >= 0.f) return Intersection(); //ray either will hit on -t or be parallel
      float t = -1*fabs(glm::dot(n,xr.o)+plane.getDistance()) / glm::dot(n,xr.d);
      return Intersection(true, t, ray.o + (ray.d*t), plane.getNormal(), Surface(plane.getColor(), plane.getFinish()));
   }

   Intersection Tracer::RayTriangleIntersect(const Ray& ray, const Triangle& tri)
   {
      //Setup transformed Ray
      Ray xr = TransformRay(ray, tri.getModelInverse());
      //Intersection code (Essential Mathematics for Games & Interactive Applications)
      glm::vec3 e1 = tri.getVertex1() - tri.getVertex0();
      glm::vec3 e2 = tri.getVertex2() - tri.getVertex0();
      glm::vec3 p = glm::cross(xr.d, e2);
      float a = glm::dot(e1, p);
      //if result zero, no intersection or infinite intersections
      //(ray parallel to triangle plane)
      if (a == 0.f) return Intersection();
      //compute denominator
      float f = 1.f/a;
      //compute barycentric coordinates
      glm::vec3 s = xr.o - tri.getVertex0();
      float u = f*glm::dot(s, p);
      if (u < 0.f || u > 1.f) return Intersection();
      glm::vec3 q = glm::cross(s, e1);
      float v = f*glm::dot(xr.d, q);
      if (v < 0.f || u+v > 1.f) return Intersection();
      //compute line parameter
      float t = f*glm::dot(e2, q);
      return Intersection(true, t, ray.o+(ray.d*t), tri.getNormal(), Surface(tri.getColor(), tri.getFinish()));
   }
}

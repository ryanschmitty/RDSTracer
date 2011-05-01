
/**
* File: RDSTracer.cpp
* --------------------
* The actual Ray Tracer Implementation!
* Author: Ryan Schmitt
*/

#include "RDSTracer.h"
#include "ProgressBar.h"
#include <iostream>

#define MAX_REFLECTIONS 5

namespace RDST
{
   void Tracer::RayTrace(const SceneDescription& scene, Image& image)
   {
      //Create rays
      std::vector<RayPtr> rays(GenerateRays(scene.cam(), image));
      std::cout << "\nTracing Rays\n";
      for (unsigned int rayi=0; rayi < rays.size(); ++rayi) { //note to self: using int for-loop here so I can use it to reference a pixel as well as a ray.
         //Intersect each ray against all objects
         Intersection* pIntrs = RayObjectsIntersect(*rays[rayi], scene.objs());
         //Shade on hit
         if (pIntrs->hit) {
            glm::vec4 src = glm::vec4(ShadePoint(*pIntrs, scene, MAX_REFLECTIONS), 1.f);
            glm::vec4 dst = image.get(rayi).rgba();
            dst = (src*src.a) + (dst*(1-src.a)); //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
            image.get(rayi).set(dst);
         }
         delete pIntrs;
         //Progress Bar: update every 10,000 rays
         if (rayi % 10000 == 0) UpdateProgress(int(float(rayi)/rays.size()*100.f));
      }
      UpdateProgress(100);
      std::cout << "\n";
   }

   std::vector<RayPtr> Tracer::GenerateRays(const Camera& cam, const Image& image)
   {
      std::cout << "Generating Rays\n";
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
            //Progress Bar: update every 10,000 rays
            if ((int)(y*w+x) % 10000 == 0) {
               UpdateProgress(int((y*w+x)/(w*h)*100.f));
            }
         }
      }
      UpdateProgress(100);
      std::cout << "\n";
      return rays;
   }

   Intersection* Tracer::RayObjectsIntersect(Ray& ray, const std::vector<GeomObjectPtr>& objs)
   {
      Intersection* pRetIntrs = new Intersection(); //defaults to hit=false
      //Intersect loop over all objects to find the closest hit
      std::vector<GeomObjectPtr>::const_iterator cit = objs.begin();
      for (; cit != objs.end(); ++cit) {
         Intersection* pIntrs = (*cit)->intersect(ray);
         //Check for closer, valid, hit
         if (pIntrs != NULL) { //NULL == no hit
             if (pIntrs->t < ray.tCur &&
                 pIntrs->t < ray.tMax &&
                 pIntrs->t > ray.tMin) {
                    ray.tCur = pIntrs->t; //set new current t
                    delete pRetIntrs;
                    pRetIntrs = pIntrs; //it's closer; grab it!
             }
             else {
                delete pIntrs;
             }
         }
      }
      return pRetIntrs;
   }

   glm::vec3 Tracer::CalcDirectIllum(const Intersection& intrs, const SceneDescription& scene)
   {
      //Parts
      glm::vec3 ambient(0.f);
      glm::vec3 diffuse(0.f);
      glm::vec3 specular(0.f);

      //For each light do Phong Shading & additively blend
      std::vector<PointLightPtr>::const_iterator cit = scene.lights().begin();
      for (; cit != scene.lights().end(); ++cit) {
         PointLight& light = **cit;
         glm::vec3 l = light.getPos()-intrs.p;
         float pointToLightDist = glm::length(l);
         l = glm::normalize(l);
         //Ambient
         ambient += glm::vec3(intrs.surf.finish.getAmbient() * intrs.surf.color * light.getColor());

         //Diffuse and Specular (Shadow Ray)
         Ray shadowRay = Ray(l, intrs.p, 0.001f, pointToLightDist);
         Intersection* pShadowIntrs = RayObjectsIntersect(shadowRay, scene.objs());
         if (!pShadowIntrs->hit) {
            //diffuse calcs
            diffuse += glm::vec3(glm::max(0.f, glm::dot(intrs.n, l)) * intrs.surf.finish.getDiffuse() * intrs.surf.color * light.getColor());
            //specular calcs
            glm::vec3 v = -intrs.incDir;
            glm::vec3 h = glm::normalize(l+v);
            float spec = glm::max(0.f, glm::dot(intrs.n, h));
            specular += glm::vec3(powf(spec,1.f/intrs.surf.finish.getRoughness()) * intrs.surf.color * intrs.surf.finish.getSpecular() * light.getColor());
         }
         delete pShadowIntrs;
      }

      //Additively blend all components and return
      return ambient + diffuse + specular;
   }

   glm::vec3 Tracer::CalcReflection(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionDepth)
   {
      glm::vec3 reflection(0.f);
      if (recursionDepth > 0 && intrs.surf.finish.getReflection() > 0.f) {
         Ray reflectionRay = Ray(glm::reflect(intrs.incDir, intrs.n), intrs.p, 0.001f);
         Intersection* pReflectionIntrs = RayObjectsIntersect(reflectionRay, scene.objs());
         if (pReflectionIntrs->hit) {
            reflection = ShadePoint(*pReflectionIntrs, scene, recursionDepth-1);
         }
         delete pReflectionIntrs;
      }
      return reflection;
   }

   glm::vec3 Tracer::refract(const glm::vec3& normal, const glm::vec3& incident, float n1, float n2)
   {
      float n = n1 / n2;
      float cosI = -glm::dot(normal, incident);
      float sinT2 = n * n * (1.f - cosI * cosI);
      if (sinT2 > 1.0) {
         return glm::vec3(0.f); //TIR
      }
      float cosT = sqrtf(1.f - sinT2);
      return n * incident + (n * cosI - cosT) * normal;
   }

   glm::vec3 Tracer::CalcRefraction(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionDepth)
   {
      glm::vec3 refraction(0.f);
      if (intrs.surf.finish.getRefraction() > 0.f) {
         float eta = intrs.surf.finish.getIndexOfRefraction();
         glm::vec3 normal = -intrs.n;
         if (glm::dot(intrs.n, intrs.incDir) < 0) {
            eta = 1.f/eta;
            normal = -normal;
         }
         Ray refractionRay = Ray(glm::refract(intrs.incDir, normal, eta), intrs.p-(0.01f*normal));
         if (glm::length(refractionRay.d) == 0.f) return refraction;
         Intersection* pRefractionIntrs = RayObjectsIntersect(refractionRay, scene.objs()); //TODO: perhaps store the actual object hit in the Intersection type.
         if (pRefractionIntrs->hit) {
            refraction = ShadePoint(*pRefractionIntrs, scene, recursionDepth-1);
         }
         else {
            refraction = glm::vec3(0, 0, 0);
         }
         delete pRefractionIntrs;
      }
      return refraction;
      /*
      glm::vec3 refraction(0.f);
      if (intrs.surf.finish.getRefraction() > 0.f) {
         //Calc needed vars
         float matToAir = intrs.surf.finish.getIndexOfRefraction();
         float airToMat = 1.f / matToAir;
         //First refraction into the object
         //Ray refractionRay = Ray(refract(intrs.n, intrs.incDir, 1.f, intrs.surf.finish.getIndexOfRefraction()), intrs.p-(0.001f*intrs.n));
         Ray refractionRay = Ray(glm::refract(intrs.incDir, intrs.n, airToMat), intrs.p-(0.001f*intrs.n));
         if (glm::length(refractionRay.d) == 0.f) {
            return refraction;
         }
         Intersection* pRefractionIntrs = RayObjectsIntersect(refractionRay, scene.objs()); //TODO: perhaps store the actual object hit in the Intersection type.
         if (!pRefractionIntrs->hit) { //ERROR
            delete pRefractionIntrs;
            return glm::vec3(5.f, 0.f, 0.f);
         }
         //Second refraction out of the object
         //refractionRay = Ray(refract(-pRefractionIntrs->n, pRefractionIntrs->incDir, intrs.surf.finish.getIndexOfRefraction(), 1.f), pRefractionIntrs->p, 0.1f);
         refractionRay = Ray(glm::refract(pRefractionIntrs->incDir, -pRefractionIntrs->n, matToAir), pRefractionIntrs->p, 0.1f);
         delete pRefractionIntrs;
         if (glm::length(refractionRay.d) == 0.f) {
            return refraction;
         }
         pRefractionIntrs = RayObjectsIntersect(refractionRay, scene.objs());
         if (pRefractionIntrs->hit) {
            refraction = ShadePoint(*pRefractionIntrs, scene, 5);
         }
         delete pRefractionIntrs;
      }
      */
      return refraction;
   }

   glm::vec3 Tracer::ShadePoint(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionDepth)
   {
      //Direct illumination
      glm::vec3 finalColor = CalcDirectIllum(intrs, scene);

      //Reflection
      glm::vec3 reflection = CalcReflection(intrs, scene, recursionDepth);
      if (glm::length(reflection) > 0.f) {
         float reflCoef = intrs.surf.finish.getReflection();
         finalColor = ((1.f-reflCoef)*glm::vec3(finalColor)) + (reflCoef*reflection);
      }

      //Refraction
      if (intrs.surf.finish.getRefraction() > 0.f) {
         glm::vec3 refraction = CalcRefraction(intrs, scene, recursionDepth);
         float refrCoef = intrs.surf.finish.getRefraction();
         finalColor = ((1.f-refrCoef)*glm::vec3(finalColor)) + (refrCoef*refraction);
      }

      return finalColor;
   }
}


/**
* File: RDSTracer.cpp
* --------------------
* The actual Ray Tracer Implementation!
* Author: Ryan Schmitt
*/

#include <iostream>
#include <ctime>
#include "RDSTracer.h"
#include "ProgressBar.h"
#include "RDSbvh.h"

#define MAX_RECURSION_DEPTH 5

namespace RDST
{
   void Tracer::RayTrace(const SceneDescription& scene, Image& image, bool antialias, int subsamples)
   {
      //Anti-aliasing stuff
      if (!antialias) subsamples = 1;
      int samplesPerD = VerifyNumSubsamples(subsamples);
      //Create rays
      int numRaysX = image.getWidth()*samplesPerD;
      int numRaysY = image.getHeight()*samplesPerD;
      RayPtrListPtr rays = GenerateRays(scene.cam(), numRaysX, numRaysY, antialias);
      //Trace Rays
      std::cout << "\nTracing Rays\n";
      for (int y=0; y < image.getHeight(); ++y) {
         for (int x=0; x < image.getWidth(); ++x) {
            //Compute subsample start indices
            int startX = x * samplesPerD;
            int startY = y * samplesPerD;
            //Iterate over all the subsamples and collect color
            glm::vec3 color(0.f);
            for (int i=0; i < samplesPerD; ++i) {
               for (int j=0; j < samplesPerD; ++j) {
                  int rayi = (startY+j) * numRaysX + (startX+i); //2D to 1D conversion
                  color += TraceRay(*(*rays)[rayi], scene, MAX_RECURSION_DEPTH);
                  if (rayi % 10000 == 0) UpdateProgress(int(float(rayi)/rays->size()*100.f));
               }
            }
            //Apply box filter and write to image
            image.get(x,y).set( glm::vec4(color/(float)subsamples, 1.f) );
         }
      }
      UpdateProgress(100);
      std::cout << "\n";
   }

   int Tracer::VerifyNumSubsamples(int subsamples)
   {
      float f_sqrt = sqrtf((float)subsamples);
      int i_sqrt = (int)f_sqrt;
      if (f_sqrt != i_sqrt) {
         std::cerr << "ERROR***: Anti-Aliasing sub-samples must be a perfect square; you entered " << subsamples << ".\n";
         exit(EXIT_FAILURE);
      }
      return i_sqrt;
   }

   RayPtrListPtr Tracer::GenerateRays(const Camera& cam, int raysInX, int raysInY, bool jitter)
   {
      std::cout << "Generating Rays\n";
      RayPtrListPtr rays = RayPtrListPtr(new std::vector<RayPtr>());
      float h = (float)raysInY;
      float w = (float)raysInX;
      float r = glm::length(cam.getRight())*0.5f;
      float l = -r;
      float t = glm::length(cam.getUp())*0.5f;
      float b = -t;
      srand((unsigned int)time(NULL));
      for (int y=0; y<h; y++) {
         for (int x=0; x<w; x++) {
            //Jitter samples
            float xOffset = 0.5;
            float yOffset = 0.5;
            if (jitter) {
               xOffset = (float)rand() / RAND_MAX;
               yOffset = (float)rand() / RAND_MAX;
            }
            //Get view coords
            float u = l+((r-l)*(x+xOffset)/w);
            float v = b+((t-b)*(y+yOffset)/h);
            //Create Ray
            glm::vec3 rayOrigin(0.f,0.f,0.f); //view space
            glm::vec3 rayDir(u,v,1.f);
            glm::mat4 matViewWorld(glm::vec4(glm::normalize(cam.getRight()),0.f), glm::vec4(cam.getUp(),0.f), glm::vec4(cam.getDir(),0.f), glm::vec4(cam.getPos(),1.f));
            rayOrigin = glm::vec3(matViewWorld * glm::vec4(rayOrigin, 1.f)); //convert to world space
            rayDir = glm::normalize(glm::vec3(matViewWorld * glm::vec4(rayDir, 0.f)));
            rays->push_back(RayPtr(new Ray(rayDir, rayOrigin)));
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

   glm::vec3 Tracer::TraceRay(Ray& ray, const SceneDescription& scene, unsigned int recursionsLeft)
   {
      glm::vec3 color(0.f);
      Intersection* pIntrs = RaySceneIntersect(ray, scene);
      if (pIntrs->hit) {
         color = ShadePoint(*pIntrs, scene, recursionsLeft);
      }
      delete pIntrs;
      return color;
   }

   Intersection* Tracer::RaySceneIntersect(Ray& ray, const SceneDescription& scene)
   {
      Intersection* pIsect = scene.bvh().intersect(ray);
      Intersection* pIsectOther = RayObjectsIntersect(ray, scene.planes());
      if (!pIsect->hit) {
         delete pIsect;
         return pIsectOther;
      }
      else if (!pIsectOther->hit) {
         delete pIsectOther;
         return pIsect;
      }
      if (pIsect->t > pIsectOther->t) {
         delete pIsect;
         pIsect = pIsectOther;
      }
      return pIsect;
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
   
   glm::vec3 Tracer::ShadePoint(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
   {
      //return (intrs.n+1.f) * 0.5f;
      //Reflection
      float reflAmt = intrs.surf.finish.getReflection();
      glm::vec3 reflection(0.f);
      if (reflAmt > 0.f) {
         reflection = reflAmt * CalcReflection(intrs, scene, recursionsLeft);
         reflection = glm::clamp(reflection, 0.f, FLT_MAX);
      }

      //Refraction
      float refrAmt = 0.f;
      glm::vec3 refraction(0.f);
      if (intrs.surf.finish.getRefraction() > 0.f) {
         refrAmt = intrs.surf.color.a;
         refraction = CalcRefraction(intrs, scene, recursionsLeft);
         if (glm::length(refraction) <= 0.f)
            refrAmt = 0.f;
         else
            refraction = glm::clamp(refrAmt*refraction, 0.f, FLT_MAX);
      }

      //Direct illumination
      float dirAmt = 1.f - reflAmt - refrAmt;
      glm::vec3 direct(0.f);
      if (dirAmt > 0.f) {
         direct = dirAmt * CalcDirectIllum(intrs, scene);
         direct = glm::clamp(direct, 0.f, FLT_MAX);
      }

      return direct + reflection + refraction;
   }

   glm::vec3 Tracer::CalcDirectIllum(const Intersection& intrs, const SceneDescription& scene)
   {
      //Parts
      glm::vec3 ambient(0.f);
      glm::vec3 diffuse(0.f);
      glm::vec3 specular(0.f);

      Sphere light = scene.areaLight;

      ambient += glm::vec3(intrs.surf.finish.getAmbient() * intrs.surf.color * light.getColor());
      int numSamples = 0;
      for (int i=0; i<256; ++i) {
         glm::vec3 l = light.uniformSample((float)rand() / RAND_MAX, (float)rand() / RAND_MAX)-intrs.p;
         float pointToLightSampleDist = glm::length(l);
         l = glm::normalize(l);
         Ray shadowRay = Ray(l, intrs.p+(0.01f*intrs.n), 0.f, pointToLightSampleDist);
         Intersection* pShadowIntrs = RaySceneIntersect(shadowRay, scene);
         if (!pShadowIntrs->hit) {
            numSamples++;
            diffuse += 0.00390625f * glm::vec3(glm::max(0.f, glm::dot(intrs.n, l)) * intrs.surf.finish.getDiffuse() * intrs.surf.color * light.getColor());
         }
         delete pShadowIntrs;
      }

      /*
      //For each light do Phong Shading & additively blend
      std::vector<PointLightPtr>::const_iterator cit = scene.lights().begin();
      for (; cit != scene.lights().end(); ++cit) {
         const PointLight& light = **cit;
         glm::vec3 l = light.getPos()-intrs.p;
         float pointToLightDist = glm::length(l);
         l = glm::normalize(l);
         //Ambient
         ambient += glm::vec3(intrs.surf.finish.getAmbient() * intrs.surf.color * light.getColor());

         //Diffuse and Specular (Shadow Ray)
         Ray shadowRay = Ray(l, intrs.p+(0.01f*intrs.n), 0.f, pointToLightDist);
         Intersection* pShadowIntrs = RaySceneIntersect(shadowRay, scene);
         if (!pShadowIntrs->hit) {
            //diffuse calcs
            diffuse += glm::vec3(glm::max(0.f, glm::dot(intrs.n, l)) * intrs.surf.finish.getDiffuse() * intrs.surf.color * light.getColor());
            //specular calcs
            glm::vec3 v = -intrs.incDir;
            glm::vec3 h = glm::normalize(l+v);
            float spec = glm::max(0.f, glm::dot(intrs.n, h));
            specular += glm::vec3(powf(spec,1.f/intrs.surf.finish.getRoughness()) * intrs.surf.finish.getSpecular() * light.getColor());
         }
         delete pShadowIntrs;
      }
      */

      //Additively blend all components and return
      return ambient + diffuse + specular;
   }

   glm::vec3 Tracer::CalcReflection(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
   {
      glm::vec3 reflection(0.f);
      if (recursionsLeft > 0 && intrs.surf.finish.getReflection() > 0.f) {
         Ray reflectionRay = Ray(glm::reflect(intrs.incDir, intrs.n), intrs.p);
         reflectionRay.o += 0.01f * reflectionRay.d;
         reflection = TraceRay(reflectionRay, scene, recursionsLeft-1);
      }
      return reflection;
   }

   glm::vec3 Tracer::CalcRefraction(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
   {
      glm::vec3 refraction(0.f);
      if (recursionsLeft > 0 && intrs.surf.finish.getRefraction() > 0.f) {
         float eta = intrs.surf.finish.getIndexOfRefraction(); //material-to-air
         if (!intrs.inside) {
            eta = 1.f/eta; //air-to-material
         }
         Ray refractionRay = Ray(glm::refract(intrs.incDir, intrs.n, eta), intrs.p);
         refractionRay.o += 0.1f * refractionRay.d;
         if (glm::length(refractionRay.d) == 0.f) return refraction; //TIR
         refraction = TraceRay(refractionRay, scene, recursionsLeft-1);
      }
      return refraction;
   }
}

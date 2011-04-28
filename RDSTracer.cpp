
/**
* File: RDSTracer.cpp
* --------------------
* The actual Ray Tracer Implementation!
* Author: Ryan Schmitt
*/

#include "RDSTracer.h"
#include "ProgressBar.h"
#include <iostream>

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
            ShadePixel(image.get(rayi), scene, *pIntrs);
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
                    //std::cout << "new T: " << pIntrs->t << std::endl;
                    ray.tCur = pIntrs->t; //set new current t
                    //std::cout << "ray T: " << ray.tCur << std::endl;
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

   void Tracer::ShadePixel(Pixel& p, const SceneDescription& scene, const Intersection& intrs)
   {
      //Required Vars
      PointLight& light = *scene.lights().at(0);
      glm::vec3 l = light.getPos()-intrs.p;
      float pointToLightDist = glm::length(l);
      l = glm::normalize(l);
      glm::vec3 v = glm::normalize(scene.cam().getPos()-intrs.p);
      glm::vec3 diffuse(0.f);
      glm::vec3 specular(0.f);
      glm::vec3 reflection(0.f);

      //Ambient
      glm::vec3 ambient(intrs.surf.finish.getAmbient() * intrs.surf.color * light.getColor());

      //Diffuse and Specular (Shadow Ray)
      Ray shadowRay = Ray(l, intrs.p, 0.001f, pointToLightDist);
      Intersection* pShadowIntrs = RayObjectsIntersect(shadowRay, scene.objs());
      if (!pShadowIntrs->hit) {
         //diffuse calcs
         diffuse = glm::vec3(glm::max(0.f, glm::dot(intrs.n, l)) * intrs.surf.finish.getDiffuse() * intrs.surf.color * light.getColor());
         //specular calcs
         glm::vec3 h = glm::normalize(l+v);
         float spec = glm::max(0.f, glm::dot(intrs.n, h));
         specular = glm::vec3(powf(spec,1.f/intrs.surf.finish.getRoughness()) * intrs.surf.color * intrs.surf.finish.getSpecular() * light.getColor());
      }
      delete pShadowIntrs;

      //Reflection
      Ray reflectionRay = Ray(glm::reflect(-v, intrs.n), intrs.p, 0.001f);
      reflection = CalcReflection(reflectionRay, scene.objs(), intrs.surf, 5);

      //Put it all together and blend
      glm::vec4 src(ambient + diffuse + specular + reflection,1.f);
      glm::vec4 dst = p.rgba();
      dst = (src*src.a) + (dst*(1-src.a)); //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
      p.set(dst);
   }

   glm::vec3 Tracer::CalcReflection(Ray& reflectionRay, const std::vector<GeomObjectPtr>& objs, const Surface& startSurface, int numReflections)
   {
      //Init temp variables
      Ray curRay = reflectionRay;
      Surface curSurf = startSurface;
      glm::vec3 color(0.f);
      //Loop for each reflection
      for (int i=0; i<numReflections; ++i) {
         if (curSurf.finish.getReflection() == 0.f) break; //skip hard work if no reflection
         Intersection* pReflectionIntrs = RayObjectsIntersect(curRay, objs);
         if (!pReflectionIntrs->hit) { //we missed; ray goes flying into nothingness... :'(
            delete pReflectionIntrs;
            break;
         }
         color += glm::vec3(curSurf.finish.getReflection() * curSurf.color * pReflectionIntrs->surf.color);
         curRay = Ray(glm::reflect(-curRay.d, pReflectionIntrs->n), pReflectionIntrs->p, 0.001f);
         curSurf = pReflectionIntrs->surf;
         delete pReflectionIntrs;
      }
      return color;
   }
}

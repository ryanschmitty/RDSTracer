
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
            glm::vec4 src = glm::vec4(ShadePoint(*pIntrs, scene, 5), 1.f);
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

   glm::vec3 Tracer::ShadePoint(const Intersection& intrs, const SceneDescription& scene, unsigned int numReflectionBounces)
   {
      //Required Vars
      glm::vec3 ambient(0.f);
      glm::vec3 diffuse(0.f);
      glm::vec3 specular(0.f);
      glm::vec3 reflection(0.f);
      glm::vec3 v = glm::normalize(scene.cam().getPos()-intrs.p);

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
            glm::vec3 h = glm::normalize(l+v);
            float spec = glm::max(0.f, glm::dot(intrs.n, h));
            specular += glm::vec3(powf(spec,1.f/intrs.surf.finish.getRoughness()) * intrs.surf.color * intrs.surf.finish.getSpecular() * light.getColor());
         }
         delete pShadowIntrs;
      }

      //Reflection
      if (intrs.surf.finish.getReflection() > 0.f) {
         Ray reflectionRay = Ray(glm::reflect(-v, intrs.n), intrs.p, 0.001f);
         reflection = CalcReflection(reflectionRay, scene, numReflectionBounces);
      }

      //Put it all together and blend
      glm::vec3 finalColor(ambient + diffuse + specular);
      if (glm::length(reflection) > 0.f) {
         float reflCoef = intrs.surf.finish.getReflection();
         finalColor = ((1.f-reflCoef)*glm::vec3(finalColor)) + (reflCoef*reflection);
      }
      return finalColor;
   }

   glm::vec3 Tracer::CalcReflection(Ray& reflectionRay, const SceneDescription& scene, int numReflections)
   {
      //Short Circuit
      if (numReflections <= 0) return glm::vec3(0.f); //skip hard work if no reflection

      //Initialization
      glm::vec3 finalColor(0.f);
      Ray curRay = reflectionRay;
      std::vector<glm::vec4> surfProps; //r, g, b, reflection coef

      //Loop for each reflection
      for (int i=0; i<numReflections; ++i) {
         Intersection* pReflectionIntrs = RayObjectsIntersect(curRay, scene.objs());
         if (!pReflectionIntrs->hit) {
            //TODO: background color
            delete pReflectionIntrs;
            break;
         }
         //evaluate full color of intersection point
         glm::vec3 hitSurfColor = ShadePoint(*pReflectionIntrs, scene, numReflections-1);
         surfProps.push_back( glm::vec4(hitSurfColor.r,
                                        hitSurfColor.g,
                                        hitSurfColor.b,
                                        pReflectionIntrs->surf.finish.getReflection()) );
         if (pReflectionIntrs->surf.finish.getReflection() == 0.f) {delete pReflectionIntrs; break;} //finish when no reflection
         delete pReflectionIntrs;
         curRay = Ray(glm::reflect(curRay.d, pReflectionIntrs->n), pReflectionIntrs->p, 0.001f);
      }
      if (surfProps.size() == 0) return glm::vec3(0.f); //no reflections hit anything
      surfProps.back().a = 0.f; //set final reflectance to 0... for safety!

      //Construct color
      std::vector<glm::vec4>::reverse_iterator crit = surfProps.rbegin();
      for (; crit != surfProps.rend(); ++crit) {
         finalColor = (glm::vec3(*crit) * (1.f - crit->a)) + (finalColor * crit->a);
      }

      return finalColor;
   }
}

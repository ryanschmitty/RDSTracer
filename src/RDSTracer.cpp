
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
#include "RandUtil.h"
#include "RDSFilters.h"
#include "RDSSamplers.h"
#include "RDSIndirectIllum.h"
#include <omp.h>

namespace RDST
{
   void Tracer::RayTrace(const SceneDescription& scene, Image& image)
   {
      //Create rays
      RayListPtr rays = GenerateRays(scene.cam(), image.getWidth(), image.getHeight(), scene.opts());

      //Trace Rays
      omp_set_num_threads(scene.opts().numThreads);
      std::cout << "\nTracing Rays with " << scene.opts().numThreads << " threads:\n";
      //Declare variables
      int currentRay = 0;
      int i, j, rayistart, rayi;
      glm::vec3 color;
      float sumWeights;
      int subsamples = scene.opts().subsamples;
      int progressUpdateNum = rays->size()/100;
      bool first = true;
      //OpenMP Loop
      #pragma omp parallel for \
              shared(currentRay, rays) \
              private(i, j, rayistart, rayi, color, sumWeights) \
              firstprivate(subsamples, progressUpdateNum)  \
              schedule(dynamic,10000)
      for (i=0; i < image.getHeight()*image.getWidth(); ++i) {
         first = false;
         rayistart = i * subsamples;
         color = glm::vec3(0.f);
         //Gather all subsamples
         sumWeights = 0.f;
         for (j=0; j < subsamples; ++j) {
            rayi = rayistart + j;
            Ray& ray = (*rays)[rayi];
            color += ray.weight * TraceRay(ray, scene, MAX_RECURSION_DEPTH);
            sumWeights += ray.weight;
            if (currentRay++ % progressUpdateNum == 0) UpdateProgress(int(float(currentRay)/rays->size()*100.f));
         }
         //Apply box filter and write to image
         image.get(i).set( glm::vec4(color/sumWeights, 1.f) );
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

   RayListPtr Tracer::GenerateRays(const Camera& cam, int width, int height, const Options& opts)
   {
      int sqsamps = VerifyNumSubsamples(opts.subsamples);
      std::cout << "\nGenerating Rays\n";
      glm::mat4 matViewWorld(glm::vec4(glm::normalize(cam.getRight()),0.f), glm::vec4(cam.getUp(),0.f), glm::vec4(cam.getDir(),0.f), glm::vec4(cam.getPos(),1.f));
      RayListPtr rays = RayListPtr(new std::vector<Ray>());
      rays->reserve(width*height*opts.subsamples);
      float h = (float)height;
      float w = (float)width;
      float r = glm::length(cam.getRight())*0.5f;
      float l = -r;
      float t = glm::length(cam.getUp())*0.5f;
      float b = -t;
      //For each pixel
      for (int y=0; y<height; y++) {
         for (int x=0; x<width; x++) {
            //Get ray subsample cluster center
//            float uCenter = l+((r-l)*(x+0.5f)/w);
//            float vCenter = b+((t-b)*(y+0.5f)/h);
            //Compute subsample start indices
            int startX = x * sqsamps;
            int startY = y * sqsamps;
            //For each subsample
            for (int j=0; j<sqsamps; j++) {
               for (int i=0; i<sqsamps; i++) {
                  //Jitter center?
                  float xOffset = 0.5f;
                  float yOffset = 0.5f;
                  if (opts.jitter) {
                     if (opts.filter == Options::GAUSSIAN) {
                        xOffset = unifRand(-0.5f, 1.5f);
                        yOffset = unifRand(-0.5f, 1.5f);
                     }
                     else {
                        xOffset = unifRand();
                        yOffset = unifRand();
                     }
                  }
                  //Get ray coords
                  float u = l+((r-l)*(startX+i+xOffset)/(w*sqsamps));
                  float v = b+((t-b)*(startY+j+yOffset)/(h*sqsamps));
                  //Create Ray
                  glm::vec3 rayOrigin(0.f,0.f,0.f);
                  glm::vec3 rayDir(u,v,1.f);
                  //Convert to world space
                  rayOrigin = glm::vec3(matViewWorld * glm::vec4(rayOrigin, 1.f));
                  rayDir = glm::normalize(glm::vec3(matViewWorld * glm::vec4(rayDir, 0.f)));
                  //Calculate distance from center of pixel from [-1,1]
                  float relativeU = xOffset - 0.5f;
                  float relativeV = yOffset - 0.5f;
                  //float relativeU = ((startX+i+xOffset)/sqsamps) - x; //figured this out with awesome maths
                  //relativeU = 2.f*relativeU - 1.f;
                  //float relativeV = ((startY+j+yOffset)/sqsamps) - y;
                  //relativeV = 2.f*relativeV - 1.f;
                  //std::cout << relativeU << ",\n" << relativeV << std::endl;
                  //Apply filter
                  float weight;
                  if (opts.filter == Options::GAUSSIAN)
                     weight = Filters::GaussianFilter(relativeU, relativeV, opts.gaussianAlpha);
                  else if (opts.filter == Options::MITCHELL)
                     weight = Filters::MitchellFilter(relativeU, relativeV, 1.f);
                  else //BOX
                     weight = 1.f / opts.subsamples;
                  //Store ray
                  rays->push_back(Ray(rayDir, rayOrigin, 0.f, FLT_MAX, weight));
               }
            }
            //Progress Bar: update every 10,000 pixels
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
      //Intersection* pIsect = RayObjectsIntersect(ray, scene.surfels());
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
         direct = dirAmt * CalcDirectIllum(intrs, scene, recursionsLeft);
         direct = glm::clamp(direct, 0.f, FLT_MAX);
      }

      //Indirect illumination
      glm::vec3 indirect(0.f);
      glm::vec3 indirectSurfs(0.f);
      if (scene.opts().bounces > 0 && scene.opts().monteCarloSamples > 0 && recursionsLeft > 0) {
         int bounces = (int)recursionsLeft > scene.opts().bounces ? scene.opts().bounces : recursionsLeft;
//         indirect = CalcIndirectIllumMonteCarlo(intrs, scene, bounces);
//         indirect = CalcIndirectIllumPointBased(intrs, scene, bounces);
//         indirect = IndirectIllumMonteCarlo(intrs, scene, bounces);
//         indirectSurfs = IndirectIllumSurfelRaster(intrs, scene);
         indirect= IndirectIllumSurfelRaster(intrs, scene);
      }

      /*
      if (recursionsLeft > 0) { //first call
//         return glm::length(indirectSurfs) > glm::length(indirect) ? glm::vec3(1.f) : glm::vec3(0.f);
         return glm::vec3(glm::length(indirectSurfs - indirect)/glm::length(glm::vec3(1.f)));
      }
      else //second call via IndirectIllumMonteCarlo
         return (direct + reflection + refraction) + (PI*indirect);
      */

//      return indirect;
//      return 2.f*(direct + reflection + refraction);
      return ((direct + reflection + refraction) + (PI*indirect));
//      return (direct + reflection + refraction) + (indirect);
//      return glm::vec3(glm::length(indirectSurfs - indirect)/glm::length(glm::vec3(1.f)));
//      return glm::length(indirectSurfs) > glm::length(indirect) ? glm::vec3(1.f) : glm::vec3(0.f);
//      return glm::vec3(0.f);
   }

//   glm::vec3 Tracer::ShadePointNoIndirect(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
//   {
//      //return (intrs.n+1.f) * 0.5f;
//      //Reflection
//      float reflAmt = intrs.surf.finish.getReflection();
//      glm::vec3 reflection(0.f);
//      if (reflAmt > 0.f) {
//         reflection = reflAmt * CalcReflection(intrs, scene, recursionsLeft);
//         reflection = glm::clamp(reflection, 0.f, FLT_MAX);
//      }
//
//      //Refraction
//      float refrAmt = 0.f;
//      glm::vec3 refraction(0.f);
//      if (intrs.surf.finish.getRefraction() > 0.f) {
//         refrAmt = intrs.surf.color.a;
//         refraction = CalcRefraction(intrs, scene, recursionsLeft);
//         if (glm::length(refraction) <= 0.f)
//            refrAmt = 0.f;
//         else
//            refraction = glm::clamp(refrAmt*refraction, 0.f, FLT_MAX);
//      }
//
//      //Direct illumination
//      float dirAmt = 1.f - reflAmt - refrAmt;
//      glm::vec3 direct(0.f);
//      if (dirAmt > 0.f) {
//         direct = dirAmt * CalcDirectIllum(intrs, scene, recursionsLeft);
//         direct = glm::clamp(direct, 0.f, FLT_MAX);
//      }
//
//      return (direct + reflection + refraction);
//   }

   void Tracer::DoAreaLights(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, const Intersection& intrs, const SceneDescription& scene)
   {
      //For each light
      std::vector<GeomObjectPtr>::const_iterator calit = scene.areaLights().begin();
      for (; calit != scene.areaLights().end(); ++calit) {
         const GeomObject& light = **calit;

         //Ambient
         ambient += glm::vec3(intrs.surf.finish.getAmbient() * intrs.surf.color * light.getColor());

         //For each sample on light
         boost::shared_ptr< std::vector<glm::vec3> > pLightSamples = light.stratefiedSamples(true, scene.opts().areaLightSamples);
         std::vector<glm::vec3>::const_iterator clsit = pLightSamples->begin();
         float contribution = 1.f / pLightSamples->size();
         for (; clsit != pLightSamples->end(); ++clsit) {

            //Create and process shadow ray
            glm::vec3 l = *clsit - intrs.p;
            float pointToLightSampleDist = glm::length(l);
            l = glm::normalize(l);
            Ray shadowRay = Ray(l, intrs.p+(RAY_EPSILON*intrs.n), 0.f, pointToLightSampleDist);
            Intersection* pShadowIntrs = RaySceneIntersect(shadowRay, scene);
            //If we hit, don't shade, unless we hit an emissive object
            glm::vec3 lightColor = glm::vec3(light.getColor());
            if (pShadowIntrs->hit) {
               if (pShadowIntrs->surf.finish.getEmissive() > 0.f) {
                  lightColor = glm::vec3(pShadowIntrs->surf.color);
               }
               else {
                  delete pShadowIntrs;
                  continue;
               }
            }
            delete pShadowIntrs;

            //Diffuse
            diffuse += contribution * glm::max(0.f, glm::dot(intrs.n, l)) * intrs.surf.finish.getDiffuse() * glm::vec3(intrs.surf.color) * lightColor;

            //Specular
            glm::vec3 v = -intrs.incDir;
            glm::vec3 h = glm::normalize(l+v);
            float spec = glm::max(0.f, glm::dot(intrs.n, h));
            specular += contribution * powf(spec,1.f/intrs.surf.finish.getRoughness()) * intrs.surf.finish.getSpecular() * lightColor;
         }
      }
   }

   void Tracer::DoPointLights(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, const Intersection& intrs, const SceneDescription& scene)
   {
      //For each light
      std::vector<PointLightPtr>::const_iterator cplit = scene.lights().begin();
      for (; cplit != scene.lights().end(); ++cplit) {
         const PointLight& light = **cplit;
         
         //Ambient
         ambient += glm::vec3(intrs.surf.finish.getAmbient() * intrs.surf.color * light.getColor());

         //Create and process shadow ray
         glm::vec3 l = light.getPos()-intrs.p;
         float pointToLightDist = glm::length(l);
         l = glm::normalize(l);
         Ray shadowRay = Ray(l, intrs.p+(RAY_EPSILON*intrs.n), 0.f, pointToLightDist);
         Intersection* pShadowIntrs = RaySceneIntersect(shadowRay, scene);
         if (!pShadowIntrs->hit) {

            //Diffuse
            diffuse += glm::vec3(glm::max(0.f, glm::dot(intrs.n, l)) * intrs.surf.finish.getDiffuse() * intrs.surf.color * light.getColor());

            //Specular
            glm::vec3 v = -intrs.incDir;
            glm::vec3 h = glm::normalize(l+v);
            float spec = glm::max(0.f, glm::dot(intrs.n, h));
            specular += glm::vec3(powf(spec,1.f/intrs.surf.finish.getRoughness()) * intrs.surf.finish.getSpecular() * light.getColor());
         }
         delete pShadowIntrs;
      }
   }

   glm::vec3 Tracer::CalcDirectIllum(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
   {
      //Parts
      glm::vec3 ambient(0.f);
      glm::vec3 diffuse(0.f);
      glm::vec3 specular(0.f);
      glm::vec3 emissive(0.f);

      //Emissive easy-ness
      emissive = intrs.surf.finish.getEmissive() * glm::vec3(intrs.surf.color);

      //Area lights
      DoAreaLights(ambient, diffuse, specular, intrs, scene);

      //For each point light do Phong shading
      DoPointLights(ambient, diffuse, specular, intrs, scene);

      //Additively blend all components and return
      return ambient + diffuse + specular + emissive;
   }

//   glm::vec3 Tracer::CalcIndirectIllumMonteCarlo(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
//   {
//      glm::vec3 indirectColor(0.f);
//      if (recursionsLeft > 0) {
//         int sqsamps = (int)sqrtf((float)scene.opts().monteCarloSamples);
//         //Construct basis for uniform sample -> intersection point transform
//         glm::vec3 k = glm::vec3(0,1,0);
//         glm::vec3 tan = fabs(intrs.n.y) == 1.f ? glm::vec3(1,0,0) : glm::normalize(k - ((glm::dot(k, intrs.n))*intrs.n)); //Gram-Schmidt Process
//         glm::vec3 bin = glm::cross(intrs.n, tan);
//         glm::mat3 xform = glm::mat3(tan, intrs.n, bin);
//         //For each sample
//         for (int i=0; i<sqsamps; ++i) {
//            for (int j=0; j<sqsamps; ++j) {
//               float u1 = (float)i/sqsamps + unifRand()/sqsamps;
//               float u2 = (float)j/sqsamps + unifRand()/sqsamps;
//               //glm::vec3 unitSamp = Samplers::CosineHemisphereSample(u1,u2);
//               //glm::vec3 unitSamp = Samplers::BadCosineHemisphereSample(u1,u2);
//               glm::vec3 unitSamp = Samplers::UniformHemisphereSample(u1,u2);
//               glm::vec3 sampleDir = glm::normalize(xform * unitSamp);
//               Ray ray = Ray(sampleDir, intrs.p+RAY_EPSILON*sampleDir);
//               Intersection* pIsect = RaySceneIntersect(ray, scene);
//               if (pIsect->hit) {
//                  indirectColor += ShadePoint(*pIsect, scene, recursionsLeft-1) * glm::vec3(intrs.surf.color) * glm::dot(intrs.n, ray.d);
//               }
//               delete pIsect;
//            }
//         }
//         indirectColor /= (float)scene.opts().monteCarloSamples;
//      }
//      return indirectColor;
//   }

//   glm::vec3 Tracer::CalcIndirectIllumPointBased(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
//   {
//      glm::vec3 indirectColor(0.f);
//      if (recursionsLeft > 0) {
//         int sqsamps = (int)sqrtf((float)scene.opts().monteCarloSamples);
//         //Construct basis for uniform sample -> intersection point transform
//         glm::vec3 k = glm::vec3(0,1,0);
//         glm::vec3 tan = fabs(intrs.n.y) == 1.f ? glm::vec3(1,0,0) : glm::normalize(k - ((glm::dot(k, intrs.n))*intrs.n)); //Gram-Schmidt Process
//         glm::vec3 bin = glm::cross(intrs.n, tan);
//         glm::mat3 xform = glm::mat3(tan, intrs.n, bin);
//         //For each sample
//         for (int i=0; i<sqsamps; ++i) {
//            for (int j=0; j<sqsamps; ++j) {
//               float u1 = (float)i/sqsamps + unifRand()/sqsamps;
//               float u2 = (float)j/sqsamps + unifRand()/sqsamps;
//               //glm::vec3 unitSamp = Samplers::CosineHemisphereSample(u1,u2);
//               //glm::vec3 unitSamp = Samplers::BadCosineHemisphereSample(u1,u2);
//               glm::vec3 unitSamp = Samplers::UniformHemisphereSample(u1,u2);
//               glm::vec3 sampleDir = glm::normalize(xform * unitSamp);
//               Ray ray = Ray(sampleDir, intrs.p+RAY_EPSILON*sampleDir);
//               Intersection* pIsect = scene.surfelCloud().intersect(ray); //Intersect against surfels... :/
//               if (pIsect->hit) {
//                  indirectColor += ShadePoint(*pIsect, scene, recursionsLeft-1) * glm::vec3(intrs.surf.color) * glm::dot(intrs.n, ray.d);
//                  //indirectColor += glm::vec3(intrs.surf.color) * glm::dot(intrs.n, ray.d);
//               }
//               delete pIsect;
//            }
//         }
//         indirectColor /= (float)scene.opts().monteCarloSamples;
//      }
//      return indirectColor;
//   }

   glm::vec3 Tracer::CalcReflection(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
   {
      glm::vec3 reflection(0.f);
      if (recursionsLeft > 0 && intrs.surf.finish.getReflection() > 0.f) {
         Ray reflectionRay = Ray(glm::reflect(intrs.incDir, intrs.n), intrs.p);
         reflectionRay.o += RAY_EPSILON * reflectionRay.d;
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
         refractionRay.o += RAY_EPSILON * refractionRay.d;
         if (glm::length(refractionRay.d) == 0.f) return refraction; //TIR
         refraction = TraceRay(refractionRay, scene, recursionsLeft-1);
      }
      return refraction;
   }
}

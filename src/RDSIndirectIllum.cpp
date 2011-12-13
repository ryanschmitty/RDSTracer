/**
* File: RDSIndirectIllum.cpp
* --------------------
* Implementation for Indirect Illumination functions.
* Author: Ryan Schmitt
*/

#include "RDSIndirectIllum.h"
#include "Camera.h"
#define GLM_FORCE_INLINE
#include <glm/gtx/rotate_vector.hpp>

namespace RDST
{
   Rasterizer::Rasterizer(int w, int h)
   : width(w), height(h)
   {
      glGenFramebuffers(1, &fbo);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);

      glGenTextures(1, &depthtexture);
      glBindTexture(GL_TEXTURE_2D, depthtexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthtexture, 0);

      glGenTextures(1, &colortexture);
      glBindTexture(GL_TEXTURE_2D, colortexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colortexture, 0);

      GLenum attachmentList[] = { GL_COLOR_ATTACHMENT0 };
      glDrawBuffers(1, attachmentList);

      GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if(status != GL_FRAMEBUFFER_COMPLETE) {
         printf("Rasterizer: Framebuffer loading error...\n");
         exit(-1);
      }

      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); //unbind fbo for now
   }


   glm::vec3 IndirectIllumMonteCarlo(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft)
   {
      glm::vec3 indirectColor(0.f);
      if (recursionsLeft > 0) {
         int sqsamps = (int)sqrtf((float)scene.opts().monteCarloSamples);
         //Construct basis for uniform sample -> intersection point transform
         glm::vec3 k = glm::vec3(0,1,0);
         glm::vec3 tan = fabs(intrs.n.y) == 1.f ? glm::vec3(1,0,0) : glm::normalize(k - ((glm::dot(k, intrs.n))*intrs.n)); //Gram-Schmidt Process
         glm::vec3 bin = glm::cross(intrs.n, tan);
         glm::mat3 xform = glm::mat3(tan, intrs.n, bin);
         //For each sample
         for (int i=0; i<sqsamps; ++i) {
            for (int j=0; j<sqsamps; ++j) {
               float u1 = (float)i/sqsamps + unifRand()/sqsamps;
               float u2 = (float)j/sqsamps + unifRand()/sqsamps;
               //glm::vec3 unitSamp = Samplers::CosineHemisphereSample(u1,u2);
               //glm::vec3 unitSamp = Samplers::BadCosineHemisphereSample(u1,u2);
               glm::vec3 unitSamp = Samplers::UniformHemisphereSample(u1,u2);
               glm::vec3 sampleDir = glm::normalize(xform * unitSamp);
               Ray ray = Ray(sampleDir, intrs.p+RAY_EPSILON*sampleDir);
               Intersection* pIsect = Tracer::RaySceneIntersect(ray, scene);
               if (pIsect->hit) {
                  indirectColor += Tracer::ShadePoint(*pIsect, scene, recursionsLeft-1) * glm::vec3(intrs.surf.color) * glm::dot(intrs.n, ray.d);
               }
               delete pIsect;
            }
         }
         indirectColor /= (float)scene.opts().monteCarloSamples;
      }
      return indirectColor;
   }

   glm::vec3 IndirectIllumSurfelRaster(const Intersection& intrs, const SceneDescription& scene)
   {
      glm::vec3 indirectColor(0.f);
      //Solve for tangent
      glm::vec3 k = glm::vec3(0,1,0);
      glm::vec3 tan = fabs(intrs.n.y) == 1.f ? glm::vec3(1,0,0) : glm::normalize(k - ((glm::dot(k, intrs.n))*intrs.n)); //Gram-Schmidt Process
      //Generate 6 cube faces (i.e. 5 cameras)
      ::Camera cameras [5];
      for (int i=0; i<4; ++i) {
         glm::vec3 dir(0.f);
         if      (i==0)   dir = tan;
         else if (i==1)   dir = glm::rotate(tan,  90.f, intrs.n);
         else if (i==2)   dir = glm::rotate(tan, 180.f, intrs.n);
         else  /*(i==3)*/ dir = glm::rotate(tan, 270.f, intrs.n);
         cameras[i] = ::Camera(90, //fov
                               intrs.p.x, intrs.p.y, intrs.p.z, //loc
                               dir.x, dir.y, dir.z, //dir
                               intrs.n.x, intrs.n.y, intrs.n.z, //up
                               0.01f, 100.f); //near/far
      } 
      cameras[4] = ::Camera(90, //fov
                            intrs.p.x, intrs.p.y, intrs.p.z, //loc
                            intrs.n.x, intrs.n.y, intrs.n.z, //dir
                            tan.x, tan.y, tan.z, //up
                            0.01f, 100.f); //near/far
      //Rasterize cube faces (i.e. render 8x8 texture per camera)
      //Add them all up
      return indirectColor;
   }
}

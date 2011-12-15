/**
 * File: RDSIndirectIllum.h
 * --------------------
 * Indirect Illumination Functions!
 * Author: Ryan Schmitt
 */

#ifndef _RDS_INDIRECT_ILLUM_H_
#define _RDS_INDIRECT_ILLUM_H_

#define GLM_FORCE_INLINE

#include <glew.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include "Camera.h"
#include "RDSScene.h"
#include "RDSSceneDesc.h"
#include "RDSSamplers.h"
#include "RDSTracer.h"

namespace RDST
{

class Rasterizer {
   public:
      explicit Rasterizer(int w, int h, const SceneDescription& desc);
      virtual ~Rasterizer() {};

      //Returns a texture handle
      GLuint rasterSurfels(::Camera& camera);
   private:
      void initGL();
      void loadVBO(const SceneDescription& desc);
      void geometry(const SceneDescription& desc);
      void view(::Camera& camera);
      void lights(const SceneDescription& desc);
      int width, height;
      GLuint fbo;
      GLuint vboId;
      GLuint depthtexture;
      GLuint colortexture;
      const SceneDescription& desc;
};

/**
 * Calculate Monte Carlo indirect illumination!
 */
glm::vec3 IndirectIllumMonteCarlo(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft);

/**
 * Calculate Surfel Raster indirect illumination!
 */
glm::vec3 IndirectIllumSurfelRaster(const Intersection& intrs, const SceneDescription& scene);

}

#endif

/**
 * File: RDSIndirectIllum.h
 * --------------------
 * Indirect Illumination Functions!
 * Author: Ryan Schmitt
 */

#ifndef _RDS_INDIRECT_ILLUM_H_
#define _RDS_INDIRECT_ILLUM_H_

#define GLM_FORCE_INLINE

#include <vector>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include "RDSScene.h"
#include "RDSSceneDesc.h"
#include "RDSSamplers.h"
#include "RDSTracer.h"

namespace RDST
{

/**
 * Calculate Monte Carlo indirect illumination!
 */
glm::vec3 IndirectIllumMonteCarlo(const Intersection& intrs, const SceneDescription& scene, unsigned int recursionsLeft);

}

#endif

/**
 * RDSSurfelGen.h
 * --------------------
 * Generates a surfel cloud.
 * Author: Ryan Schmitt
 */

#ifndef __RDS_SURFEL_GEN_H__
#define __RDS_SURFEL_GEN_H__

#include "RDSPointDistribution.h"
#include "RDSScene.h"
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>

namespace RDST
{
   /**
    * Surfel Generator class.
    */
   class SurfelGenerator
   {
   private:
      /* Hidden ctors */
      explicit SurfelGenerator()
      {}
      explicit SurfelGenerator(const SurfelGenerator& rhs)
      {}
      ~SurfelGenerator()
      {}

   public:
      static void GenerateSurfels(SceneDescription& scene) {
         boost::shared_ptr< std::vector<GeomObjectPtr> > surfels(new std::vector<GeomObjectPtr>());
         std::vector<GeomObjectPtr>::const_iterator cit = scene.objs().begin();
         for (; cit != scene.objs().end(); ++cit) {
            Sphere* pSphere = dynamic_cast<Sphere*>(cit->get());
            Triangle* pTriangle = dynamic_cast<Triangle*>(cit->get());
            Box* pBox = dynamic_cast<Box*>(cit->get());
            if (pSphere != NULL) {
               RDST::GenerateSurfels(*surfels, scene, *pSphere);
            }
            else if (pTriangle != NULL) {
               RDST::GenerateSurfels(*surfels, scene, *pTriangle);
            }
            else if (pBox != NULL) {
               RDST::GenerateSurfels(*surfels, scene, *pBox);
            }
         }
         scene.setSurfelCloud(surfels);
      }
   };
}
#endif

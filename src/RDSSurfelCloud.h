/**
 * RDSSurfelCloud.h
 * --------------------
 * A Surfel Cloud representation.
 * Author: Ryan Schmitt
 */

#ifndef __RDS_SURFEL_CLOUD_H__
#define __RDS_SURFEL_CLOUD_H__

#include "RDSScene.h"
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>

namespace RDST
{
   /**
    * Surfel Cloud class.
    */
   class SurfelCloud
   {
   public:
      explicit SurfelCloud(GeomObjectPtrListPtr pGeomObjects)
      : _pRawSurfelList(pGeomObjects)
      {}

      //Non-mutable
      const std::vector<GeomObjectPtr>& rawSurfels() const
      { return *_pRawSurfelList; }

   private:
      GeomObjectPtrListPtr _pRawSurfelList;
   };
   typedef boost::shared_ptr<SurfelCloud> SurfelCloudPtr;
}
#endif

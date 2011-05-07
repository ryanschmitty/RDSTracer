/**
 * RDSSceneDesc.h
 * --------------------
 * The description of a scene.
 * Author: Ryan Schmitt
 */

#ifndef __RDS_SCENE_DESC_H__
#define __RDS_SCENE_DESC_H__

#include "RDSScene.h"
#include "RDSbvh.h"

namespace RDST
{
   /**
    * Package class for all data required for a scene.
    */
   class SceneDescription
   {
   public:
      explicit SceneDescription()
      : _pCam(CameraPtr()),
        _pLights(PointLightPtrListPtr()),
        _pObjs(GeomObjectPtrListPtr()),
        _pPlanes(GeomObjectPtrListPtr()),
        _bvh(BVH(_pObjs))
      {}
      explicit SceneDescription(CameraPtr pCamera,
                                PointLightPtrListPtr lights,
                                GeomObjectPtrListPtr geometryObjects,
                                GeomObjectPtrListPtr planes,
                                const BVH& bvh)
      : _pCam(pCamera),
        _pLights(lights),
        _pObjs(geometryObjects),
        _pPlanes(planes),
        _bvh(bvh)
      {}

      //Mutable
      void setCam(CameraPtr pCamera)
      { _pCam = pCamera; }
      void setLights(PointLightPtrListPtr lights)
      { _pLights = lights; }
      GeomObjectPtrListPtr getObjectList()
      { return _pObjs; }
      void setObjs(GeomObjectPtrListPtr objs)
      { _pObjs = objs; }
      GeomObjectPtrListPtr getPlaneList()
      { return _pPlanes; }
      void setPlanes(GeomObjectPtrListPtr planes)
      { _pPlanes = planes; }
      void setBVH(const BVH& bvh)
      { _bvh = bvh; }

      //Non-mutable
      const Camera& cam() const
      { return *_pCam; }
      const std::vector<PointLightPtr>& lights() const
      { return *_pLights; }
      const std::vector<GeomObjectPtr>& objs() const
      { return *_pObjs; }
      const std::vector<GeomObjectPtr>& planes() const
      { return *_pPlanes; }
      const BVH& bvh() const
      { return _bvh; }

   private:
      CameraPtr            _pCam;
      PointLightPtrListPtr _pLights;
      GeomObjectPtrListPtr _pObjs;
      GeomObjectPtrListPtr _pPlanes;
      BVH                  _bvh;
   };
}

#endif
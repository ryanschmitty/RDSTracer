/**
 * RDSSceneDesc.h
 * --------------------
 * The description of a scene.
 * Author: Ryan Schmitt
 */

#ifndef __RDS_SCENE_DESC_H__
#define __RDS_SCENE_DESC_H__

#include "RDSScene.h"
#include "RDSSurfelCloud.h"
#include "RDSbvh.h"

namespace RDST
{
   /**
    * Package class for rendering options.
    */
   struct Options
   {
      enum FILTER_TYPE {BOX, GAUSSIAN, MITCHELL};
      Options()
         : povRayFile(""),
           imgname(""),
           height(480),
           width(640),
           enableGammaCorrection(false),
           gamma(2.2f),
           enableAA(false),
           jitter(false),
           subsamples(1),
           areaLightSamples(16),
           filter(BOX),
           gaussianAlpha(1.f),
           numThreads(8),
           monteCarloSamples(16),
           bounces(0)
      {}
      std::string povRayFile, imgname;
      int height, width;
      bool enableGammaCorrection;
      float gamma;
      bool enableAA;
      bool jitter;
      int subsamples;
      int areaLightSamples;
      FILTER_TYPE filter;
      float gaussianAlpha;
      int numThreads;
      int monteCarloSamples;
      int bounces;
   };

   /**
    * Package class for all data required for a scene.
    */
   class SceneDescription
   {
   public:
      explicit SceneDescription(CameraPtr pCamera,
                                PointLightPtrListPtr lights,
                                GeomObjectPtrListPtr areaLights,
                                GeomObjectPtrListPtr geometryObjects,
                                GeomObjectPtrListPtr planes,
                                const BVH& bvh,
                                SurfelCloudPtr surfels,
                                const Options& opts = Options())
      : _pCam(pCamera),
        _pLights(lights),
        _pLightObjs(areaLights),
        _pObjs(geometryObjects),
        _pPlanes(planes),
        _bvh(bvh),
        _opts(opts),
        _pSurfelCloud(surfels)
      {}

      //Mutable
      void setCam(CameraPtr pCamera)
      { _pCam = pCamera; }
      void setLights(PointLightPtrListPtr lights)
      { _pLights = lights; }
      void setLightObjs(GeomObjectPtrListPtr areaLights)
      { _pLightObjs = areaLights; }
      void setObjs(GeomObjectPtrListPtr objs)
      { _pObjs = objs; }
      void setPlanes(GeomObjectPtrListPtr planes)
      { _pPlanes = planes; }
      void setBVH(const BVH& bvh)
      { _bvh = bvh; }
      void setOpts(const Options& opts)
      { _opts = opts; }
      void setSurfelCloud(SurfelCloudPtr surfels)
      { _pSurfelCloud = surfels; }

      //Non-mutable
      const Camera& cam() const
      { return *_pCam; }
      const std::vector<PointLightPtr>& lights() const
      { return *_pLights; }
      const std::vector<GeomObjectPtr>& areaLights() const
      { return *_pLightObjs; }
      const std::vector<GeomObjectPtr>& objs() const
      { return *_pObjs; }
      const std::vector<GeomObjectPtr>& planes() const
      { return *_pPlanes; }
      const BVH& bvh() const
      { return _bvh; }
      const Options& opts() const
      { return _opts; }
      const SurfelCloud& surfelCloud() const
      { return *_pSurfelCloud; }

   private:
      CameraPtr            _pCam;
      PointLightPtrListPtr _pLights;
      GeomObjectPtrListPtr _pLightObjs;
      GeomObjectPtrListPtr _pObjs;
      GeomObjectPtrListPtr _pPlanes;
      BVH                  _bvh;
      Options              _opts;
      SurfelCloudPtr       _pSurfelCloud;
   };
}

#endif

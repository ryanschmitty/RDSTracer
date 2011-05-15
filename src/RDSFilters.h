/**
 * File: RDSFilters.h
 * --------------------
 * Some useful filter functions
 * Author: Ryan Schmitt
 */

#ifndef _RDS_FILTERS_H_
#define _RDS_FILTERS_H_

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>

namespace RDST
{
   class Filters
   {
   private:
      /* Hidden ctors */
      explicit Filters()
      {}
      explicit Filters(const Filters& rhs)
      {}
      ~Filters()
      {}
   public:
      static float MitchellFilter(float x, float y, float invWidth);
      static float GaussianFilter(float x, float y, float alpha);
   private:
      static float Mitchell1D(float x);
      static float Gaussian1D(float d, float alpha);
   };
}

#endif
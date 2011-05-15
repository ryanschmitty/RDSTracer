/**
* File: RDSFilters.cpp
* --------------------
* Useful filter function implementations
* Author: Ryan Schmitt
*/

#include "RDSFilters.h"

namespace RDST
{
   //Should follow B + 2C = 1
   const float B = 1.f/3.f;
   const float C = 0;

   float Filters::Mitchell1D(float x) {
      x = fabsf(2.f*x);
      if (x > 1.f)
         return ((-B - 6*C) * x*x*x + (6*B + 30*C) * x*x +
                 (-12*B - 48*C) * x + (8*B + 24*C)) * (1.f/6.f);
      else
         return ((12 - 9*B - 6*C) * x*x*x +
                 (-18 + 12*B + 6*C) * x*x +
                 (6 - 2*B)) * (1.f/6.f);
   }

   float Filters::MitchellFilter(float x, float y, float invWidth) {
      return Mitchell1D(x*invWidth) * Mitchell1D(y * invWidth);
   }

   float Filters::Gaussian1D(float d, float alpha) {
      return glm::max(0.f, float(expf(-alpha*d*d) - expf(-alpha)));
   }

   float Filters::GaussianFilter(float x, float y, float a) {
      return Gaussian1D(x, a) * Gaussian1D(y, a);
   }
}
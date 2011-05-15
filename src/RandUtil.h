
/**
 * File: RandUtil.h
 * --------------------
 * Utility functions for random variables.
 * Author: Ryan Schmitt
 */

#ifndef _RDS_RAND_UTIL_H_
#define _RDS_RAND_UTIL_H_

#include <ctime>
#include <cstdlib>

namespace RDST
{
   // Random number in [0,1]
   static float unifRand()
   {
      return float(rand()) / RAND_MAX;
   }

   // Random number in [a, b]
   static float unifRand(float a, float b)
   {
      return (b-a)*unifRand() + a;
   }

   //Seed the generator with system clock
   static void seed()
   {
      srand((unsigned int)time(NULL));
   }
}
#endif
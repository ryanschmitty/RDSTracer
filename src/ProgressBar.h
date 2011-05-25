
/**
 * File: ProgressBar.h
 * --------------------
 * Utility functions for printing progress bars.
 * Author: Ryan Schmitt
 */

#ifndef _RDS_PROGRESS_BAR_H_
#define _RDS_PROGRESS_BAR_H_

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace RDST
{
   /**
    * Draws the progress bar output, percentage is [0, 100].
    */
   static void UpdateProgress(int percent)
   {
      percent = glm::clamp(percent, 0, 100);
      std::ostringstream ss;
      ss << "Progress: " << std::setw(3) << percent << "% [";
      int barWidth = 40; //how many = make up the whole bar
      int progress = (int)(barWidth*percent/100.f);
      for (int i=0; i<barWidth; ++i) {
         if (i < progress) ss << '=';
         else ss << ' ';
      }
      std::cout << ss.str() << "]\r" << std::flush;
   }
}

#endif

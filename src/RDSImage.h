
/**
 * File: RDSImage.h
 * --------------------
 * 2D Image of RGBA values.
 * Author: Ryan Schmitt
 */

#ifndef _RDS_IMAGE_H_
#define _RDS_IMAGE_H_

#define GLM_FORCE_INLINE

#include <fstream>
#include <vector>
#include <boost/assert.hpp>
#include <glm/glm.hpp>

namespace RDST
{
   /**
    * Represents the RGBA values of a pixel
    */
   class Pixel
   {
   public:
      explicit Pixel(float red = 0.f,
                     float green = 0.f,
                     float blue = 0.f,
                     float alpha = 1.f)
      : _r(red),
        _g(green),
        _b(blue),
        _a(alpha)
      {}

      float r() const
      { return _r; }
      float g() const
      { return _g; }
      float b() const
      { return _b; }
      float a() const
      { return _a; }
      glm::vec4 rgba() const
      { return glm::vec4(_r,_g,_b,_a); }

      void setR(float red)
      { _r = red; }
      void setG(float green)
      { _g = green; }
      void setB(float blue)
      { _b = blue; }
      void setA(float alpha)
      { _a = alpha; }
      void set(float red, float green, float blue, float alpha)
      { setR(red); setG(green); setB(blue); setA(alpha); }
      void set(const glm::vec4& color)
      { set(color.r, color.g, color.b, color.a); }

   private:
      float _r, _g, _b, _a;
   };

   /**
    * Represents a 2D collection of Pixels (i.e. an Image).
    * Note that 0,0 is bottom left
    */
   class Image
   {
   public:
      explicit Image(int _width, int _height, bool _gammaCorrection=false, float _gamma=2.2f)
      : w(_width),
        h(_height),
        gammaCorrect(_gammaCorrection),
        gamma(_gamma)
      {
         image.resize(0); //force vector to clear
         image.resize(w*h); //Initialize all Pixels with default ctor
      }

      /* Dimension inquiries */
      int getWidth() const
      { return w; }
      int getHeight() const
      { return h; }
      int getNumPixels() const
      { return w*h; }

      /* 2D functions */
      Pixel& get(int x, int y) //mutable
      { return image.at(y*w+x); }
      const Pixel& get(int x, int y) const //non-mutable
      { return image.at(y*w+x); }
      void set(int x, int y, const Pixel& pixel)
      { image.at(y*w+x) = pixel; }

      /* 1D functions */
      Pixel& get(int i) //mutable
      { return image.at(i); }
      const Pixel& get(int i) const //non-mutable
      { return image.at(i); }
      void set(int i, const Pixel& pixel)
      { image.at(i) = pixel; }

      Image downSample(int samples)
      {
         float f_sqrt = sqrtf((float)samples);
         int i_sqrt = (int)f_sqrt;
         if (f_sqrt != i_sqrt) {
            std::cerr << "Anti-Aliasing sub-samples must be a perfect square.\n";
            exit(EXIT_FAILURE);
         }
         Image output(w/i_sqrt, h/i_sqrt);
         //Loop over the output buffer
         for (int y = 0; y < output.getHeight(); y++) {
            for (int x = 0; x < output.getWidth(); x++) {
               int superY = y*i_sqrt;
               int superX = x*i_sqrt;
               glm::vec4 finalColor(0.f);
               //Box filter all sub-samples
               for (int i=0; i<i_sqrt; ++i) {
                  for (int j=0; j<i_sqrt; ++j) {
                     finalColor += get(superX+i,superY+j).rgba() / float(samples);
                  }
               }
               output.get(x,y).set(finalColor);
            }
         }
         return output;
      }

      void writeToDisk(std::string& fname)
      {
         /* Write an uncompressed PPM, ADAPTED FROM: http://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C */
         //std::string fnameExtension = fname;
         //fnameExtension += ".ppm";
         //std::ofstream file(fnameExtension.c_str(), std::ofstream::binary); //binary open
         //if (file.fail()) {
         //   fprintf(stderr,"Failed to open output file: %s\n", fname.c_str());
         //   exit(-1);
         //}
         //// PPM Header (P6\n<width> <height>\n<max color>\n)
         //file << "P6\n" << w << " " << h << "\n255\n";
         //// data, 0,0 is top left...
         //for (int y = 0; y < h; ++y) {
         //   for (int x = 0; x < w; ++x) {
         //      //Clamp it (TODO: HDR)
         //      float red = glm::clamp(get(x,h-1-y).r(), 0.f, 1.f); //reverse because my Ray Tracer (and most other peoples') assumes 0,0 is bottom left not top left!
         //      float green = glm::clamp(get(x,h-1-y).g(), 0.f, 1.f);
         //      float blue = glm::clamp(get(x,h-1-y).b(), 0.f, 1.f);
         //      //Gamma Correction it
         //      if (gammaCorrect) {
         //         float inverseGamma = 1.f / gamma;
         //         red = powf(red, inverseGamma);
         //         green = powf(green, inverseGamma);
         //         blue = powf(blue, inverseGamma);
         //      }
         //      /*if (gammaCorrect) {
         //         red = srgbEncode(red);
         //         green = srgbEncode(green);
         //         blue = srgbEncode(blue);
         //      }*/
         //      //Write it
         //      file.put((int)(red*255));
         //      file.put((int)(green*255));
         //      file.put((int)(blue*255));
         //   }
         //}
         //file.close();

         /* Write an uncompressed TGA, ADAPTED FROM: http://paulbourke.net/dataformats/tga/tgatest.c */
         std::string fnameExtension = fname;
         fnameExtension += ".tga";
         std::ofstream file(fnameExtension.c_str(), std::ofstream::binary); //binary open
         if (file.fail()) {
            fprintf(stderr,"Failed to open output file: %s\n", fname.c_str());
            exit(-1);
         }
         //header
         file.put(0);
         file.put(0);
         file.put(2);                     /* uncompressed RGB */
         file.put(0); file.put(0);
         file.put(0); file.put(0);
         file.put(0);
         file.put(0); file.put(0);        /* X origin */
         file.put(0); file.put(0);        /* y origin */
         file.put((w & 0x00FF));
         file.put((w & 0xFF00) / 256);
         file.put((h & 0x00FF));
         file.put((h & 0xFF00) / 256);
         file.put(24);                    /* 32 bit bitmap (24 rgb 8 a) */
         file.put(0);
         //data
         for (std::vector<Pixel>::const_iterator it = image.begin(); it != image.end(); ++it) {
            float red = glm::clamp(it->r(), 0.f, 1.f); //reverse because my Ray Tracer (and most other peoples') assumes 0,0 is bottom left not top left!
            float green = glm::clamp(it->g(), 0.f, 1.f);
            float blue = glm::clamp(it->b(), 0.f, 1.f);
            file.put((int)(blue*255));
            file.put((int)(green*255));
            file.put((int)(red*255));
         }
         file.close();
      }

      float srgbEncode(float c)
      {
         if (c <= 0.0031308f)
         {
            return 12.92f * c; 
         }
         else
         {
            return 1.055f * powf(c, 0.4166667f) - 0.055f; // Inverse gamma 2.4
         }
      }

   private:
      int w, h;
      bool gammaCorrect;
      float gamma;
      std::vector<Pixel> image;
   };
} // end namespace RDST

#endif
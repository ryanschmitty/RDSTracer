
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
      explicit Image(const Image& inputBuffer, int superSamples)
         : w(inputBuffer.getWidth()/(superSamples/2)),
           h(inputBuffer.getHeight()/(superSamples/2))
      {
         BOOST_ASSERT(superSamples%2==0); //assert we've got an even number
         image.resize(0);
         image.resize(w*h);
         downSample(inputBuffer, superSamples);
      }
      explicit Image(short width, short height)
      : w(width),
        h(height)
      {
         image.resize(0); //force vector to clear
         image.resize(w*h); //Initialize all Pixels with default ctor
      }

      /* Dimension inquiries */
      short getWidth() const
      { return w; }
      short getHeight() const
      { return h; }

      /* 2D functions */
      Pixel& get(short x, short y) //mutable
      { return image.at(y*w+x); }
      const Pixel& get(short x, short y) const //non-mutable
      { return image.at(y*w+x); }
      void set(short x, short y, const Pixel& pixel)
      { image.at(y*w+x) = pixel; }

      /* 1D functions */
      Pixel& get(int i) //mutable
      { return image.at(i); }
      const Pixel& get(int i) const //non-mutable
      { return image.at(i); }
      void set(int i, const Pixel& pixel)
      { image.at(i) = pixel; }

      void writeToDisk(std::string& fname)
      {
         /* Write an uncompressed PPM, ADAPTED FROM: http://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C */
         std::string fnameExtension = fname;
         fnameExtension += ".ppm";
         std::ofstream file(fnameExtension.c_str(), std::ofstream::binary); //binary open
         if (file.fail()) {
            fprintf(stderr,"Failed to open output file: %s\n", fname.c_str());
            exit(-1);
         }
         // PPM Header (P6\n<width> <height>\n<max color>\n)
         file << "P6\n" << w << " " << h << "\n255\n";
         // data, 0,0 is top left...
         for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
               //Clamp it (TODO: HDR)
               float red = glm::clamp(get(x,h-1-y).r(), 0.f, 1.f); //reverse because my Ray Tracer (and most other peoples') assumes 0,0 is bottom left not top left!
               float green = glm::clamp(get(x,h-1-y).g(), 0.f, 1.f);
               float blue = glm::clamp(get(x,h-1-y).b(), 0.f, 1.f);
               //Gamma Correction it
               red = powf(red, 1.f / 2.2f); // 1/gamma
               green = powf(green, 1.f / 2.2f); // 1/gamma
               blue = powf(blue, 1.f / 2.2f); // 1/gamma
               //Write it
               file.put((int)(red*255));
               file.put((int)(green*255));
               file.put((int)(blue*255));
            }
         }
         file.close();

         /* Write an uncompressed TGA, ADAPTED FROM: http://paulbourke.net/dataformats/tga/tgatest.c */
         //std::string fnameExtension = fname;
         //fnameExtension += ".tga";
         //std::ofstream file(fnameExtension.c_str(), std::ofstream::binary); //binary open
         //if (file.fail()) {
         //   fprintf(stderr,"Failed to open output file: %s\n", fname.c_str());
         //   exit(-1);
         //}
         ////header
         //file.put(0);
         //file.put(0);
         //file.put(2);                     /* uncompressed RGB */
         //file.put(0); file.put(0);
         //file.put(0); file.put(0);
         //file.put(0);
         //file.put(0); file.put(0);        /* X origin */
         //file.put(0); file.put(0);        /* y origin */
         //file.put((w & 0x00FF));
         //file.put((w & 0xFF00) / 256);
         //file.put((h & 0x00FF));
         //file.put((h & 0xFF00) / 256);
         //file.put(24);                    /* 32 bit bitmap (24 rgb 8 a) */
         //file.put(0);
         ////data
         //for (std::vector<Pixel>::const_iterator it = image.begin(); it != image.end(); ++it) {
         //   float red = glm::clamp(it->getR(), 0.f, 1.f); //reverse because my Ray Tracer (and most other peoples') assumes 0,0 is bottom left not top left!
         //   float green = glm::clamp(it->getG(), 0.f, 1.f);
         //   float blue = glm::clamp(it->getB(), 0.f, 1.f);
         //   file.put((int)(blue*255));
         //   file.put((int)(green*255));
         //   file.put((int)(red*255));
         //}
         //file.close();
      }

   private:
      void downSample(const Image& inputBuffer, int superSamples)
      {
         BOOST_ASSERT(superSamples%2==0);
         int halfSS = superSamples/2;
         int ibMaxY = inputBuffer.getHeight()-1;
         for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
               int inputBufY = y*halfSS;
               int inputBufX = x*halfSS;
               glm::vec4 finalColor(0.f);
               //Box filter
               for (int i=0; i<superSamples/2; ++i) {
                  for (int j=0; j<superSamples/2; ++j) {
                     finalColor += inputBuffer.get(inputBufX+i,(ibMaxY)-(inputBufY+j)).rgba() / float(superSamples);
                  }
               }
               get(x,h-1-y).set(finalColor);
            }
         }
      }

      short w, h;
      std::vector<Pixel> image;
   };
} // end namespace RDST

#endif
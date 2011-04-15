
/**
 * File: RDSImage.h
 * --------------------
 * 2D Image of RGBA values.
 * Author: Ryan Schmitt
 */

#ifndef _RDS_IMAGE_H_
#define _RDS_IMAGE_H_

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
      : r(red),
        g(green),
        b(blue),
        a(alpha)
      {}

      float getR() const
      { return r; }
      float getG() const
      { return g; }
      float getB() const
      { return b; }
      float getA() const
      { return a; }
      glm::vec4 get() const
      { return glm::vec4(r,g,b,a); }

      void setR(float red)
      { r = red; }
      void setG(float green)
      { g = green; }
      void setB(float blue)
      { b = blue; }
      void setA(float alpha)
      { a = alpha; }
      void set(float red, float green, float blue, float alpha)
      { setR(red); setG(green); setB(blue); setA(alpha); }
      void set(const glm::vec4& color)
      { set(color.r, color.g, color.b, color.a); }

   private:
      float r, g, b, a;
   };

   /**
    * Represents a 2D collection of Pixels (i.e. an Image).
    * Note that 0,0 is bottom left
    */
   class Image
   {
   public:
      explicit Image(short width, short height, const std::string& filename)
      : w(width),
        h(height),
        fname(filename)
      {
         image.resize(0); //force vector to clear
         image.resize(w*h); //Initialize all Pixels with default ctor
      }

      /* filename get/set */
      const std::string& getFilename() const
      { return fname; }
      void setFilename(const std::string filename)
      { fname = filename; }

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

      void writeToDisk()
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
               float red = glm::clamp(get(x,h-1-y).getR(), 0.f, 1.f); //reverse because my Ray Tracer (and most other peoples') assumes 0,0 is bottom left not top left!
               float green = glm::clamp(get(x,h-1-y).getG(), 0.f, 1.f);
               float blue = glm::clamp(get(x,h-1-y).getB(), 0.f, 1.f);
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
      short w, h;
      std::string fname;
      std::vector<Pixel> image;
   };
} // end namespace RDST

#endif
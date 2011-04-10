
/**
 * File: RDSImage.h
 * --------------------
 * 2D Image of RGBA values.
 * Author: Ryan Schmitt
 */

#ifndef _IMAGE_SCHMITT_
#define _IMAGE_SCHMITT_

#include <fstream>
#include <vector>
#include <boost/assert.hpp>

namespace RDST
{
   /**
    * Represents the RGBA values of a pixel
    */
   class Pixel {
   public:
      Pixel()
         : r(0),
           g(0),
           b(0),
           a(0)
      {}
      Pixel(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
      : r(red),
        g(green),
        b(blue),
        a(alpha)
      {}
      Pixel(float red, float green, float blue, float alpha)
      { setf(red, green, blue, alpha); }

      /* unsinged char [0,255] operations */

      unsigned char getR() const
      { return r; }
      unsigned char getG() const
      { return g; }
      unsigned char getB() const
      { return b; }
      unsigned char getA() const
      { return a; }

      void setR(unsigned char red)
      { r = red; }
      void setG(unsigned char green)
      { g = green; }
      void setB(unsigned char blue)
      { b = blue; }
      void setA(unsigned char alpha)
      { a = alpha; }
      void set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
      { setR(red); setG(green); setB(blue); setA(alpha); }

      /* float [0,1] operations */

      float getRf() const
      { return (float)r/255.f; }
      float getGf() const
      { return (float)g/255.f; }
      float getBf() const
      { return (float)b/255.f; }
      float getAf() const
      { return (float)a/255.f; }

      void setRf(float red)
      { BOOST_ASSERT_MSG(red>=0.f && red<=1.f, "value not [0,1].\n"); r = (unsigned char)(red*255); }
      void setGf(float green)
      { BOOST_ASSERT_MSG(green>=0.f && green<=1.f, "value not [0,1].\n"); g = (unsigned char)(green*255); }
      void setBf(float blue)
      { BOOST_ASSERT_MSG(blue>=0.f && blue<=1.f, "value not [0,1].\n"); b = (unsigned char)(blue*255); }
      void setAf(float alpha)
      { BOOST_ASSERT_MSG(alpha>=0.f && alpha<=1.f, "value not [0,1].\n"); a = (unsigned char)(alpha*255); }
      void setf(float red, float green, float blue, float alpha)
      { setRf(red); setGf(green); setBf(blue); setAf(alpha); }

   private:
      unsigned char r, g, b, a;
   };

   /**
    * Represents a 2D collection of Pixels (i.e. an Image).
    * Note that 0,0 is bottom left
    */
   class Image {
   public:
      Image(short width, short height, const std::string& filename)
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
      int getWidth() const
      { return w; }
      int getHeight() const
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

      void writeToDisk() //FIXME: should this be const?
      {
         /* Write an uncompressed TGA, ADAPTED FROM: http://paulbourke.net/dataformats/tga/tgatest.c */
         //open file
         std::ofstream file(fname.c_str());
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
         file.put(32);                    /* 32 bit bitmap (24 rgb 8 a) */
         file.put(0);
         //data
         for (std::vector<Pixel>::const_iterator it = image.begin(); it != image.end(); ++it) {
            file.put(it->getB());
            file.put(it->getG());
            file.put(it->getR());
            file.put(it->getA());
         }
         file.close();
      }

   private:
      short w, h;
      std::string fname;
      std::vector<Pixel> image;
   };
} // end namespace RDST

#endif
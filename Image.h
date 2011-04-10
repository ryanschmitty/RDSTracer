
/**
 * File: Image.h
 * --------------------
 * 2D Image of RGBA values.
 * Author: Ryan Schmitt
 */

#ifndef _IMAGE_SCHMITT_
#define _IMAGE_SCHMITT_

#include <vector>

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
      {
         r = red;
         g = green;
         b = blue;
         a = alpha;
      }

   private:
      unsigned char r, g, b, a;
   };

   /**
    * Represents a 2D collection of Pixels (i.e. an Image).
    */
   class Image {
   public:
      Image(short width, short height)
      : w(width),
        h(height)
      {
         image.resize(0); //force vector to clear
         image.resize(w*h); //Initialize all Pixels
      }

      /* Dimension inquiries */
      int width() const
      { return w; }
      int height() const
      { return h; }

      /* 2D gets */
      Pixel& get(short x, short y) //mutable
      {
         return image.at(y*w+x); //at() used to check bounds
      }
      const Pixel& get(short x, short y) const //non-mutable
      {
         return image.at(y*w+x); //at() used to check bounds
      }

      /* 1D gets */
      Pixel& get(int i) //mutable
      {
         return image.at(i); //at() used to check bounds
      }
      const Pixel& get(int i) const //non-mutable
      {
         return image.at(i); //at() used to check bounds
      }

   private:
      short w, h;
      std::vector<Pixel> image;
   };
} // end namespace RDST

#endif
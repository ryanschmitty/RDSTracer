
/**
 * File: vec3.h
 * --------------------
 * 3D Vector for graphics purposes.
 * Author: Ryan Schmitt
 */

#ifndef _VEC_3_SCHMITT_
#define _VEC_3_SCHMITT_

#include <iostream>
#include <cmath>

const float PI = 3.14159265f;

template <typename t>
struct Vec3 {
   //Vars
   t x;
   t y;
   t z;
   
   //Constructors
   Vec3() : x(0),y(0),z(0) {}
   Vec3(t xVal, t yVal, t zVal) : x(xVal),y(yVal),z(zVal) {}
   Vec3(const Vec3 &v) : x(v.x),y(v.y),z(v.z) {}

   //Overloaded Operators
   bool  operator== (const Vec3&) const;
   Vec3  operator+  (const Vec3&) const;
   Vec3& operator+= (const Vec3&);
   Vec3  operator-  (const Vec3&) const;
   Vec3& operator-= (const Vec3&);
   Vec3  operator*  (const Vec3&) const;
   Vec3& operator*= (const Vec3&);
   Vec3  operator*  (const t&) const;
   Vec3& operator*= (const t&);
   Vec3  operator/  (const t&) const;
   Vec3& operator/= (const t&);

   //Functions
   t     length() const;
   t     lengthSquared() const;
   Vec3  normal() const;
   void  normalize();
   void  zero();
   t     dot(const Vec3&) const;
   Vec3  cross(const Vec3&) const;
   t     angleDegs(const Vec3&) const;
   t     angleRads(const Vec3&) const;
};

/**
 * Overloaded Operators
 */
/* Equality */
template <typename t>
bool Vec3<t>::operator== (const Vec3<t>& v) const {
   return (x==v.x && y==v.y && z==v.z);
}
/* Vector component-wise addition */
template <typename t>
Vec3<t> Vec3<t>::operator+ (const Vec3<t>& v) const {
   return Vec3<t>(x+v.x, y+v.y, z+v.z);
}
template <typename t>
Vec3<t>& Vec3<t>::operator+= (const Vec3<t>& v) {
   *this = *this + v;
   return *this;
}
/* Vector component-wise subtraction */
template <typename t>
Vec3<t> Vec3<t>::operator- (const Vec3<t>& v) const {
   return Vec3<t>(x-v.x, y-v.y, z-v.z);
}
template <typename t>
Vec3<t>& Vec3<t>::operator-= (const Vec3<t>& v) {
   *this = *this - v;
   return *this;
}
/* Vector component-wise multiplication */
template <typename t>
Vec3<t> Vec3<t>::operator* (const Vec3<t>& v) const {
   return Vec3<t>(x*v.x, y*v.y, z*v.z);
}
template <typename t>
Vec3<t>& Vec3<t>::operator*= (const Vec3<t>& v) {
   *this = *this * v;
   return *this;
}
/* Scalar multiplication */
template <typename t>
Vec3<t> Vec3<t>::operator* (const t& scalar) const {
   return Vec3<t>(x*scalar, y*scalar, z*scalar);
}
template <typename t>
Vec3<t>& Vec3<t>::operator*= (const t& scalar) {
   *this = *this * scalar;
   return *this;
}
/* Scalar division */
template <typename t>
Vec3<t> Vec3<t>::operator/ (const t& scalar) const {
   return Vec3<t>(x/scalar, y/scalar, z/scalar);
}
template <typename t>
Vec3<t>& Vec3<t>::operator/= (const t& scalar) {
   *this = *this / scalar;
   return *this;
}
/* Stream insertion operator */
template <typename t>
std::ostream& operator<< (std::ostream &out, const Vec3<t>& v) {
   out << "<" << v.x << ", " << v.y << ", " << v.z << ">";
   return out;
}

/**
 * Functions
 */
template <typename t>
t Vec3<t>::length() const {
   return sqrt(lengthSquared());
}
template <typename t>
t Vec3<t>::lengthSquared() const {
   return (x*x+y*y+z*z);
}
template <typename t>
Vec3<t> Vec3<t>::normal() const {
   t len = length();
   if (len == 0) {
      std::cerr << "ERROR: call to normal() on a vector with length 0." << std::endl;
      return *this;
   }
   return *this/len;
}
template <typename t>
void Vec3<t>::normalize() {
   t len = length();
   if (len == 0) {
      std::cerr << "ERROR: call to normalize() on a vector with length 0." << std::endl;
      return;
   }
   x /= len;
   y /= len;
   z /= len;
}
template <typename t>
void Vec3<t>::zero() {
   x=y=z=0;
}
template <typename t>
t Vec3<t>::dot(const Vec3& v) const {
   return (x*v.x + y*v.y + z*v.z);
}
template <typename t>
Vec3<t> Vec3<t>::cross(const Vec3& v) const {
   return Vec3<t>((y*v.z)-(z*v.y),(z*v.x)-(x*v.z),(x*v.y)-(y*v.x));
}
template <typename t>
t Vec3<t>::angleDegs(const Vec3& v) const  {
   t len = length();
   t vlen = v.length();
   if (len == 0 || vlen == 0) {
      std::cerr << "ERROR: call to angleDegs() on a vector with length 0." << std::endl;
      return NULL;
   }
   return acos(dot(v)/(len*vlen)) * (180.0f/PI);
}
template <typename t>
t Vec3<t>::angleRads(const Vec3& v) const  {
   t len = length();
   t vlen = v.length();
   if (len == 0 || vlen == 0) {
      std::cerr << "ERROR: call to angleRads() on a vector with length 0." << std::endl;
      return NULL;
   }
   return acos(dot(v)/(len*vlen));
}

//Some useful types
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;

#endif
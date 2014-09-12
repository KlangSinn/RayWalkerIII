// -----------------------------------------------------------------------------
// Copyright  : (C) 2014 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Color
// -----------------------------------------------------------------------------

#ifndef BUW_COLOR_HPP
#define BUW_COLOR_HPP

#include <iostream>

struct Color
{
	
	Color() : r(0), g(0), b(0) {}
	Color(float red, float green, float blue) : r(red), g(green), b(blue) {}
	float r;
	float g;
	float b;

  friend std::ostream& operator<<(std::ostream& os, Color const& c)
  {
    os << "(" << c.r << "," << c.g << "," << c.b << ")\n";
    return os;
  }

  Color& operator+=(Color const& other)
  {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
  }

  Color& operator-=(Color const& other)
  {
    r -= other.r;
    g -= other.g;
    b -= other.b;
    return *this;
  }

  friend Color operator+(Color const& a, Color const& b)
  {
    auto tmp(a);
    tmp += b;
    return tmp;
  }

  friend Color operator-(Color const& a, Color const& b)
  {
    auto tmp(a);
    tmp -= b;
    return tmp;
  }

  Color clip() {
		double alllight = r + g + b;
		double excesslight = alllight - 3;
		if(excesslight > 0) {
			r = r + excesslight * (r/alllight);
			g = g + excesslight * (g/alllight);
			b = b + excesslight * (b/alllight);			
		}
		if(r > 1) { r = 1; }
		if(g > 1) { g = 1; }
		if(b > 1) { b = 1; }
		if(r < 0) { r = 0; }
		if(g < 0) { g = 0; }
		if(b < 0) { b = 0; }

		return Color(r, g, b);
	}	
};

#endif //#define BUW_COLOR_HPP

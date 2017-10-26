#pragma once

#include <cmath>

struct Point
{
   double x, y;

   bool operator==(const Point& p) const { return std::abs(x - p.x) < 1e-9 && std::abs(y - p.y) < 1e-9; }
   double operator%(const Point& p) const { return x*p.x + y*p.y; }
   double operator*(const Point& p) const { return x*p.y - y*p.x; }
   Point operator/(const double value) const { return { x / value, y / value }; }
   Point operator-(const Point& p) const { return { x - p.x, y - p.y }; }
   Point operator+(const Point& p) const { return { x + p.x, y + p.y }; }
};


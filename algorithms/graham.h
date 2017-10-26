#pragma once

#include <algorithm>
#include <vector>

#include "point.h"

namespace algorithms
{

std::vector<Point> GrahamScan(std::vector<Point> points)
{
   std::vector<Point> result;
   result.reserve(points.size());
   for (size_t i = 1; i < points.size(); ++i)
      if (points[0].x > points[i].x) std::swap(points[0], points[i]);

   sort(points.begin() + 1, points.end(), [&](const Point& left, const Point& right)
   {
      return (left - points[0]) * (right - points[0]) > 0.0;
   });

   for (const auto& point : points)
   {
      while (result.size() > 2 && (result.back() - point) * (result[result.size() - 2] - result.back()) > 0.0)
         result.pop_back();
      result.push_back(point);
   }
   return result;
}

}

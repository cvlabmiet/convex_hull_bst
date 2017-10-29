#pragma once

#include <algorithm>
#include <vector>

#include "point.h"

namespace algorithms
{
template<class It>
std::vector<Point> GrahamScanInPlace(It begin, It end)
{
   if (begin == end)
      return {};

   // TODO: find not just leftmost, but leftmost, that is at the bottom
   std::vector<Point> result;
   result.reserve(std::distance(begin, end));
   for (auto it = std::next(begin); it != end; ++it)
      if (begin->x > it->x) std::swap(*begin, *it);

   sort(begin + 1, end,
      [leftMostPoint = *begin](const Point& left, const Point& right)
      {
         return (left - leftMostPoint) * (right - leftMostPoint) > 0.0;
      });

   for (auto it = begin; it != end; ++it)
   {
      const Point& point = *it;
      while (result.size() > 2 &&
         (result.back() - point) * (result[result.size() - 2] - result.back()) > 0.0)
         result.pop_back();
      result.push_back(point);
   }
   return result;
}

std::vector<Point> GrahamScan(std::vector<Point> points)
{
   return GrahamScanInPlace(points.begin(), points.end());
}

}

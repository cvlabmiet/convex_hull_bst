#pragma once

#include <set>
#include <vector>

#include "point.h"

namespace algorithms
{

struct BstConvexHull
{
   void AddPoint(const Point& pt)
   {
      const Point point = pt - center;
      if (convexHull.size() < 2)
      {
         convexHull.emplace(point);
         return;
      }
      auto next = getIt(convexHull.upper_bound(point));
      auto prev = prevIt(next);

      if ((point - *prev) * (*next - *prev) > 0.0)
      {
         // works not well on it and pr on different sides of tree
         convexHull.emplace_hint(prev, point);

         while ((*nextIt(next) - point) * (*next - point) > 0.0)
            next = getIt(convexHull.erase(next));
         while ((*prevIt(prev) - point) * (*prev - point) < 0.0)
            prev = prevIt(convexHull.erase(prev));
      }
   }

   BstConvexHull(Point center) : center(center) {}

   static BstConvexHull Create(const std::vector<Point>& points)
   {
     Point maxPoint = points[0];
     Point minPoint = points[1];
     if (maxPoint.x < minPoint.x)
        std::swap(minPoint, maxPoint);

     for (const Point& point : points)
     {
        if (point.x > maxPoint.x) maxPoint = point;
        if (point.x < minPoint.x) minPoint = point;
     }
     BstConvexHull convexHull{ (minPoint + maxPoint) / 2 };
     convexHull.AddPoint(minPoint);
     convexHull.AddPoint(maxPoint);

     for (const Point& point : points)
        convexHull.AddPoint(point);

     return convexHull;
   }

private:
   struct less
   {
      bool operator()(const Point& left, const Point& right) const
      {
         const bool leftUp =  0 < left.y;
         const bool rightUp = 0 < right.y;
         if (leftUp != rightUp)
            return leftUp < rightUp;
         if (left.y == right.y && left.y == 0)
            return (0 < left.x) < (0 < right.x);

         return left * right > 0.0;
      }
   };

   std::set<Point, less>::iterator getIt(std::set<Point, less>::iterator it) const noexcept
   {
      return it != convexHull.end() ? it : convexHull.begin();
   }
   std::set<Point, less>::iterator nextIt(std::set<Point, less>::iterator it) const noexcept
   {
      return getIt(std::next(it));
   }
   std::set<Point, less>::iterator prevIt(std::set<Point, less>::iterator it) const noexcept
   {
      return it != convexHull.begin() ? std::prev(it) : std::prev(convexHull.end());
   }

   Point center;
   std::set<Point, less> convexHull;
};

}

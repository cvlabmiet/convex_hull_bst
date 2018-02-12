#pragma once

#include <set>
#include <vector>

#include <boost/pool/pool_alloc.hpp>

#include "point.h"

namespace algorithms
{

struct BstConvexHull
{
   void AddPoint(const Point& pt)
   {
      const Point point = pt - center;
      auto next = getIt(convexHull.upper_bound(point));
      auto prev = prevIt(next);

      if (ccw(*prev, point, *next))
      {
         while (true)
         {
            const auto afterNext = nextIt(next);
            if (ccw(point, *next, *afterNext))
               break;
            convexHull.erase(next);
            next = afterNext;
         }

         while (true)
         {
            const auto beforePrev = prevIt(prev);
            if (ccw(*beforePrev, *prev, point))
               break;
            convexHull.erase(prev);
            prev = beforePrev;
         }
         convexHull.emplace_hint(prev, point);
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
     Point furthestPoint = maxPoint;
     double furthestDist = 0;


     const auto divideVect = maxPoint - minPoint;
     for (const Point& point : points)
     {
        const double dist = abs((point - minPoint) * divideVect);
        if (dist > furthestDist)
        {
           furthestPoint = point;
           furthestDist = dist;
        }
     }

     BstConvexHull convexHull{ (minPoint + maxPoint + furthestPoint) / 3 };
     convexHull.convexHull.emplace(minPoint - convexHull.center);
     convexHull.convexHull.emplace(maxPoint - convexHull.center);
     convexHull.convexHull.emplace(furthestPoint - convexHull.center);

     for (const Point& point : points)
        convexHull.AddPoint(point);

     return convexHull;
   }

   struct less
   {
      bool operator()(const Point& left, const Point& right) const noexcept
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

   // TODO: I never free singleton memory
   using setallocator = boost::fast_pool_allocator<Point,
         boost::default_user_allocator_new_delete, boost::details::pool::default_mutex, 1048576>;

   const std::set<Point, less, setallocator>& GetPoints() const noexcept { return convexHull; }
   const Point& GetCenter() const noexcept { return center; }

private:
   bool ccw(const Point& a, const Point& b, const Point& c) const noexcept
   {
      return (b.x*c.y - c.x*b.y) -
         (a.x*c.y - c.x * a.y) +
         (a.x*b.y - b.x*a.y) > 0;
   }

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

   const Point center;
   std::set<Point, less, setallocator> convexHull;
};

}

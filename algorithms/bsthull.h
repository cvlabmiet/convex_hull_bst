#pragma once

#include <set>
#include <vector>

#include "point.h"

namespace algorithms
{
namespace impl
{
bool ccw(const Vector& a, const Vector& b, const Vector& c) noexcept
{
   return (b.x()*c.y() - c.x()*b.y()) -
      (a.x()*c.y() - c.x() * a.y()) +
      (a.x()*b.y() - b.x()*a.y()) > 0;
}

struct less
{
   bool operator()(const Vector& left, const Vector& right) const noexcept
   {
      const bool leftUp =  0 < left.y();
      const bool rightUp = 0 < right.y();
      if (leftUp != rightUp)
         return leftUp < rightUp;
      if (left.y() == right.y() && left.y() == 0)
         return (0 < left.x()) < (0 < right.x());

      return ccw(left, right, Vector(0.0, 0.0));
   }
};

struct CyclicSet
{
   using settype = std::set<Vector, less>;
   using iterator = settype::iterator;

   auto upper_bound(const Vector& vect) const noexcept
   {
      const auto it = innerset.upper_bound(vect);
      return it != innerset.end() ? it : innerset.begin();
   }

   auto next(const iterator& it) const noexcept
   {
      const auto nextIt = std::next(it);
      return nextIt != innerset.end() ? nextIt : innerset.begin();
   }

   auto prev(const iterator& it) const noexcept
   {
      return it != innerset.begin() ? std::prev(it) : std::prev(innerset.end());
   }

   void erase(const iterator& it) noexcept
   {
      innerset.erase(it);
   }

   void emplace_hint(const iterator& it, const Vector& vect) noexcept
   {
      innerset.emplace_hint(it, vect);
   }

   void emplace(const Vector& vect) noexcept
   {
      innerset.emplace(vect);
   }

   settype innerset;
};
}

struct BstConvexHull
{
   BstConvexHull(Point center) : center(center) {}

   void AddPoint(const Point& pt)
   {
      using impl::ccw;

      const Vector vector = pt - center;
      auto next = hull.upper_bound(vector);
      auto prev = hull.prev(next);

      if (ccw(*prev, vector, *next))
      {
         while (true)
         {
            const auto afterNext = hull.next(next);
            if (ccw(vector, *next, *afterNext))
               break;
            hull.erase(next);
            next = afterNext;
         }

         while (true)
         {
            const auto beforePrev = hull.prev(prev);
            if (ccw(*beforePrev, *prev, vector))
               break;
            hull.erase(prev);
            prev = beforePrev;
         }
         hull.emplace_hint(prev, vector);
      }
   }

   static BstConvexHull Create(const std::vector<Point>& points)
   {
     Point maxPoint = points[0];
     Point minPoint = points[1];
     if (maxPoint.x() < minPoint.x())
        std::swap(minPoint, maxPoint);

     for (const Point& point : points)
     {
        if (point.x() > maxPoint.x()) maxPoint = point;
        if (point.x() < minPoint.x()) minPoint = point;
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

     BstConvexHull convexHull{ Point(
        (minPoint.x() + maxPoint.x() + furthestPoint.x()) / 3,
        (minPoint.y() + maxPoint.y() + furthestPoint.y()) / 3) };

     convexHull.hull.emplace(minPoint - convexHull.center);
     convexHull.hull.emplace(maxPoint - convexHull.center);
     convexHull.hull.emplace(furthestPoint - convexHull.center);

     for (const Point& point : points)
        convexHull.AddPoint(point);

     return convexHull;
   }

   const impl::CyclicSet::settype& GetPoints() const noexcept { return hull.innerset; }
   const Point& GetCenter() const noexcept { return center; }

private:
   impl::CyclicSet hull;
   const Point center;
};

}

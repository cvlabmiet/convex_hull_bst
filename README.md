# Convex Hull Calculation Approach based on BST

[![Build Status](https://travis-ci.org/DimaWoogy/convex_hull_bst.svg?branch=master)](https://travis-ci.org/DimaWoogy/convex_hull_bst)

## Abstract
The repository contains algorithm based on binary search tree using, which have average case time complexity O(N*log(H)) where N is the size of the input set of points and H is the number of vertices found to be on the convex hull. The algorithm calculates the approximate center and stores all convex hull points in balanced binary search tree by the angle from it. Implementation of the new algorithm is written in C++ and tested against well-known algorithms. Our approach works better on low percentage of points in hull which is common case in convex hull calculation.

## License
convex_hull_bst is lincesed under the MIT. Full details in LICENSE.md.

## Algorithms comparison
This algorithm is better than the most popular Graham Scan on low percentage of points.
You can see results of testing our algorithm against Graham Scan on this chart:

![Comparison for 100000 number of points](https://user-images.githubusercontent.com/7341039/35782511-0efc6bf4-0a0a-11e8-91dc-1915fcb06527.png)

In this table you can see the ratio of the working time of the new algorithm to the Graham Scan:

![The ratio of the working time of the new algorithm to the Graham Scan](https://user-images.githubusercontent.com/7341039/35782561-c56158c8-0a0a-11e8-8433-d180e0df00d8.png)


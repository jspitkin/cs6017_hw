#pragma once

#include "Point.hpp"
#include <vector>

template<int Dimension> //The dimension of the points.  This can be used like any other constant within.
class BucketKNN{


public:

    BucketKNN(const std::vector<Point<Dimension> >& points, int divisions) {
        
        // Get the bounds for each dimension
        AABB bounds = getBounds(points);
        mins_ = bounds.mins;
        maxes_ = bounds.maxs;
        divisions_ = divisions;

        // Initialize the buckets
        for (int i = 0; i < std::pow(divisions, Dimension); i++) {
            buckets_.push_back({});
        }

        // Calculate the box size for each dimension
        for (int i = 0; i < Dimension; i++) {
            tile_sizes_[i] = (maxes_[i] - mins_[i]) / divisions;
        }

        // Place the points into the buckets
        for (const Point<Dimension>& p : points) {
            std::cout << p << std::endl;
            int index = getIndex(p);
            buckets_[index].push_back(p);
        }

        std::cout << std::endl;

    }
  

    std::vector<Point<Dimension>> rangeQuery(const Point<Dimension>& p, float radius) const{

        std::vector<Point<Dimension>> in_range = {};

        std::cout << "point: " << p << std::endl;
        for (std::vector<Point<Dimension>> bucket : buckets_) {
            AABB<Dimension> bucket_AABB = getBounds(bucket);
            Point<Dimension> closest_point = bucket_AABB.closestInBox(p);
            if (distance(closest_point, p) <= radius) {
                for (Point<Dimension> point : bucket) {
                    if (distance(point, p) <= radius) {
                        std::cout << "Distance: " << distance(point, p) << std::endl;
                        std::cout << "Added:  " << point << std::endl << std::endl;
                        in_range.push_back(p);
                    }
                }
            }
            
        }

        return in_range;
    }


    std::vector<Point<Dimension> > KNN(const Point<Dimension>& p, int k) const{

        return {};

    }

  


private:
    int divisions_;
    std::array<float, Dimension> tile_sizes_;
    std::array<float, Dimension> mins_;
    std::array<float, Dimension> maxes_;
    std::vector<std::vector<Point<Dimension>>> buckets_; 

    std::array<int, Dimension> findBucket(const Point<Dimension> p) {
        std::array<int, Dimension> bucket;
        for (int i = 0; i < Dimension; i++) {
            if (p[i] == maxes_[i]) 
                bucket[i] = divisions_ - 1;
            else 
                bucket[i] = std::floor((p[i] - mins_[i]) / tile_sizes_[i]);
        }
        return bucket;
    }


    int getIndex(const Point<Dimension> p) {
        std::array<int, Dimension> bucket = findBucket(p);
        int index = 0;
        for (int i = 0; i < Dimension; i++) {
            index += bucket[i] * std::pow(divisions_, i);
        }
        return index;
    }

};

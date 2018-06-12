#pragma once

#include "Point.hpp"
#include <vector>

template<int Dimension> //The dimension of the points.  This can be used like any other constant within.
class BucketKNN{


public:

    BucketKNN(const std::vector<Point<Dimension> >& points, int divisions) {
        AABB bounds = getBounds(points);
        mins_ = bounds.mins;
        maxes_ = bounds.maxs;
        divisions_ = divisions;
        for (int i = 0; i < std::pow(divisions, Dimension); i++) {
            buckets_.push_back({});
        }
        for (int i = 0; i < Dimension; i++) {
            tile_sizes_[i] = (maxes_[i] - mins_[i]) / divisions;
        }
        for (const Point<Dimension>& p : points) {
            std::cout << "Adding: " << p << std::endl;
            int index = getIndex(p);
            buckets_[index].push_back(p);
        }
    }
  

    std::vector<Point<Dimension>> rangeQuery(const Point<Dimension>& p, float radius) const{
        std::vector<Point<Dimension>> in_range = {};
        for (std::vector<Point<Dimension>> bucket : buckets_) {
            AABB<Dimension> bucket_AABB = getBounds(bucket);
            Point<Dimension> closest_point = bucket_AABB.closestInBox(p);
            if (distance(closest_point, p) <= radius) {
                for (Point<Dimension> point : bucket) {
                    if (distance(point, p) <= radius) {
                        in_range.push_back(point);
                    }
                }
            }
            
        }
        return in_range;
    }


    std::vector<Point<Dimension>> KNN(const Point<Dimension>& p, int k) const{
        std::vector<Point<Dimension>> neighbours = {};
        int radius = 1;
        while (neighbours.size() < k) {
            std::vector<Point<Dimension>> query_points = rangeQuery(p, radius);
            if (query_points.size() >= k) {
                for (Point<Dimension> n : query_points) {
                    if (neighbours.size() == k) 
                        break;
                    neighbours.push_back(n);
                }

            }
            radius *= 2;
        }
        return neighbours;
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

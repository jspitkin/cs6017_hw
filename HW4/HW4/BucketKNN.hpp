#pragma once

#include "Point.hpp"
#include <vector>

template<int Dimension> //The dimension of the points.  This can be used like any other constant within.
class BucketKNN{


public:

    BucketKNN(const std::vector<Point<Dimension> >& points, int divisions) {
        std::cout << "BucketKNN Init" << std::endl; 
        
        // Get the bounds for each dimension
        AABB bounds = getBounds(points);
        mins_ = bounds.mins;
        maxes_ = bounds.maxs;
        divisions_ = divisions;

        // Initialize the buckets
        std::vector<std::vector<Point<Dimension>>> 
            buckets_ (std::pow(divisions, Dimension), std::vector<Point<Dimension>>());


        // Calculate the box size for each dimension
        for (int i = 0; i < Dimension; i++) {
            tile_sizes_[i] = (maxes_[i] - mins_[i]) / divisions;
        }

        // Place the points into the buckets
        for (const Point<Dimension>& p : points) {
            int index = getIndex(p);
            std::cout << index << std::endl;
            buckets_[index].push_back(p);
        }
    }
  

    std::vector<Point<Dimension> > rangeQuery(const Point<Dimension>& p, float radius) const{

        return {};
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

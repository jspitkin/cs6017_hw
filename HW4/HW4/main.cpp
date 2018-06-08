#include <iostream>
#include "BucketKNN.hpp"
#include "Point.hpp"


template<int Dimension>
std::vector<Point<Dimension>> generate_points(int n_samples);
float random_float(float min, float max);


int main(int argc, const char * argv[]) {
  const int dimension = 5;
  const int n_samples = 100;
  int divisions = 10;
  const std::vector<Point<dimension>> points = generate_points <dimension> (n_samples);;
  BucketKNN test = BucketKNN(points, divisions);
  for (int i = 0; i < n_samples; i ++) {
      std::cout << points[i] << std::endl;
  }
  std::cout << "Hello, World!\n";
  return 0;
}


template<int Dimension>
std::vector<Point<Dimension>> generate_points(int n_samples) {
    const int min_range = -100;
    const int max_range = 100;
    std::vector<Point<Dimension>> points;
    for (int i = 0; i < n_samples; i++) {
        std::array<float, Dimension> point;
        for (int j = 0; j < Dimension; j++) {
           point[j] = random_float(min_range, max_range);
        } 
        Point<Dimension> p{point};
        points.push_back(p);
    }
    return points;
}


float random_float(float min, float max) {
    return (max - min) * ((((float) rand()) / (float) RAND_MAX)) + min;
}

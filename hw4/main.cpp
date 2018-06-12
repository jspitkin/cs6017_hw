#include <iostream>
#include "BucketKNN.hpp"
#include "Generators.hpp"
#include "Point.hpp"


template<int Dimension>
const std::vector<Point<Dimension>> generate_points(int n_samples);
float random_float(float min, float max);
void test_bucketing();


int main(int argc, const char * argv[]) {
    test_bucketing();

    return 0;
}

void test_bucketing() {
    const int dimension = 2;
    float min = 0;
    float max = 99;
    int n_samples = 10;
    int k = 2;
    int divisions = 10;

    std::vector<Point<dimension>> points;
    UniformGenerator generator = UniformGenerator <dimension> (min, max);
    for (int i = 0; i < n_samples; i++) {
        points.push_back(generator.generatePoint());
    }
    BucketKNN bucket = BucketKNN(points, divisions);
    Point<dimension> p = generator.generatePoint();
    std::vector<Point<dimension>> neighbours = bucket.KNN(p, 3);
    std::cout << "Point: " << p << std::endl;
    for (Point<dimension> n : neighbours) {
        std::cout << "Neighbour: " << n << std::endl;
    }
}

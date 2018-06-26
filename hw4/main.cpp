#include <iostream>
#include "BucketKNN.hpp"
#include "Generators.hpp"
#include "KDTree.hpp"
#include "Point.hpp"


void test_bucketing();
void test_KDTree();


int main(int argc, const char * argv[]) {
    //test_bucketing();
    test_KDTree();

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

void test_KDTree() {
    const int dimension = 1;
    int *dim_pointer = (int *) (&dimension);
    float min = 0;
    float max = 99;
    int n_samples = 3;
    int k = 2;

    for (int j = 2; j < 5; j++) {
        std::cout << "Variable: " << dimension << std::endl;
        std::vector<Point<dimension>> points;
        UniformGenerator<dimension> generator = UniformGenerator <dimension> (min, max);
        KDTree tree = KDTree(points);
        *dim_pointer = j;
    }
    return;

    std::vector<Point<1>> points;
    UniformGenerator<1> generator = UniformGenerator <1> (min, max);
    for (int i = 0; i < n_samples; i++) {
        Point<dimension> p = generator.generatePoint();
        points.push_back(p);
    }
    KDTree<1> tree = KDTree<1>(points);
    Point<dimension> p = generator.generatePoint();
    std::cout << "Point: " << p << std::endl;
    //tree.rangeQuery(p, 1);
    std::vector<Point<dimension>> neighbours = tree.KNN(p, 1);
    //for (Point<dimension> n : neighbours) {
    //    std::cout << "Neighbour: " << n << std::endl;
    //}
}

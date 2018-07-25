#include <iostream>
#include "BucketKNN.hpp"
#include "Generators.hpp"
#include "KDTree.hpp"
#include "Point.hpp"
#include "QuadTree.hpp"


void test_bucketing();
void test_kdtree();
void test_quadtree();


int main(int argc, const char * argv[]) {
    test_bucketing();
    test_kdtree();
    test_quadtree();

    return 0;
}

void test_bucketing() {
    const int dimension = 4;
    float min = -100;
    float max = 100;
    int n_samples = 10000;
    int k = 5;
    int divisions = 10;
    float radius = 20;

    std::vector<Point<dimension>> points;
    std::vector<Point<dimension>> rq_points;
    UniformGenerator generator = UniformGenerator <dimension> (min, max);
    Point<dimension> qp = generator.generatePoint();
    DistanceComparator<dimension> cmp{qp};
    for (int i = 0; i < n_samples; i++) {
      Point<dimension> p = generator.generatePoint();
      if (distance(p, qp) <= radius) {
        rq_points.push_back(p);
      }
      points.push_back(p);
    }
    BucketKNN bucket = BucketKNN(points, divisions);
    std::vector knn = bucket.KNN(qp, k);
    std::vector range_points = bucket.rangeQuery(qp, radius);
    std::sort(points.begin(), points.end(), cmp);
    std::sort(rq_points.begin(), rq_points.end(), cmp);
    std::sort(range_points.begin(), range_points.end(), cmp);
    std::sort(knn.begin(), knn.end(), cmp);
    points.resize(k);
    for (int i = 0; i < k; i++) {
      if (distance(points[i], knn[i]) != 0) {
        std::cout << "bucket knn failed on " << i << std::endl;
      }
    }
    std::cout << "bucket knn passed" << std::endl << std::endl;

    for (int i = 0; i < rq_points.size(); i++) {
      if (distance(range_points[i], rq_points[i]) != 0) {
        std::cout << "bucket rq failed on " << i << std::endl;
      }
    }
    std::cout << "bucket rq passed" << std::endl << std::endl;
  
}

void test_kdtree() {
    const int dimension = 3;
    float min = -100;
    float max = 100;
    int n_samples = 10000;
    int k = 5;
    float radius = 20;

    std::vector<Point<dimension>> points;
    std::vector<Point<dimension>> rq_points;
    UniformGenerator generator = UniformGenerator <dimension> (min, max);
    Point<dimension> qp = generator.generatePoint();
    DistanceComparator<dimension> cmp{qp};
    for (int i = 0; i < n_samples; i++) {
      Point<dimension> p = generator.generatePoint();
      if (distance(p, qp) <= radius) {
        rq_points.push_back(p);
      }
      points.push_back(p);
    }
    KDTree tree = KDTree(points);
    std::vector knn = tree.KNN(qp, k);
    std::vector range_points = tree.rangeQuery(qp, radius);
    std::sort(points.begin(), points.end(), cmp);
    std::sort(rq_points.begin(), rq_points.end(), cmp);
    std::sort(range_points.begin(), range_points.end(), cmp);
    std::sort(knn.begin(), knn.end(), cmp);
    points.resize(k);
    for (int i = 0; i < k; i++) {
      if (distance(points[i], knn[i]) != 0) {
        std::cout << "kdtree knn failed on " << i << std::endl;
      }
    }
    std::cout << "kdtree knn passed" << std::endl << std::endl;

    for (int i = 0; i < rq_points.size(); i++) {
      if (distance(range_points[i], rq_points[i]) != 0) {
        std::cout << "kdtree rq failed on " << i << std::endl;
      }
    }
    std::cout << "kdtree rq passed" << std::endl << std::endl;
  
}

void test_quadtree() {
    const int dimension = 2;
    float min = -100;
    float max = 100;
    int n_samples = 10000;
    int k = 5;
    int max_size = 10;
    float radius = 3;

    std::vector<Point<dimension>> points;
    std::vector<Point<dimension>> rq_points;
    UniformGenerator generator = UniformGenerator <dimension> (min, max);
    Point<dimension> qp = generator.generatePoint();
    DistanceComparator<dimension> cmp{qp};
    for (int i = 0; i < n_samples; i++) {
      Point<dimension> p = generator.generatePoint();
      if (distance(p, qp) <= radius) {
        rq_points.push_back(p);
      }
      points.push_back(p);
    }
    QuadTree tree = QuadTree(points, max_size);
    std::vector knn = tree.KNN(qp, k);
    std::vector range_points = tree.rangeQuery(qp, radius);
    std::sort(points.begin(), points.end(), cmp);
    std::sort(rq_points.begin(), rq_points.end(), cmp);
    std::sort(range_points.begin(), range_points.end(), cmp);
    std::sort(knn.begin(), knn.end(), cmp);
    points.resize(k);
    for (int i = 0; i < k; i++) {
      if (distance(points[i], knn[i]) != 0) {
        std::cout << "quadtree knn failed on " << i << std::endl;
      }
    }
    std::cout << "quadtree knn passed" << std::endl << std::endl;

    for (int i = 0; i < rq_points.size(); i++) {
      if (distance(range_points[i], rq_points[i]) != 0) {
        std::cout << "quadtree rq failed on " << i << std::endl;
      }
    }
    std::cout << "quadtree rq passed" << std::endl << std::endl;
  
}


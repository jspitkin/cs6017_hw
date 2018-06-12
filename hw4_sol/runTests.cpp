
#include <iostream>
#include "Point.hpp"
#include "Generators.hpp"
#include "Stopwatch.hpp"

#include "BucketKNN.hpp"
#include "KDTree.hpp"
#include "QuadTree.hpp"


#include <vector>
#include <algorithm>
#include <fstream>


int main(int argc, char** argv){


  std::ofstream outs("normal.csv");
  outs << "DataStructure,Dimension,K,N,Time" << std::endl;
  Stopwatch sw;
  
  float dummy = 0;
  
  for(int trial = 0; trial < 3; ++trial){
	GaussianGenerator<2> ug(-10, 10);
	for(int i = 100; i < 1000000; i += 50000){
	  //num trials
	  auto data = getTrialData<2>(i, 100, ug);
	  
	  KDTree kdt(data.training);
	  
	  for(int k = 10; k < std::min(i, 10000); k *= 10){
		sw.start();
		for(auto& p : data.testing){
		  dummy += kdt.KNN(p, k)[0][0];
		}
		outs << "KDTree,2,"<<k<<","<<i<<","<<sw.stop()/data.testing.size() << std::endl;
	  }
	}
  }


  for(int trial = 0; trial < 3; ++trial){
	GaussianGenerator<2> ug(-10, 10);
	for(int i = 100; i < 1000000; i += 50000){
	  //num trials
	  auto data = getTrialData<2>(i, 100, ug);
	  
	  BucketKNN bknn(data.training, std::log(data.training.size()));
	  
	  for(int k = 10; k < std::min(i, 10000); k *= 10){
		sw.start();
		for(auto& p : data.testing){
		  dummy += bknn.KNN(p, k)[0][0];
		}
		outs << "Buckets,2,"<<k<<","<<i<<","<<sw.stop()/data.testing.size() << std::endl;
	  }
	}
  }

  for(int trial = 0; trial < 3; ++trial){
	GaussianGenerator<2> ug(-10, 10);
	for(int i = 100; i < 1000000; i += 50000){
	  //num trials
	  auto data = getTrialData<2>(i, 100, ug);
	  
	  QuadTree qt(data.training, 16);
	  
	  for(int k = 10; k < std::min(i, 10000); k *= 10){
		sw.start();
		for(auto& p : data.testing){
		  dummy += qt.KNN(p, k)[0][0];
		}
		outs << "QuadTree,2,"<<k<<","<<i<<","<<sw.stop()/data.testing.size() << std::endl;
	  }
	}
  }

  

  std::cout << "dummy " << dummy << std::endl;
  return 0;
  
}


void sanity(){
  std::vector<Point<2> > points;

  /*{
	{{0,0}}, {{-1, 0}}, {{0, 1}}, {{2, 2}}, {{-1, 3}}
	};*/

  GaussianGenerator<2> unif(0, 1);
  for(int i = 0; i < 20; ++i){
	points.push_back(unif.generatePoint());
  }
  
  auto bounds = getBounds(points);
  std::cout << bounds << std::endl;
  BucketKNN bknn(points, 4);

  bknn.printBuckets();

  {
	std::cout << "query point:" << points[0] << std::endl;
	std::cout << "range" << std::endl;
	auto neighbors = bknn.rangeQuery(points[0], 1.2);
	std::sort(neighbors.begin(), neighbors.end(), DistanceComparator{points[0]});
	std::cout << "neighbors: " << neighbors.size() << std::endl;
	for(auto & n : neighbors){
	  std::cout << n << " " << distance(n, points[0]) << std::endl;
	}
  }

  {
    std::cout << "knn" << std::endl;
	auto neighbors = bknn.KNN(points[0], 10);
	std::cout << "neighbors: " << neighbors.size() << std::endl;
	for(auto & n : neighbors){
	  std::cout << n << " " << distance( n, points[0]) << std::endl;
	}
  }


  KDTree kdtree(points);
  {
	std::cout << "query point:" << points[0] << std::endl;
	std::cout << "range" << std::endl;
	auto neighbors = kdtree.rangeQuery(points[0], 1.2);
	std::cout << "neighbors: " << neighbors.size() << std::endl;
	for(auto & n : neighbors){
	  std::cout << n << " " << distance(n, points[0]) << std::endl;
	}
  }

  {
	std::cout << "knn" << std::endl;
	auto neighbors = kdtree.KNN(points[0], 10);
	std::cout << "neighbors: " << neighbors.size() << std::endl;
	for(auto & n : neighbors){
	  std::cout << n << " " << distance( n, points[0]) << std::endl;
	}
  }

  QuadTree qt(points, 2);
  {
	std::cout << "query point:" << points[0] << std::endl;
	std::cout << "range" << std::endl;
	auto neighbors = qt.rangeQuery(points[0], 1.2);
	std::cout << "neighbors: " << neighbors.size() << std::endl;
	for(auto & n : neighbors){
	  std::cout << n << " " << distance(n, points[0]) << std::endl;
	}
  }

  {
	std::cout << "knn" << std::endl;
	auto neighbors = qt.KNN(points[0], 10);
	std::cout << "neighbors: " << neighbors.size() << std::endl;
	for(auto & n : neighbors){
	  std::cout << n << " " << distance( n, points[0]) << std::endl;
	}
  }
 
}

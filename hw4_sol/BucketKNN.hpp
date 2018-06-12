#pragma once

#include "Point.hpp"
#include <utility> //for pair


template<int Dimension>
class BucketKNN{


public:

  BucketKNN(const std::vector<Point<Dimension> >& points, int divisions_)
	:numPoints(points.size()), divisions(divisions_)
  {
	auto bounds = getBounds(points);
	mins = bounds.mins;
	//	std::cout << "bucketSizes" << std::endl;
	for(int i = 0; i < Dimension; ++i){
	  bucketSizes[i] = (bounds.maxs[i] - bounds.mins[i])/divisions;
	}

	buckets.resize(std::pow(divisions, Dimension));
	//	std::cout << "num buckets: " << buckets.size() << std::endl;
	for(const auto& point : points){
	  buckets[getBucket(getIndices(point))].push_back(point);
	  
	}
	
  }
  



  void printBuckets() const {

	for(const auto& bucket : buckets){
	  std::cout << "bucket" << std::endl;

	  for(auto& p : bucket){
		std::cout << p << std::endl;
	  }
	  std::cout << std::endl;
	}
  }


  std::vector<Point<Dimension> > rangeQuery(const Point<Dimension>& p, float radius) const{

	std::vector<Point<Dimension> > ret;

	auto indices = getIndices(p);

	//	std::cout << "radii" << std::endl;
	std::array<int, Dimension> radii;
	for(int i = 0; i < Dimension; ++i){
	  radii[i] = std::ceil(radius/bucketSizes[i]);
	  //	  std::cout << radii[i] << std::endl;
	}
	

	for(auto offset = firstOffset(radii); offset != lastOffset(radii); offset = nextOffset(offset, radii)){

	  std::array<int, Dimension> index;
	  bool indexValid = true;
	  for(int i = 0; i < Dimension; ++i){
		index[i] = indices[i] + offset[i];
		if(index[i] < 0 || index[i] >= divisions){
		  indexValid = false;
		  break;
		}
	  }

	  if(!indexValid){ continue; }
	  
	  const auto& bucket = buckets[getBucket(index)];
	  for(auto & pt : bucket){
		if(distance(p, pt) < radius){
		  ret.push_back(pt);
		}
	  }
	  
	}
	
	return ret;
  }


  std::vector<Point<Dimension> > KNN(const Point<Dimension>& p, int k) const{

	assert(numPoints >= k);


	float minDimension = *std::min_element(bucketSizes.begin(), bucketSizes.end());
	float r = minDimension;
	
	while(true){
	  auto neighbors = rangeQuery(p, r);
	  if(neighbors.size() >= k){
		std::sort(neighbors.begin(), neighbors.end(), DistanceComparator(p));
		neighbors.resize(k);
		return neighbors;
	  }
	  r += minDimension; //expand the search area
	}
	
	
  }

  


private:

  int numPoints;
  std::vector< std::vector< Point<Dimension> > > buckets;
  std::array<float, Dimension> mins;
  std::array<float, Dimension> bucketSizes;
  int divisions;


  int getBucket(const std::array<int, Dimension>& indices) const{
	
	int bucket = 0;
	for(int i = 0; i < Dimension; ++i){
	  int pow = Dimension - i - 1;
	  bucket += indices[pow] * std::pow(divisions, pow);
	}
	return bucket;
  }

  std::array<int, Dimension> getIndices(const Point<Dimension>& point) const{
	
	
	std::array<int, Dimension> indices;
	for(int i = 0; i < Dimension; ++i){
	  indices[i] = std::clamp(static_cast<int>((point[i] - mins[i])/bucketSizes[i]), 0, divisions -1);
	}
	return indices;
  }

  
  std::array<int, Dimension> firstOffset(const std::array<int, Dimension>& radii) const{
	std::array<int, Dimension> ret;
	for(int i = 0; i < Dimension; ++i){
	  ret[i] = -radii[i];
	}
	return ret;
  }
  
  std::array<int, Dimension> lastOffset(const std::array<int, Dimension>& radii) const{
	std::array<int, Dimension> ret;
	for(int i = 0; i < Dimension; ++i){
	  ret[i] = radii[i];
	}
	return nextOffset(ret, radii);
  }

  std::array<int, Dimension> nextOffset(const std::array<int, Dimension>& cur,
	  const std::array<int, Dimension>& radii) const{

	std::array<int, Dimension> ret = cur;
	
	int i = Dimension -1;
	ret[i]++;
	while( i > 0 && ret[i] > radii[i]){
	  ret[i] = -radii[i];
	  ret[i -1]++;
	  --i;
	}

	return ret;
	
  }
  
  
};

#pragma once

#include <array>
#include <ostream>
#include <cmath>


//Points are Read only
template<int Dimension>
struct Point{
  //static constexpr int D = Dimension; //save this so we can use it
  std::array<float, Dimension> point;

  float operator[](int index) const { return point[index]; }

};


template<int Dimension>
float distance(const Point<Dimension>& a, const Point<Dimension>& b){
  float dist = 0;
  for(int i = 0; i < Dimension; ++i){
	dist += (a[i] - b[i])*(a[i] - b[i]);
  }
  return std::sqrt(dist);
}

template<int Dimension>
std::ostream& operator<<(std::ostream& outs, const Point<Dimension>& p){
  outs << "Point(";
  for(int i = 0; i < Dimension -1; ++i){
	outs << p[i] << ", ";
  }
  outs << p[Dimension -1] << ")";
  return outs;
}


template<int Dimension>
struct CompareBy{

  template<int PD>
  bool operator()(const Point<PD>& lhs, const Point<PD>& rhs){
	static_assert(Dimension < PD, "must sort by a dimension that exists!");
	//lexographic ordering, after the provided dimension
	if(lhs[Dimension] == rhs[Dimension]){ //equal in the dim we care about, compare other dimensions
	  for(int i = 0; i < PD; ++i){
		if(i != Dimension){
		  if(lhs[i] != rhs[i]){
			return lhs[i] < rhs[i];
		  }
		}
	  }
	  return false; //points are equal
	  
	} else { //easy case
	  return lhs[Dimension] < rhs[Dimension];
	}
  }
  
};


template<int Dimension>
struct DistanceComparator {
public:
  DistanceComparator(const Point<Dimension>& q_ )
	:p{q_} {}

  bool operator()(const Point<Dimension>& lhs, const Point<Dimension>& rhs){
	return distance(p, lhs) < distance(p, rhs);
  }

private:
  Point<Dimension> p;
};




template<int Dimension>
struct AABB{
  std::array<float, Dimension> mins, maxs;
  //default contstruct to be infinite
  AABB()
  {
	for(int i = 0; i < Dimension; ++i){
	  mins[i] = std::numeric_limits<float>::min();
	  maxs[i] = std::numeric_limits<float>::max();
	}
  }

  Point<Dimension> closestInBox(const Point<Dimension>& p ){

	std::array<float, Dimension> arr;
	for(int i = 0; i < Dimension; ++i){
	  arr[i] = std::clamp(p[i], mins[i], maxs[i]);
	}
	return Point<Dimension>{arr};
	
  }
};

template<int Dimension>
AABB<Dimension> getBounds(const std::vector<Point<Dimension> > & points){

  std::array<float, Dimension> mins, maxs;
  for(int i = 0; i < Dimension; ++i){
	mins[i] = std::numeric_limits<float>::max();
	maxs[i] = std::numeric_limits<float>::min();
  }
  for(const auto& p : points){
	for(int i = 0; i < Dimension; ++i){
	  mins[i] = std::min(mins[i], p[i]);
	  maxs[i] = std::max(maxs[i], p[i]);
	}
  }

  AABB<Dimension> ret;
  ret.mins = mins;
  ret.maxs = maxs;
  return ret;
  
}

template<int Dimension>
std::ostream& operator<<(std::ostream& outs, const AABB<Dimension>& aabb){
  outs << "AABB( " << Point<Dimension>{aabb.mins} << " : " << Point<Dimension>{aabb.maxs} << " ) ";
  return outs;
}

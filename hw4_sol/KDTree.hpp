#pragma once

#include "Point.hpp"
#include <memory>
#include <queue>


template<int Dimension>
class KDTree{
public:

  KDTree(const std::vector<Point<Dimension> >& points){
	std::vector<Point<Dimension> > myCopy = points;
	root = std::unique_ptr<Node<0> >(new Node<0>(myCopy.begin(), myCopy.end()));
  }
  
  std::vector<Point<Dimension> > rangeQuery(const Point<Dimension>& p, float radius) const{

	std::vector<Point<Dimension> > ret;
	rangeQueryRecurse(root, p, radius, ret);

	return ret;
  }


  std::vector<Point<Dimension> > KNN(const Point<Dimension>& p, int k) const {

	std::vector<Point<Dimension> > pQueue;
	AABB<Dimension> aabb; //initialized to infinity
	knnRecurse(root, p, k, aabb, pQueue);

	return pQueue;
  }
  
private:
  
  template<int SplitDimension>
  struct Node{
	Point<Dimension> p;
	std::unique_ptr<Node< (SplitDimension + 1)%Dimension> >  left, right;

	template<typename Iter>
	Node(Iter begin, Iter end)
	{
	  using ChildType = Node<(SplitDimension +1)%Dimension>;
	  
	  auto middle = begin + (end - begin)/2;
	  std::nth_element(begin, middle, end, CompareBy<SplitDimension>{});
	  p = *middle;
	  if(begin != middle){
		left = std::unique_ptr<ChildType>(new ChildType(begin, middle));
	  }
	  if(middle +1 != end){
		right = std::unique_ptr<ChildType>(new ChildType(middle + 1, end));
	  }
	  
	}
	
  };
  
  std::unique_ptr<Node<0> > root;


  template<int SplitDimension>
  void rangeQueryRecurse(const std::unique_ptr<Node<SplitDimension> >& node,
	  const Point<Dimension>& p,
	  float radius,
	  std::vector<Point<Dimension> >& ret) const{

	if(distance(p, node->p) < radius){
	  ret.push_back(node->p);
	}

	if(node->left && node->p[SplitDimension] >= (p[SplitDimension] - radius)){
	  rangeQueryRecurse(node->left, p, radius, ret);
	}

	if(node->right && node->p[SplitDimension] <= (p[SplitDimension] + radius)){
	  rangeQueryRecurse(node->right, p, radius, ret);
	}
	

  }

  template <int SplitDimension>
  void knnRecurse(const std::unique_ptr<Node<SplitDimension> >& node,
	  const Point<Dimension>& p,
	  int k,
	  const AABB<Dimension>& aabb,
	  std::vector< Point<Dimension> >& pQueue) const{ 

	using ChildType = Node<(SplitDimension +1)%Dimension>;
	DistanceComparator<Dimension> cmp{p};
	
	if(pQueue.size() < k){

	  pQueue.push_back(node->p);
	  std::push_heap(pQueue.begin(), pQueue.end(), cmp);
	  
	} else if (distance(node->p, p) < distance(pQueue.front(), p)){

	  std::pop_heap(pQueue.begin(), pQueue.end(), cmp);
	  pQueue.pop_back();
	  pQueue.push_back(node->p);
	  std::push_heap(pQueue.begin(), pQueue.end(), cmp);
	  
	}

	if(node->left){
	  AABB leftBox = aabb;
	  leftBox.maxs[SplitDimension] = node->p[SplitDimension];
	  if(pQueue.size() < k || distance(leftBox.closestInBox(p), p) < distance(pQueue.front(), p)){
		knnRecurse(node->left, p, k, leftBox, pQueue);
	  }
	}

	if(node->right){
	  AABB rightBox = aabb;
	  rightBox.mins[SplitDimension] = node->p[SplitDimension];
	  if(pQueue.size() < k || distance(rightBox.closestInBox(p), p) < distance(pQueue.front(), p)){
		knnRecurse(node->right, p, k, rightBox, pQueue);
	  }
	}
	
  }
};


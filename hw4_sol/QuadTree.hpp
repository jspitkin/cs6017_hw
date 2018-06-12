#pragma once

#include "Point.hpp"


class QuadTree{

public:

  QuadTree(const std::vector<Point<2> >& points, int maxSize){

	std::vector<Point<2> > myPoints = points;
	bounds = getBounds(myPoints);
	root = std::unique_ptr<Node>(new Node(myPoints.begin(), myPoints.end(), maxSize, bounds));
	
  }

  std::vector<Point<2> > rangeQuery(const Point<2>& p, float radius) const{

	std::vector<Point<2> > ret;
	rangeQueryRecurse(root, p, radius, bounds, ret);
	return ret;
	
  }

  std::vector<Point<2> > KNN(const Point<2>& p, int k) const{
	
	std::vector<Point<2> > pQueue;

	knnRecurse(root, p, k, bounds, pQueue);

	return pQueue;

  }
  
  
private:

  struct Node{

	bool isLeaf;
	std::array<std::unique_ptr<Node>, 4> children;
	std::vector<Point<2> > points;

	static constexpr int NW = 0, NE = 1, SW = 2, SE =3;
	

	template<typename Iter>
	Node(Iter begin, Iter end, int maxSize, AABB<2> aabb){

	  if(end - begin < maxSize){
		//		std::cout << "leaf of size " << (end - begin) << std::endl;
		isLeaf = true;
		points.assign(begin, end);
		
	  } else {
		isLeaf = false;

		//		std::cout << "internal with " << (end - begin) << std::endl;
		
		float xSplit = (aabb.mins[0] + aabb.maxs[0])*0.5;
		float ySplit = (aabb.mins[1] + aabb.maxs[1])*0.5;
		
		//partition by x
		auto xMiddle = std::partition(begin, end, [xSplit](const Point<2>& p){
			  return p[0] < xSplit;
			});

		//partition by y, twice
		auto yMiddleLeft = std::partition(begin, xMiddle, [ySplit](const Point<2>& p){
			  return p[1] < ySplit;
			});

		auto yMiddleRight = std::partition(xMiddle, end, [ySplit](const Point<2>& p){
			  return p[1] < ySplit;
			});

		if(yMiddleLeft != xMiddle){
		  
		  AABB<2> aabbNW = aabb;
		  aabbNW.maxs[0] = xSplit;
		  aabbNW.mins[1] = ySplit;
		  
		  children[NW] = std::unique_ptr<Node>(new Node(yMiddleLeft, xMiddle, maxSize, aabbNW));
		}


		if(yMiddleRight != end){
		  AABB<2> aabbNE = aabb;
		  aabbNE.mins[0] = xSplit;
		  aabbNE.mins[1] = ySplit;
		  
		  children[NE] = std::unique_ptr<Node>(new Node(yMiddleRight, end, maxSize, aabbNE));
		}

		if(begin != yMiddleLeft){
		  AABB<2> aabbSW = aabb;
		  aabbSW.maxs[0] = xSplit;
		  aabbSW.maxs[1] = ySplit;
		  
		  children[SW] = std::unique_ptr<Node>(new Node(begin, yMiddleLeft, maxSize, aabbSW));
		}

		if(xMiddle != yMiddleRight){
		  AABB<2> aabbSE = aabb;
		  aabbSE.mins[0] = xSplit;
		  aabbSE.maxs[1] = ySplit;
		  
		  children[SE] = std::unique_ptr<Node>(new Node(xMiddle, yMiddleRight, maxSize, aabbSE));
		
		}

	  }
	}
  };
  std::unique_ptr<Node> root;
  AABB<2> bounds;

  void rangeQueryRecurse(const std::unique_ptr<Node>& node, const Point<2>& p, float r, AABB<2> aabb,
	  std::vector<Point<2> >& ret) const{

	if(node->isLeaf){
	  for(const auto& x : node->points){
		if(distance(x, p) < r){
		  ret.push_back(x);
		}
	  }
	} else {

	  float xSplit = (aabb.mins[0] + aabb.maxs[0])*0.5;
	  float ySplit = (aabb.mins[1] + aabb.maxs[1])*0.5;
	  
	  
	  if(node->children[Node::NW]){
		
		AABB<2> aabbNW = aabb;
		aabbNW.maxs[0] = xSplit;
		aabbNW.mins[1] = ySplit;

		if(distance(aabbNW.closestInBox(p), p) <= r){ 
		  rangeQueryRecurse(node->children[Node::NW], p, r, aabbNW, ret);
		}
	  }

	  if(node->children[Node::NE]){
		AABB<2> aabbNE = aabb;
		aabbNE.mins[0] = xSplit;
		aabbNE.mins[1] = ySplit;

		if(distance(aabbNE.closestInBox(p), p) <= r){
		  rangeQueryRecurse(node->children[Node::NE], p, r, aabbNE, ret);
		}
	  }
	  if(node->children[Node::SW]){
		AABB<2> aabbSW = aabb;
		aabbSW.maxs[0] = xSplit;
		aabbSW.maxs[1] = ySplit;
		if(distance(aabbSW.closestInBox(p), p) <= r){
		  rangeQueryRecurse(node->children[Node::SW], p, r, aabbSW, ret);
		}
	  }
	  if(node->children[Node::SE]){
		AABB<2> aabbSE = aabb;
		aabbSE.mins[0] = xSplit;
		aabbSE.maxs[1] = ySplit;
		
		if(distance(aabbSE.closestInBox(p), p) <= r){
		  rangeQueryRecurse(node->children[Node::SE], p, r, aabbSE, ret);
		  
		}
	  }
	  
	}
	
  }

  void knnRecurse(const std::unique_ptr<Node>& node, const Point<2>& p, int k, AABB<2> aabb,
	  std::vector<Point<2> >& pQueue) const{

	DistanceComparator<2> cmp{p};
	
	if(node->isLeaf){
	  for(const auto& x : node->points){
		if(pQueue.size() < k){
		  
		  pQueue.push_back(x);
		  std::push_heap(pQueue.begin(), pQueue.end(), cmp);
		  
		} else if(distance(x, p) < distance(pQueue.front(), p)){
		  
		  std::pop_heap(pQueue.begin(), pQueue.end(), cmp);
		  pQueue.pop_back();
		  pQueue.push_back(x);
		  std::push_heap(pQueue.begin(), pQueue.end(), cmp);
		  
		}
	  }
	} else {

	  float xSplit = (aabb.mins[0] + aabb.maxs[0])*0.5;
	  float ySplit = (aabb.mins[1] + aabb.maxs[1])*0.5;
	  
	  
	  if(node->children[Node::NW]){
		
		AABB<2> aabbNW = aabb;
		aabbNW.maxs[0] = xSplit;
		aabbNW.mins[1] = ySplit;

		if(pQueue.size() < k || distance(aabbNW.closestInBox(p), p) <= distance(pQueue.front(), p)){ 
		  knnRecurse(node->children[Node::NW], p, k, aabbNW, pQueue);
		}
	  }

	  if(node->children[Node::NE]){
		AABB<2> aabbNE = aabb;
		aabbNE.mins[0] = xSplit;
		aabbNE.mins[1] = ySplit;

		if(pQueue.size() < k || distance(aabbNE.closestInBox(p), p) <= distance(pQueue.front(), p)){
		  knnRecurse(node->children[Node::NE], p, k, aabbNE, pQueue);
		}
	  }
	  if(node->children[Node::SW]){
		AABB<2> aabbSW = aabb;
		aabbSW.maxs[0] = xSplit;
		aabbSW.maxs[1] = ySplit;
		if(pQueue.size() < k || distance(aabbSW.closestInBox(p), p) <= distance(pQueue.front(), p)){
		  knnRecurse(node->children[Node::SW], p, k, aabbSW, pQueue);
		}
	  }
	  if(node->children[Node::SE]){
		AABB<2> aabbSE = aabb;
		aabbSE.mins[0] = xSplit;
		aabbSE.maxs[1] = ySplit;
		
		if(pQueue.size() < k || distance(aabbSE.closestInBox(p), p) <= distance(pQueue.front(), p)){
		  knnRecurse(node->children[Node::SE], p, k, aabbSE, pQueue);
		  
		}
	  }
	  
	}

	
	
  }
  
};

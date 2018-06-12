#pragma once

#include "Point.hpp"
#include <memory>
#include <queue>


template<int Dimension>
class KDTree{
public:

  KDTree(const std::vector<Point<Dimension> >& points){
  }
  
private:
  
  template<int SplitDimension> //Don't store the split dimension explicitly
  struct Node{
	Point<Dimension> p;
	//The children will have the "next" splitting dimension
	//Since this is part of the type, we can't "forget" to set it properly... nice!
	std::unique_ptr<Node< (SplitDimension + 1)%Dimension> >  left, right;


	/*
	  We'll use iterators to describe the chunk of our points array that belong to this node/subtree
	 */
	template<typename Iter>
	Node(Iter begin, Iter end)
	{
	  //Our children (if we have any) will use the "next" splitting dimension
	  using ChildType = Node<(SplitDimension +1)%Dimension>;
	  
	  
	}
	
  };
  
};


#pragma once

#include "Point.hpp"
#include <memory>
#include <queue>


template<int Dimension>
class KDTree{
public:

    KDTree(const std::vector<Point<Dimension> >& points){
        std::cout << "Dimension: " << Dimension << std::endl;
        std::vector<Point<Dimension>> myCopy = points;
        //All of the points belong to the root
        root = std::unique_ptr<Node<0>>(new Node<0>(myCopy.begin(), myCopy.end()));
    }

    std::vector<Point<Dimension>> rangeQuery(const Point<Dimension>& p, float radius) const {
        std::vector<Point<Dimension>> ret;
        rangeQueryRecurse(root, p, radius, ret);

        return ret;
    }

    std::vector<Point<Dimension>> KNN(const Point<Dimension>& p, int k) const {
        std::vector<Point<Dimension>> pQueue;
        AABB<Dimension> aabb;
        knnRecurse(root, p, k, aabb, pQueue);
        
        return pQueue;
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

    std::unique_ptr<Node<0>> root;
  
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
        
        // not enough points in the queue - just add the point to the heap
        if(pQueue.size() < k){

          // add the current point to the queue
          pQueue.push_back(node->p);
          // fix the newly added points position in the heap
          std::push_heap(pQueue.begin(), pQueue.end(), cmp);
          
        } 
        // the distance between the current node and the point is less than
        // the distance between the max item in the heap and the point
        // meaning, the current node is better than the worst point in the heap
        // and we should replace it
        else if (distance(node->p, p) < distance(pQueue.front(), p)){

          // move the max item to the bottom of the heap
          std::pop_heap(pQueue.begin(), pQueue.end(), cmp);
          // remove that max item
          pQueue.pop_back();
          // add the current node to the heap
          pQueue.push_back(node->p);
          // fix the newly added points position in the heap
          std::push_heap(pQueue.begin(), pQueue.end(), cmp);
          
        }

        // Try and recurse left
        if(node->left){
          AABB leftBox = aabb;
          // set the max to the current nodes value
          leftBox.maxs[SplitDimension] = node->p[SplitDimension];
          // if we still need more neighbours
          // or the distance between the closest point in the box and the query point is less than
          // the distance from the furthest neighbour in the heap and the query point
          if(pQueue.size() < k || distance(leftBox.closestInBox(p), p) < distance(pQueue.front(), p)){
        	knnRecurse(node->left, p, k, leftBox, pQueue);
          }
        }

        // Try and recurse right
        if(node->right){
          AABB rightBox = aabb;
          // set the min to the current nodes value
          rightBox.mins[SplitDimension] = node->p[SplitDimension];
          // if we still need more neighbours
          // or the distance between the closest point in the box and the query point is less than
          // the distance from the furthest neighbour in the heap and the query point
          if(pQueue.size() < k || distance(rightBox.closestInBox(p), p) < distance(pQueue.front(), p)){
        	knnRecurse(node->right, p, k, rightBox, pQueue);
          }
        }
      
    }
};


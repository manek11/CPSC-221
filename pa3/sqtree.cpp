/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  // Your code here.
  stats im_stats = stats(imIn);
  unsigned int width = imIn.width();
  unsigned int height = imIn.height();

  pair <int,int> ul;
  ul.first = 0;
  ul.second = 0;  

  //build tree called on the entire image
  root = buildTree(im_stats, ul, width, height, tol); 
}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {
  // Your code here.

  Node *common_node_base = new Node(ul, w, h, s.getAvg(ul, w, h), s.getVar(ul,w,h));

  if (ul.first == ul.first+w-1 && ul.second == ul.second+h-1){
    return common_node_base;
  }
  else if(s.getVar(ul,w,h) > tol){
    return common_node_base;
  }

  else{
    int temp_width = w/2;
    int temp_height = h/2;

    pair<int,int>  temp_ul_NW(ul.first, ul.second);
    // RGBAPixel curr_pixel_NW = s.getAvg(temp_ul_NW, temp_width, temp_height);
    // double var_NW = s.getVar(temp_ul_NW, temp_width, temp_height);

    pair<int,int>  temp_ul_NE(ul.first + temp_width -1, ul.second);
    // RGBAPixel curr_pixel_NE = s.getAvg(temp_ul_NE, temp_width, temp_height);
    // double var_NE = s.getVar(temp_ul_NE, temp_width, temp_height);  
  
    pair<int,int>  temp_ul_SW(ul.first, ul.second + temp_height-1);
    // RGBAPixel curr_pixel_SW = s.getAvg(temp_ul_SW, temp_width, temp_height);
    // double var_SW = s.getVar(temp_ul_SW, temp_width, temp_height); 

    pair<int,int>  temp_ul_SE(ul.first + temp_width - 1, ul.second + temp_height -1);
    // RGBAPixel curr_pixel_SW = s.getAvg(temp_ul_SE, temp_width, temp_height);
    // double var_SE = s.getVar(temp_ul_SE, temp_width, temp_height);     

      Node *tempNW_ = buildTree(s, temp_ul_NW, temp_width, temp_height, tol);  
      Node *tempNE_ = buildTree(s, temp_ul_NE, temp_width, temp_height, tol);
      Node *tempSW_ = buildTree(s, temp_ul_SW, temp_width, temp_height, tol);
      Node *tempSE_ = buildTree(s, temp_ul_SE, temp_width, temp_height, tol);    
  }
}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  // Your code here.
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  // Your code here.
}

void SQtree::copy(const SQtree & other) {
  // Your code here.

}

int SQtree::size() {
  // Your code here.
}


/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"
#include <algorithm>
#include <math.h>

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

  size_tree = 1;

  //build tree called on the entire image
  this->root = buildTree(im_stats, ul, width, height, tol); 
}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {
  // Your code here.
  //Initialize node and associated values
  double curr_node_var = s.getVar(ul, w, h);
  RGBAPixel curr_node_avg = s.getAvg(ul, w, h);
  Node *curr_node = new Node(ul, w, h, curr_node_avg, curr_node_var);

  //----------Base Conditions----------//

  if(ul.first == ul.first+w-1 && ul.second==ul.second+h-1){
    size_tree++;
    return curr_node;
  }

  else if(h==1 && w==1){
    size_tree++;
    return curr_node;
  }

  // else if(tol < curr_node_var){
  //   size_tree++;
  //   return curr_node;    
  // }

  //----------Recursive Steps & Conditions----------//
  else{
    
    if (w>=2 && h>=2){

      pair<int, int> hori_line(ul.first+1, ul.second); 
      pair<int, int> vert_line(ul.first, ul.second+1); 
      double min_var = 0.0;

      int x = ul.first+1;
      int y = ul.second+1;

      for (int i=x; i< (ul.first+w-1);i++){
        for(int j=y; j< (ul.second+h-1);j++){
          // cout<<"105"<<endl;
            //with vertical line in outer loop it is fixed for every inner loop
            //so we will evaluate first every first vertical line with all possible horizontal lines
            //now after parition and data stored make 4 nodes with given hz & vert coordinates and calculate variabilty of all 4 
            //based on that store the max var value in vector
            pair<int, int> NW_ul(ul.first, ul.second); 
            pair<int, int> NE_ul(i, ul.second); 
            pair<int, int> SW_ul(ul.first, j); 
            pair<int, int> SE_ul(i, j);

            //Initialize NW
            double NW_node_var = s.getVar(NW_ul, i-ul.first, j-ul.second);
            
            //Initialize NE
            double NE_node_var = s.getVar(NE_ul, w - (i-ul.first), j-ul.second);
                
            //Initialize SW
            double SW_node_var = s.getVar(SW_ul, i-ul.first, h - (j-ul.second));
            
            //Initialize SE
            double SE_node_var = s.getVar(SE_ul, w - (i-ul.first), h - (j-ul.second));           
            
            //important: also while iterating through keep a min_variabilty and pair temp values which keep getting updated
            //this will make net part a lot easier

            if(i==x && j==y){
              min_var = max(max(NW_node_var, NE_node_var),max(SW_node_var, SE_node_var));
              hori_line.first = i;
              vert_line.second = j;                  
            }
            else{
              if ( min_var >= max(max(NW_node_var, NE_node_var),max(SW_node_var, SE_node_var)) ){
                min_var = max(max(NW_node_var, NE_node_var),max(SW_node_var, SE_node_var));
                hori_line.first = i;
                vert_line.second = j;
              }
            }
        }
      }

      //after this point we will have the min variabilty value and corresponding hz_vert lines pair
      //now for curr_node assign all 4 nodes to build_tree recursive call
      pair<int, int> NW_ulf(ul.first, ul.second); 
      pair<int, int> NE_ulf(hori_line.first, ul.second); 
      pair<int, int> SW_ulf(ul.first, vert_line.second);
      pair<int, int> SE_ulf(hori_line.first, vert_line.second);

      if(s.getVar(NW_ulf, hori_line.first-ul.first, vert_line.second-ul.second) > tol){
        curr_node->NW = buildTree(s, NW_ulf, hori_line.first-ul.first, vert_line.second-ul.second, tol); 
      }
      else{
        size_tree++;
        return curr_node;    
      }

      if(s.getVar(NE_ulf,  w - (hori_line.first-ul.first), vert_line.second-ul.second) > tol){
        curr_node->NE = buildTree(s, NE_ulf,  w - (hori_line.first-ul.first), vert_line.second-ul.second, tol);
      }
      else{
        size_tree++;
        return curr_node;    
      }

      if(s.getVar(SW_ulf, hori_line.first-ul.first, h - (vert_line.second-ul.second)) > tol){
        curr_node->SW = buildTree(s, SW_ulf, hori_line.first-ul.first, h - (vert_line.second-ul.second), tol); 
      }
      else{
        size_tree++;
        return curr_node;    
      }      

      if(s.getVar(SE_ulf,  w - (hori_line.first-ul.first), h - (vert_line.second-ul.second)) > tol){
        curr_node->SE = buildTree(s, SE_ulf,  w - (hori_line.first-ul.first), h - (vert_line.second-ul.second), tol);
      }
      else{
        size_tree++;
        return curr_node;    
      }  

    }

    // w>h => vertical lines only because h is smaller
    else if(w>h){
      pair<int, int> vert_line_start(ul.first+1, ul.second); 
      double min_var = 0.0;

      for(int i=ul.first+1; i<ul.first+w-1; i++){
            // cout<<"192"<<endl;

            pair<int, int> SW_ul(ul.first, ul.second); 
            pair<int, int> SE_ul(i, ul.second);

            //Initialize SW
            double SW_node_var = s.getVar(SW_ul, i-ul.first, h);

            //Initialize SE
            double SE_node_var = s.getVar(SE_ul, w - (i-ul.first), h);

            if(i==ul.first+1){
              min_var = max(SW_node_var, SE_node_var);
              vert_line_start.first = i;                
            }
            else{
              if (min_var >= max(SW_node_var, SE_node_var)){
                min_var = max(SW_node_var, SE_node_var);
                vert_line_start.first = i;
              }
            }
      }

      pair<int, int> SW_ulf(ul.first, ul.second); 
      pair<int, int> SE_ulf(vert_line_start.first, ul.second);

      if(s.getVar(SW_ulf, vert_line_start.first-ul.first, h) > tol){
        curr_node->SW = buildTree(s, SW_ulf, vert_line_start.first-ul.first, h, tol);      
      }
      else{
        size_tree++;
        return curr_node;    
      }  

      if(s.getVar(SE_ulf,  w - (vert_line_start.first-ul.first), h) > tol){
        curr_node->SE = buildTree(s, SE_ulf,  w - (vert_line_start.first-ul.first), h, tol);
      }
      else{
        size_tree++;
        return curr_node;    
      } 

    }

    // w<h => horizontal lines only because w is higher
    else{
      pair<int, int> hz_line_start(ul.first, ul.second+1); 
      double min_var = 0.0;

      for(int i=hz_line_start.second; i<ul.second+h-1; i++){
            // cout<<"242"<<endl;
            pair<int, int> NW_ul(ul.first, ul.second); 
            pair<int, int> SW_ul(ul.first, i); 

            //Initialize NW
            double NW_node_var = s.getVar(NW_ul, w, i-ul.second);
                
            //Initialize SW
            double SW_node_var = s.getVar(SW_ul, w, h - (i-ul.second));            
            
            if(i==hz_line_start.second){
              min_var = max(SW_node_var, NW_node_var);
              hz_line_start.second = i;                
            }
            else{
              if (min_var >= max(SW_node_var, NW_node_var)){
                min_var = max(SW_node_var, NW_node_var);
                hz_line_start.second = i;
              }
            }
      }

      pair<int, int> NW_ulf(ul.first, ul.second); 
      pair<int, int> SW_ulf(ul.first, hz_line_start.second);

      if(s.getVar(NW_ulf,  w, hz_line_start.second-ul.second) > tol){
        curr_node->NW = buildTree(s, NW_ulf,  w, hz_line_start.second-ul.second, tol);
      }
      else{
        size_tree++;
        return curr_node;    
      } 

      if(s.getVar(SW_ulf, w, h - (hz_line_start.second-ul.second)) > tol){
        curr_node->SW = buildTree(s, SW_ulf, w, h - (hz_line_start.second-ul.second), tol);   
      }
      else{
        size_tree++;
        return curr_node;    
      } 
    }
  }

  return curr_node;

}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  // Your code here.
  PNG Im(this->root->width, this->root->height);
  render(this->root, Im);
  return Im;
}

void SQtree::render(Node* Imroot, PNG &imIn){
  if(Imroot == NULL){
    return;
  }

  else if(Imroot->NE == NULL && Imroot->NW == NULL && Imroot->SE == NULL && Imroot->SW == NULL){
    for (int i = Imroot->upLeft.first ; i < (Imroot->upLeft.first + Imroot->width); i++){
      for (int j = Imroot->upLeft.second ; j < (Imroot->upLeft.second + Imroot->height); j++){
        RGBAPixel *pix = imIn.getPixel(i, j);
        *pix = Imroot->avg;
      }
    }
    return;
  }

  if(Imroot->NE != NULL){
    render(Imroot->NE, imIn);
  }
  if(Imroot->NW != NULL){
    render(Imroot->NW, imIn);
  }
  if(Imroot->SE != NULL){
    render(Imroot->SE, imIn);
  }
  if(Imroot->SW != NULL){
    render(Imroot->SW, imIn);
  }
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  // Your code here.
  if (this->root == NULL)
    return;

  clear(this->root->NE);
  clear(this->root->NW);
  clear(this->root->SE);
  clear(this->root->SW);
  delete this->root;
  this->root = NULL;
}

void SQtree::clear(Node *Subtree){

    if (Subtree == NULL)
        return;

    clear(Subtree->NE);
    clear(Subtree->NW);
    clear(Subtree->SE);
    clear(Subtree->SW);
    delete Subtree;
    Subtree = NULL;

}

void SQtree::copy(const SQtree & other) {
  // Your code here.
    this->root = copy(other.root);
}

 SQtree::Node * SQtree::copy(Node *OtherRoot){
   if(OtherRoot == NULL){
     return NULL;
   }
   Node *RetRoot = new Node(OtherRoot->upLeft, OtherRoot->width, OtherRoot->height, OtherRoot->avg, OtherRoot->var);
   RetRoot->NE = copy(OtherRoot->NE);
   RetRoot->NW = copy(OtherRoot->NW);
   RetRoot->SE = copy(OtherRoot->SE);
   RetRoot->SW = copy(OtherRoot->SW);
   return RetRoot;

 }


int SQtree::size() {
  // Your code here.
  return size_tree;
}


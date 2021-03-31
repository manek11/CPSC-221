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

  int image_height = imIn.height();
  int image_width = imIn.width();
  stats s = stats(imIn);
  pair<int, int> ul(0,0);
  sQtree_size = 0; 
  this->root = buildTree(s, ul, image_width, image_height, tol); //buildtree called on full image that root is pointing to 

}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {
    
  if(w==1 && h==1){
    sQtree_size++;
    return new Node(s, ul, w, h);
  }

  if(s.getVar(ul, w, h) <= tol){
    sQtree_size++;
    return new Node(s, ul, w, h);
  }

  Node *curr = new Node(s, ul, w, h);

  pair<int,int> NW;
  pair<int,int> NE;
  pair<int,int> SW;
  pair<int,int> SE;

  pair<int,int> vert_split_coord;
  pair<int,int> hz_split_coord;
  
  double NW_var;
  double NE_var;
  double SW_var;
  double SE_var;
 
  double min_max_var = LLONG_MAX;
  int flag = -1;

  for(int j=ul.second; j<ul.second+h; j++){       
    for(int i=ul.first; i<ul.first+w; i++){ 

      if(i==ul.first && j==ul.second){
        continue;
      }

      // vertical check
      else if (i!=ul.first && j==ul.second){
        NW = ul;
        NE = {i, ul.second};
        
        NW_var = s.getVar(NW, i-ul.first, h);
        NE_var = s.getVar(NE, w-(i-ul.first), h);

        if (min_max_var >= max(NE_var, NW_var)){
          min_max_var = max(NE_var, NW_var);
          vert_split_coord = {i, ul.second};
          flag = 1;
        }
      }
      
      // hz check
      else if (j!=ul.second && i==ul.first){
          NW = ul;
          SW = {ul.first, j};

          NW_var = s.getVar(NW, w, j-ul.second);      
          SW_var = s.getVar(SW, w, h-(j-ul.second));

          if (min_max_var >= max(NW_var, SW_var) ){
              min_max_var = max(NW_var, SW_var);
              hz_split_coord = {ul.first, j};
              flag = 2;
          }
      }

      // quadtree regular and shifty check
      else if(i!=ul.first && j!=ul.second) {
        NW = ul;
        NE = {i, ul.second};
        SW = {ul.first, j};
        SE = {i, j};

        NW_var = s.getVar(NW, i - ul.first, j - ul.second);
        NE_var = s.getVar(NE, w - (i - ul.first), j - ul.second);
        SW_var = s.getVar(SW, i - ul.first, h - (j - ul.second));
        SE_var = s.getVar(SE, w - (i - ul.first), h - (j - ul.second));

        if (min_max_var >= max(max(NE_var, NW_var), max(SE_var, SW_var))){
          min_max_var = max(max(NE_var, NW_var), max(SE_var, SW_var));
          vert_split_coord = {i, ul.second};
          hz_split_coord = {ul.first, j};
          flag = 4;
        }
      }

    }
  }
  
  if(flag == 1){
    NW = ul;
    NE = {vert_split_coord.first, ul.second};
    curr->NW = buildTree(s, NW, vert_split_coord.first-ul.first, h, tol);
    curr->NE = buildTree(s, NE, w-(vert_split_coord.first-ul.first), h, tol);
  }
  else if(flag == 2){
    NW = ul;
    SW = {ul.first, hz_split_coord.second};
    curr->NW = buildTree(s, ul, w, hz_split_coord.second-ul.second, tol);
    curr->SW = buildTree(s, SW, w, h-(hz_split_coord.second-ul.second), tol);
  }
  else if(flag == 4){
    NW = ul;
    NE = {vert_split_coord.first, ul.second};
    SW = {ul.first, hz_split_coord.second};
    SE = {vert_split_coord.first, hz_split_coord.second};

    curr->NW = buildTree(s, NW, vert_split_coord.first-ul.first, hz_split_coord.second-ul.second, tol);
    curr->NE = buildTree(s, NE, w-(vert_split_coord.first-ul.first), hz_split_coord.second-ul.second, tol);
    curr->SW = buildTree(s, SW, vert_split_coord.first-ul.first, h-(hz_split_coord.second-ul.second), tol);
    curr->SE = buildTree(s, SE, w-(vert_split_coord.first-ul.first), h-(hz_split_coord.second-ul.second), tol);  
  }
  
  sQtree_size++;
  return curr;

}

PNG SQtree::render() {
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

void SQtree::clear() {
  clear(this->root);
  this->root = NULL;
}

void SQtree::clear(Node *Subtree){
  if (Subtree == NULL){
    return;
  }

  clear(Subtree->NW);
  clear(Subtree->NE);
  clear(Subtree->SE);
  clear(Subtree->SW);

  Subtree->NW = NULL;
  Subtree->NE = NULL;
  Subtree->SE = NULL;
  Subtree->SW = NULL;

  delete Subtree;
  Subtree = NULL;
}

void SQtree::copy(const SQtree & other) {
  this->root = copy(other.root);
  this->sQtree_size = other.sQtree_size;
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
  return sQtree_size;
}
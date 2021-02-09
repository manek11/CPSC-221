#include "block.h"
#include <cmath>
#include <iostream>

int Block::height() const {
  /* your code here */
  if(!this->data.size()) {
    return 0;
  }
  return this->data[0].size();
}

int Block::width() const {
  /* your code here */
  return this->data.size();
}

void Block::render(PNG &im, int x) const {
  /* your code here */
  int height = this->height();
  int width = this->width(); 
  for (int i=0; i<width; i++){
    for (int j=0; j<height; j++){
      HSLAPixel *pixel = im.getPixel(i+x,j); 
      *pixel = this->data[i][j];
    }
  }
}

void Block::build(PNG &im, int x, int width) {
  /* your code here */
  unsigned int height = im.height();
  data.resize(width);
  for (int i = 0; i < width; i++)   {
    data[i].resize(height);
    for (unsigned int j = 0; j < height; j++){
      HSLAPixel *pixel = im.getPixel(i+x, j);
      data[i][j] = *pixel; 
    }
  }
}

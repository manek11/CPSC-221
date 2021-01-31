#include "block.h"
#include <cmath>
#include <iostream>

int Block::height() const {
  return this->data[0].size();
}

int Block::width() const {
 return this->data.size();
}

/**
   * Renders the given block onto img with its upper
   * left corner at (x, 0). Assumes block fits on the 
   * image.
   */
void Block::render(PNG &im, int x) const {
  /* your code here */
  for (int i=x; i<this->width()+x; i++){
    for (int j=0; j<this->height(); j++){
      HSLAPixel *pixel = im.getPixel(i,j); 
      *pixel = this->data[i-x][j];
    }
  }
}

  /** 
   * Creates a block that is width X img.height pixels in size
   * by copying the pixels from (x, 0) to (x+width-1, img.height-1)
   * in img. Assumes img is large enough to supply these pixels.
   */
void Block::build(PNG &im, int x, int width) {
  for (int i = x; i < x+width; i++)   {
    for (unsigned int j = 0; j < im.height(); j++){
      HSLAPixel *pixel = im.getPixel(i, j);
      this->data[i][j] = *pixel;
   }
 }
}
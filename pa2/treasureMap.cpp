#include "treasureMap.h"
#include "queue.h"
#include <string>
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
/* YOUR CODE HERE */
    this->base = baseim;
    this->maze = mazeim;
    this->start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){
/* YOUR CODE HERE */
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    int n, start_val, end;
    int d_md = d%64;

    string d_in_bin = std::bitset<8>(d_md).to_string(); 
    int r = pixel->r;
    string r_in_bin = std::bitset<8>(r).to_string();
    int g = pixel->g;
    string g_in_bin = std::bitset<8>(g).to_string();
    int b = pixel->b;
    string b_in_bin = std::bitset<8>(b).to_string();

    n = d_in_bin.size();

    start_val = n-2; end = n-1;
    b_in_bin = b_in_bin.substr(0, b_in_bin.size()-2) + d_in_bin.substr(start_val, end-start_val+1);

    start_val = n-4; end = n-3;
    g_in_bin = g_in_bin.substr(0, g_in_bin.size()-2) + d_in_bin.substr(start_val, end-start_val+1);

    start_val = n-6; end = n-5;
    r_in_bin = r_in_bin.substr(0, r_in_bin.size()-2) + d_in_bin.substr(start_val, end-start_val+1);

    pixel->r = std::stoi(r_in_bin, nullptr, 2);
    pixel->g = std::stoi(g_in_bin, nullptr, 2);
    pixel->b = std::stoi(b_in_bin, nullptr, 2);
}


PNG treasureMap::renderMap(){

/* YOUR CODE HERE */
    PNG base_copy = PNG(this->base);

    vector<vector<bool>> visit_status (base_copy.width(), vector<bool>(base_copy.height(), false) ); //where row == height && column == width
    vector<vector<int>> shortest_lengths (base_copy.width(), vector<int>(base_copy.height()) ); //where row == height && column == width

    Queue<pair<int, int>> traversal_q;

    visit_status[this->start.first][this->start.second] = true;

    shortest_lengths[this->start.first][this->start.second] = 0;
    int dist_start = 0;

    this->setLOB(base_copy, this->start, dist_start);

    traversal_q.enqueue(this->start);

    while(!traversal_q.isEmpty()){
        
        pair<int, int> curr = traversal_q.dequeue();
        vector<pair<int,int>> nbrs = this->neighbors(curr);
        pair<int, int> p;
        
        for(unsigned long i = 0; i < nbrs.size(); i++){
            p = nbrs[i];
            
            if (this->good(visit_status, curr, p))
            {   
                visit_status[p.first][p.second] = true;
                shortest_lengths[p.first][p.second] = shortest_lengths[curr.first][curr.second] + 1;
                int p_dist = shortest_lengths[p.first][p.second];
                this->setLOB(base_copy, p, p_dist);
                traversal_q.enqueue(p);
            }
            
        }
    }

    return base_copy;
}

// draws the entire maze on a copy of the base image by darkening the locations
// so it's visible as a shadow. This can be done by iterating over every
// pixel, but it will be faster to begin from the start position
// and find the points in the maze from there. Returns an image whose 
// non-maze pixels are the same as the base image, and whose maze pixels 
// have colour values that are 50% of those in the base image, for each of 
// red, green and blue. (We specify the computation of the colours in the 
// description of a setGrey function, below.) 
//
// This function also draws a red 7px x 7px square at the start                                                                                    
// location. If the start location is near the edge of the image,
// just draw the existing portion of the square. The start 
// should be at the center of the square.

PNG treasureMap::renderMaze(){
/* YOUR CODE HERE */

    PNG base_copy = PNG(this->base);
    RGBAPixel *start_pixel = this->maze.getPixel(this->start.first,this->start.second);
    RGBAPixel *red_pixel = new RGBAPixel(255, 0, 0);
    
    int width = base_copy.width();
    int height = base_copy.height();

    for(unsigned int i = 0; i < width; i++){
        for(unsigned int j = 0; j < height; j++){
            RGBAPixel *maze_pixel = this->maze.getPixel(i,j);
            if(*start_pixel == *maze_pixel){
                pair<int, int> loc;
                loc.first = i;
                loc.second = j;
                this->setGrey(base_copy, loc);
            }
        }
    }

    for(int i = this->start.first-3; i<this->start.first+4; i++){
        for(int j = this->start.second-3; j<this->start.second+4; j++){
            if( i>=0 && j>=0 && i<width && j<height){
                RGBAPixel *pixel = base_copy.getPixel(i,j);
                pixel->r = red_pixel->r;
                pixel->g = red_pixel->g;
                pixel->b = red_pixel->b;
            }
        }
    }

    return base_copy;
}


// tests next, a neighbor (adjacent vertex) of curr to see if it is 
// 1. within the image, 2. unvisited, and 3. the same colour as curr in
// the maze image (member var).  An entry in table v is true if a cell 
// has previously been visited, and false, otherwise. Colour sameness is
// measured using the == operator on RGBAPixel values.

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
/* YOUR CODE HERE */
    //Check if next is within the image
    int h = this->base.height();
    int w = this->base.width();

    bool within_im;
    if(next.first < w && next.second < h  && next.first >= 0 && next.second >= 0) within_im = true;
    else {
        within_im = false;
        return false;
    }

   //check if visited or not 
   bool unvisited;
   if(v[next.first][next.second] == true) {unvisited = false; return false;} 
   else unvisited = true; 

    RGBAPixel *pixel_1 = maze.getPixel(curr.first, curr.second); 
    RGBAPixel *pixel_2 = maze.getPixel(next.first, next.second); 
    bool same_color = (*pixel_1 == *pixel_2);

   return (within_im && same_color && unvisited); 
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

/* YOUR CODE HERE */
    vector<pair<int, int>> nbrs;
    pair<int, int> below (curr.first, curr.second+1);
    pair<int, int> above (curr.first, curr.second-1);
    pair<int, int> left (curr.first-1, curr.second);
    pair<int, int> right (curr.first+1, curr.second);

    nbrs.push_back(left);
    nbrs.push_back(below);
    nbrs.push_back(right);
    nbrs.push_back(above);

    return nbrs;
}

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
    im.getPixel(loc.first, loc.second)->r = 2*((im.getPixel(loc.first, loc.second)->r)/4);
    im.getPixel(loc.first, loc.second)->g = 2*((im.getPixel(loc.first, loc.second)->g)/4);
    im.getPixel(loc.first, loc.second)->b = 2*((im.getPixel(loc.first, loc.second)->b)/4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

    // RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    // int n, start_val, end;
    // int d_md = d%64;

    // string d_in_bin = std::bitset<8>(d_md).to_string(); 
    // int r = pixel->r;
    // string r_in_bin = std::bitset<8>(r).to_string();
    // int g = pixel->g;
    // string g_in_bin = std::bitset<8>(g).to_string();
    // int b = pixel->b;
    // string b_in_bin = std::bitset<8>(b).to_string();

    // n = d_in_bin.size();

    // start_val = n-2; end = n-1;
    // b_in_bin = b_in_bin.substr(0, b_in_bin.size()-2) + d_in_bin.substr(start_val, end-start_val+1);

    // start_val = n-4; end = n-3;
    // g_in_bin = g_in_bin.substr(0, g_in_bin.size()-2) + d_in_bin.substr(start_val, end-start_val+1);

    // start_val = n-6; end = n-5;
    // r_in_bin = r_in_bin.substr(0, r_in_bin.size()-2) + d_in_bin.substr(start_val, end-start_val+1);

    // pixel->r = std::stoi(r_in_bin, nullptr, 2);
    // pixel->g = std::stoi(g_in_bin, nullptr, 2);
    // pixel->b = std::stoi(b_in_bin, nullptr, 2);
    
}


PNG treasureMap::renderMap(){

// /* YOUR CODE HERE */
//  // 0. Make a copy of the base image. You will change its pixels corresponding to 
//     //      locations within the maze.
//     PNG base_copy = PNG(this->base);

//     // 1. Initialize working vectors: 
//     //      a. Define a 2d vector whose dimensions are the same as the image, and whose 
//     //      values are boolean. Use this vector to indicate whether or not a location
//     //      has been visited by the traversal.
//     //      b. Define a 2d vector whose dimensions are the same as the image, and whose
//     //      values are integers. This structure will be used to store the length of the
//     //      shortest path between the current location and the start location.
//     vector<vector<bool>> visit_status (base_copy.width(), vector<bool>(base_copy.height(), false) ); //where row == height && column == width
//     vector<vector<int>> shortest_lengths (base_copy.width(), vector<int>(base_copy.height()) ); //where row == height && column == width

//     // 2. Initialize a queue whose purpose is to orchestrate the traversal. As you search
//     //      the maze, you will have to keep a list of locations to explore. The queue maintains
//     //      that list.
//     Queue<pair<int, int>> traversal_q;

//     // 3. For the start location, do the following:
//     //      a. mark the start location as visited (structure in step 1a)
//     visit_status[this->start.first][this->start.second] = true;

//     //      b. set the distance to the start to be 0 (structure in step 1b)
//     shortest_lengths[this->start.first][this->start.second] = 0;
//     int dist_start = 0;

//     //      c. encode the distance within the start location's lower order bits of the treasure map 
//     //          (structure in step 0)
//     this->setLOB(base_copy, this->start, dist_start);

//     //      d. enqueue the start location. (structure in step 2)
//     traversal_q.enqueue(this->start);

//     // 4. While the queue is not empty:
//     //      a. set the current location, curr, to the value at the front of the queue (and dequeue). 
//     //      b. for each compass neighbor, p, of curr in the order of left, below, right, above:
//     //          i. if p is "good" (defined below) then
//     //              1) mark p's location as visited
//     //              2) set p's distance to the start to be curr's distance, + 1. 
//     //              3) encode the distance in the lower order bits of the treasure map
//     //              4) enqueue p
//     // 4.
//     while(!traversal_q.isEmpty()){
//         //a.
//         pair<int, int> curr = traversal_q.dequeue();
        
//         vector<pair<int,int>> nbrs = this->neighbors(curr);
//         pair<int, int> p;
//         //b.
//         for(unsigned long i = 0; i < nbrs.size(); i++){
//             p = nbrs[i];
//             // i.
//             if (this->good(visit_status, curr, p))
//             {   // 1)
//                 visit_status[p.first][p.second] = true;

//                 // 2)
//                 shortest_lengths[p.first][p.second] = shortest_lengths[curr.first][curr.second] + 1;
//                 int p_dist = shortest_lengths[p.first][p.second];

//                 // 3)
//                 this->setLOB(base_copy, p, p_dist);

//                 // 4)
//                 traversal_q.enqueue(p);
//             }
            
//         }
//     }

//     // 5. return the image
//     return base_copy;

return PNG(this->base);

}

PNG treasureMap::renderMaze(){

/* YOUR CODE HERE */
PNG base_copy = PNG(this->base);

    for(unsigned int i = 0; i<base_copy.width(); i++){
        for(unsigned int j = 0; j<base_copy.height(); j++){
            //RGBAPixel *base_pixel = base_copy.getPixel(i,j);
            RGBAPixel *maze_pixel = this->maze.getPixel(i,j);
            //if not white
            if( !(maze_pixel->r == 0 && maze_pixel->g == 0 && maze_pixel->b == 0) ){
                pair<int, int> loc;
                loc.first = i; loc.second = j;
                this->setGrey(base_copy, loc);
            }
        }
    }

    for(unsigned int i = this->start.first-3; i<this->start.first+4; i++){
        for(unsigned int j = this->start.second-3; j<this->start.second+4; j++){
            RGBAPixel *pixel = base_copy.getPixel(i, j);
            pixel->r = 255;
            pixel->g = 0;
            pixel->b = 0;
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
    if(next.first < h && next.second < w  && next.first >= 0 && next.second >= 0) within_im = true;
    else within_im = false;

   //Check if next is same as curr
//    RGBAPixel *currpix = this->maze.getPixel(curr.first, curr.second);
//    RGBAPixel *nextpix = this->maze.getPixel(next.first, next.second);

   //bool same_color = this->maze.getPixel(curr.first, curr.second)==(this->maze.getPixel(next.first, next.second));
   
    RGBAPixel *pixel_1 = maze.getPixel(curr.first, curr.second); 
    RGBAPixel *pixel_2 = maze.getPixel(next.first, next.second); 
    bool same_color = pixel_1 == pixel_2;

   //check if visited or not 
   bool unvisited;
   if(v[next.first][next.second] == true) unvisited = false; 
   else unvisited = true;

   return (within_im && same_color && unvisited); 

}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

/* YOUR CODE HERE */

vector<pair<int,int>> neighbours_val;

// left, below, right, above. 

// neighbours_val.push_back(pair<int, int>(curr.first+1, curr.second));
// neighbours_val.push_back(pair<int, int>(curr.first, curr.second+1));
// neighbours_val.push_back(pair<int, int>(curr.first-1, curr.second));
// neighbours_val.push_back(pair<int, int>(curr.first, curr.second-1));

// return neighbours_val;
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


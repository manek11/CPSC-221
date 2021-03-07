#include "decoder.h"
#include "queue.h"
#include "stack.h"
#include <vector>
// #include <std

using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s){
    this->start = s;
    this->mapImg = tm;

    vector<vector<bool>> visit_status (mapImg.width(), vector<bool>(mapImg.height(), false) ); //where row == height && column == width
    vector<vector<int>> shortest_lengths (mapImg.width(), vector<int>(mapImg.height()) ); //where row == height && column == width
    pair<int, int> t(-1,-1);
    vector< vector< pair<int, int> > > first_discovered (mapImg.width(), vector<pair<int, int>>(mapImg.height(), t));  //initialized with pairs with 0s

    Queue<pair<int, int>> traversal_q;

    visit_status[this->start.first][this->start.second] = true;

    shortest_lengths[this->start.first][this->start.second] = 0;

    traversal_q.enqueue(this->start);

    while(!traversal_q.isEmpty()){
        pair<int, int> curr = traversal_q.dequeue();
        
        vector<pair<int,int>> nbrs = this->neighbors(curr);
        pair<int, int> p;

        for(unsigned long i = 0; i < nbrs.size(); i++){
            p = nbrs[i];
            
            if (this->good(visit_status, shortest_lengths, curr, p)){

                visit_status[p.first][p.second] = true;

                shortest_lengths[p.first][p.second] = shortest_lengths[curr.first][curr.second] + 1;
                int p_dist = shortest_lengths[p.first][p.second]; //changed

                RGBAPixel *p_pix = this->mapImg.getPixel(p.first, p.second);
                RGBAPixel *curr_pix = this->mapImg.getPixel(curr.first, curr.second);

                first_discovered[p.first][p.second].first = curr.first;
                first_discovered[p.first][p.second].second = curr.second;

                traversal_q.enqueue(p);
            }
        }
    }
    
    this->discovered_maze = first_discovered; 

    int max_dist = 0;
    pair<int, int> end_point; 
    end_point.first = 0;
    end_point.second = 0;
    for(int i = 0; i<this->mapImg.width(); i++){
        for(int j = 0; j<this->mapImg.height(); j++){
            if (first_discovered[i][j].first != -1 && first_discovered[i][j].second != -1){
                if(shortest_lengths[i][j] > max_dist){
                    max_dist = shortest_lengths[i][j];
                    end_point.first = i;
                    end_point.second = j;
                }
            }
        }
    }

    //Figure how to go backwards from end point to start.
    int i,j;
    i = end_point.first; j = end_point.second;
    pathPts.push_back(end_point);

    while( (first_discovered[i][j].first != -1) && (first_discovered[i][j].second != -1) ) {
        pair<int, int> disc_from = first_discovered[i][j];
        pathPts.push_back(disc_from);
        i = disc_from.first;
        j = disc_from.second;
    }

    //reverse
    std::reverse(this->pathPts.begin(), this->pathPts.end());
}



PNG decoder::renderSolution(){
/* YOUR CODE HERE */
    PNG map_copy = PNG(this->mapImg);
    RGBAPixel *red_pixel = new RGBAPixel(255, 0, 0);
    for (int i=0; i<pathPts.size(); i++){
        pair<int,int> curr_loc = pathPts[i];
        if(curr_loc.first < map_copy.width() && curr_loc.second < map_copy.height() && curr_loc.first >=0 && curr_loc.second >= 0){
            RGBAPixel *pixel = map_copy.getPixel(curr_loc.first, curr_loc.second);
            pixel->r = red_pixel->r;
            pixel->g = red_pixel->g;
            pixel->b = red_pixel->b;
        }
    }

    return map_copy;
}

PNG decoder::renderMaze(){
    PNG base_copy = PNG(this->mapImg);
    RGBAPixel *red_pixel = new RGBAPixel(255, 0, 0);
    
    int width = base_copy.width();
    int height = base_copy.height();

    for( int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
             if (this->discovered_maze[i][j].first != -1 && this->discovered_maze[i][j].second != -1){
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
                pixel->b = red_pixel->b;//smart
            }
        }
    }

    return base_copy;


}

void decoder::setGrey(PNG & im, pair<int,int> loc){
    /* YOUR CODE HERE */
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);
}

pair<int,int> decoder::findSpot(){
/* YOUR CODE HERE */
    return pathPts[pathPts.size() - 1];
}

int decoder::pathLength(){
    /* YOUR CODE HERE */
    int manhattanDistance = 0;
    for (int i=0; i < pathPts.size()-1; i++){
            pair<int,int> curr = pathPts[i];
            pair<int,int> curr_next = pathPts[i+1];
            manhattanDistance += abs(curr.first - curr_next.first) + abs(curr.second - curr_next.second);
    }
    return manhattanDistance+1; //specs
}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){
    /* YOUR CODE HERE */

    int h = this->mapImg.height();
    int w = this->mapImg.width();

    bool within_im;
    if(next.first < w && next.second < h  && next.first >= 0 && next.second >= 0) {
        within_im = true;
    }
    else {
        within_im = false;
        return false;
    }

   //check if visited or not 
   bool unvisited;
   if(v[next.first][next.second] == true) { 
        unvisited = false;
        return false;
    } 
   else{
        unvisited = true;
    } 

    RGBAPixel *pixel_1 = mapImg.getPixel(curr.first, curr.second);     
    RGBAPixel *pixel_2 = mapImg.getPixel(next.first, next.second);     

    bool d_cond = ((d[curr.first][curr.second] + 1) % 64) == ((pixel_2->r%4)*16 + (pixel_2->g%4)*4 + (pixel_2->b%4)); //was second first in comment 

   return (within_im && d_cond && unvisited); 

}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {
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


bool decoder::compare(RGBAPixel p, int d){
    /* YOUR CODE HERE */
    return ((d+1)%64) == ((p.r%4)*16 + (p.g%4)*4 + (p.b%4));
}

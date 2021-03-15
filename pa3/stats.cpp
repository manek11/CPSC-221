#include "stats.h"
#include<math.h> 
#include<vector>

stats::stats(PNG & im){
    /* Your code here!! */
    long curr_r_sum = 0; 
    long curr_g_sum = 0; 
    long curr_b_sum = 0; 
    long curr_r_sqsum = 0; 
    long curr_g_sqsum = 0; 
    long curr_b_sqsum = 0; 

    int image_height = im.height();
    int image_width = im.width();

    sumRed.resize(image_width);
    sumGreen.resize(image_width);
    sumBlue.resize(image_width);
    sumsqRed.resize(image_width);
    sumsqGreen.resize(image_width);
    sumsqBlue.resize(image_width);

    for(int i=0; i<image_width; i++){

        sumRed[i].resize(image_height);
        sumGreen[i].resize(image_height);
        sumBlue[i].resize(image_height);
        sumsqRed[i].resize(image_height);
        sumsqGreen[i].resize(image_height);
        sumsqBlue[i].resize(image_height);     

        for(int j=0; j<image_height; j++){

            RGBAPixel *curr_pixel = im.getPixel(i, j);
            int r = curr_pixel->r;
            int g = curr_pixel->g;
            int b = curr_pixel->b;

            curr_r_sum = r;
            curr_g_sum = g;
            curr_b_sum = b;
            curr_r_sqsum = r*r;
            curr_g_sqsum = g*g;
            curr_b_sqsum = b*b;
            
            if (i==0 && j==0){
                sumRed[i][j] = curr_r_sum;
                sumGreen[i][j] = curr_g_sum;
                sumBlue[i][j] = curr_b_sum;
                sumsqRed[i][j] = curr_r_sqsum;
                sumsqGreen[i][j] = curr_g_sqsum;
                sumsqBlue[i][j] = curr_b_sqsum;                
            }
            else if (i==0){
                sumRed[i][j] = curr_r_sum + sumRed[i][j-1];
                sumGreen[i][j] = curr_g_sum + sumGreen[i][j-1];
                sumBlue[i][j] = curr_b_sum  + sumBlue[i][j-1];
                sumsqRed[i][j] = curr_r_sqsum + sumsqRed[i][j-1];
                sumsqGreen[i][j] = curr_g_sqsum + sumsqGreen[i][j-1];
                sumsqBlue[i][j] = curr_b_sqsum + sumsqBlue[i][j-1];                
            }
            else if (j==0){
                sumRed[i][j] = curr_r_sum + sumRed[i-1][j];
                sumGreen[i][j] = curr_g_sum + sumGreen[i-1][j];
                sumBlue[i][j] = curr_b_sum  + sumBlue[i-1][j];
                sumsqRed[i][j] = curr_r_sqsum + sumsqRed[i-1][j];
                sumsqGreen[i][j] = curr_g_sqsum + sumsqGreen[i-1][j];
                sumsqBlue[i][j] = curr_b_sqsum + sumsqBlue[i-1][j];
            }
            else{
                sumRed[i][j] = curr_r_sum - sumRed[i-1][j-1] + sumRed[i][j-1] + sumRed[i-1][j];
                sumGreen[i][j] = curr_g_sum - sumGreen[i-1][j-1] + sumGreen[i][j-1] + sumGreen[i-1][j];
                sumBlue[i][j] = curr_b_sum - sumBlue[i-1][j-1] + sumBlue[i][j-1] + sumBlue[i-1][j];
                sumsqRed[i][j] = curr_r_sqsum - sumsqRed[i-1][j-1] + sumsqRed[i][j-1] + sumsqRed[i-1][j];
                sumsqGreen[i][j] = curr_g_sqsum - sumsqGreen[i-1][j-1] + sumsqGreen[i][j-1] + sumsqGreen[i-1][j];
                sumsqBlue[i][j] = curr_b_sqsum - sumsqBlue[i-1][j-1] + sumsqBlue[i][j-1] + sumsqBlue[i-1][j];                    
            }                   
        }
    }

    
    
}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){

    if (channel == 'r'){
        return quickSum(sumRed, ul, w, h);
        
    }
    else if (channel == 'g'){
        return quickSum(sumGreen, ul, w, h);
    }

    else{
        return quickSum(sumBlue, ul, w, h);
    }
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
    if (channel == 'r'){
        return quickSum(sumsqRed, ul, w, h);
        
    }
    else if (channel == 'g'){
        return quickSum(sumsqGreen, ul, w, h);
    }

    else{
        return quickSum(sumsqBlue, ul, w, h);
    }

}

long stats::quickSum(vector< vector< long >> sum_vect, pair<int,int> ul, int w, int h){


    pair<int,int> br(ul.first+w-1, ul.second+h-1);

    if(ul.first == 0 && ul.second == 0) return sum_vect[ul.first+w-1][ul.second+h-1];
    if(ul.first == 0) return sum_vect[br.first][br.second] - sum_vect[br.first][ul.second-1];
    if(ul.second == 0) return sum_vect[br.first][br.second] - sum_vect[ul.first-1][br.second];
    else return sum_vect[br.first][br.second] - sum_vect[ul.first+w-1][ul.second-1] - sum_vect[ul.first-1][ul.second+h-1] + sum_vect[ul.first-1][ul.second-1] ;

}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
/* Your code here!! */
    double var = 0;
    int rect_area = w*h;
    long x_r = getSum('r', ul, w, h);
    long x_g = getSum('g', ul, w, h);
    long x_b = getSum('b', ul, w, h);
    long x_rsq = getSumSq('r', ul, w, h);
    long x_gsq = getSumSq('g', ul, w, h);
    long x_bsq = getSumSq('b', ul, w, h);    

    var  = (x_rsq - (x_r*x_r)/rect_area) + (x_gsq - (x_g*x_g)/rect_area) + (x_bsq - (x_b*x_b)/rect_area);

    return var; 
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
/* Your code here!! */
    long x_r = getSum('r', ul, w, h);
    long x_g = getSum('g', ul, w, h);
    long x_b = getSum('b', ul, w, h);

    int r = x_r/(w*h);
    int g = x_g/(w*h);
    int b = x_b/(w*h);

    return RGBAPixel(r, g, b); 
}
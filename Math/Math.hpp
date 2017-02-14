#pragma once

#include <cmath>
#include <queue>
#include <utility>
#include "../Image/Image.hpp"

namespace satellite {

//Bresenham's line algorithm (circle)
std::queue< std::pair<short, short> > getPixels ( short x0, short y0, short x1, short y1 );

namespace math {

//First moment of matix
double m ( short x0, short y0, short x1, short y1, satellite::Image& picture);

//First moment +0 of matrix with log h
double m0 ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//First moment +h of matrix with log h
double mh ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Corresponding standard deviation +0 of matrix with log h
double s0 ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Corresponding standard deviation +h of matrix with log h
double sh ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Dreif of matrix with log h
double d ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Covariation of matrix with log h
double cov ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Correlation of matrix with log h
double r ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Semivariance of matrix with log h
double var ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

};
};

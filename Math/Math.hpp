#pragma once

#include <cmath>
#include <queue>
#include <utility>
#include <list>
#include <vector>
#include "../Image/Image.hpp"

namespace satellite {

//Bresenham's line algorithm
std::queue< std::pair<short, short> > getPixelsInLine ( short x0, short y0, short x1, short y1 );

//Bresenham's line algorithm (circle)
std::queue< std::pair<short, short> > getPixelsInCircle ( short x0, short y0, short r );

namespace math {
/*
 * Package of variables such as dreif, covariance, correlation, semivariance
 *-----------------------
 * err:
 *  - false - no errors
 *  - true - bad data
**/
class Pack {
public:
  Pack ( double d = 0, double cov = 0, double s = 0 ) : _drift(d), _covariance(cov), _semivariance(s) {};
  Pack calc ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture  );
  inline bool err () {
    return _err;
  };
  inline void resetErr () {
    _err = false;
  }
  inline double drift () {
    return _drift;
  };
  inline double covariance () {
    return _covariance;
  };
  inline double semivariance () {
    return _semivariance;
  };
private:
  double _drift,
         _covariance,
         _semivariance;
  bool _err;
};

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

//Drift of matrix with log h
double d ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Covariation of matrix with log h
double cov ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Correlation of matrix with log h
double r ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Semivariance of matrix with log h
double g ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Get approximated function by Gauss–Seidel method
std::vector<double> leastSquares ( unsigned long int degree, std::list<double> x, std::list<double> y, double maxDiff = 1e-2 );

};
};

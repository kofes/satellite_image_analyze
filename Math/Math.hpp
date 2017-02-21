#pragma once

#include <cmath>
#include <queue>
#include <utility>
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
  Pack ( double d = 0, double cov = 0, double cor = 0, double s = 0 ) : dreif(d), covariance(cov), correlation(cor), semivariance(s) {};
  Pack calc (  short x0, short y0, short x1, short y1, double h, satellite::Image& picture  );
  inline bool checkErr () {
    return err;
  };
  inline void resetErr () {
    err = false;
  }
  inline double getDreif () {
    return dreif;
  };
  inline double getCovariance () {
    return covariance;
  };
  inline double getCorrelation () {
    return correlation;
  };
  inline double getSemivariance () {
    return semivariance;
  };
private:
  double dreif,
         covariance,
         correlation,
         semivariance;
  bool err;
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

//Dreif of matrix with log h
double d ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Covariation of matrix with log h
double cov ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Correlation of matrix with log h
double r ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

//Semivariance of matrix with log h
double g ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture );

};
};

#pragma once

#include <cmath>
#include <complex>
#include <climits>
#include <cfloat>
#include <queue>
#include <utility>
#include <list>
#include <vector>
#include "Image.hpp"

namespace satellite {

//Bresenham's line algorithm
std::queue< std::pair<short, short> > getPixelsInLine ( short x0, short y0, short x1, short y1 );

//Bresenham's line algorithm (circle)
std::queue< std::pair<short, short> > getPixelsInCircle ( short x0, short y0, short r );

//Windows classes for Fast Fourier transform
namespace window {
  class Window {
  public:
    virtual double operator()(size_t n, size_t N) = 0;
  };
  class Hamming : public Window {
  public:
    double operator()(size_t n, size_t N) {
      return 0.53836 - 0.46164*std::cos(2*M_PI*n/(N-1));
    }
  };
  class Rectangular : public Window {
  public:
    double operator()(size_t n, size_t N) {
      return (n < N) ? 1 : 0;
    }
  };
  class Hann : public Window {
  public:
    double operator()(size_t n, size_t N) {
      return 0.5*(1 - std::cos(2*M_PI*n/(N-1)));
    }
  };
  class Blackman : public Window {
  public:
    Blackman(double src = 0.16) : _alpha(src) {};
    double operator()(size_t n, size_t N) {
      return (1-_alpha)/2 - 0.5*std::cos(2*M_PI*n/(N-1)) + _alpha/2*std::cos(4*M_PI*n/(N-1));
    }
    double& alpha(double src) {
      return (_alpha = src);
    }
  private:
    double _alpha;
  };
  class Gaussian : public Window {
  public:
    Gaussian(double src = 0.4) : _alpha(src) {};
    double operator()(size_t n, size_t N) {
      if (_alpha > 0.5) _alpha = 0.5;
      return std::pow(M_E, -0.5*std::pow((n-(N-1)/2)/(_alpha*(N-1)/2), 2));
    }
    double& alpha(double src) {
      return (_alpha = src);
    }
  private:
    double _alpha;
  };
  class Cosine : public Window {
  public:
    double operator()(size_t n, size_t N) {
      return std::sin(M_PI*n/(N-1));
    }
  };
  class Lanczos : public Window {
  public:
    double operator()(size_t n, size_t N) {
      if (!n) return 1;
      return std::sin(M_PI*((2*n/(N-1))-1))/(M_PI*((2*n/(N-1))-1));
    }
  };
  class FlatTop : public Window {
  public:
    double operator()(size_t n, size_t N) {
      return 1 - 1.93*std::cos(2*M_PI*n/(N-1))
  					+ 1.29*std::cos(4*M_PI*n/(N-1))
  					- 0.388*std::cos(6*M_PI*n/(N-1))
  					+ 0.032*std::cos(8*M_PI*n/(N-1));
    }
  };
  class Nuttall : public Window {
  public:
    double operator()(size_t n, size_t N) {
      return 0.355768 - 0.487396*std::cos(2*M_PI*n/(N-1))
  					+ 0.144232*std::cos(4*M_PI*n/(N-1))
  					- 0.012604*std::cos(6*M_PI*n/(N-1));
    }
  };
};

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
  Pack calc ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture  );
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
double m0 ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture );

//First moment +h of matrix with log h
double mh ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture );

//Corresponding standard deviation +0 of matrix with log h
double s0 ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture );

//Corresponding standard deviation +h of matrix with log h
double sh ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture );

//Drift of matrix with log h
double d ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture );

//Covariation of matrix with log h
double cov ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture );

//Correlation of matrix with log h
double r ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture );

//Semivariance of matrix with log h
double g ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture );

//Get approximated function by Gauss–Seidel method
std::vector<double> leastSquares ( unsigned long int degree, std::list<double> x, std::list<double> y, double maxDiff = 1e-2 );

//Moment of vector pairs
double moment ( const std::vector< std::pair<double, unsigned long> >& x, double center = 0, unsigned short degree = 1, double min = DBL_MIN, double max = DBL_MAX );

//First moment of vector pairs
double first_row_moment ( const std::vector< std::pair<double, unsigned long> >& x, unsigned short degree = 1, double min = DBL_MIN, double max = DBL_MAX );

//Central moment of vector pairs
double central_moment ( const std::vector< std::pair<double, unsigned long> >& x, unsigned short degree = 2, double min = DBL_MIN, double max = DBL_MAX );

//Covariation of two vectors pairs
double cov ( const std::vector< std::pair<double, unsigned long> >& x, const std::vector< std::pair<double, unsigned long> >& y );

//Finding Otsu's threshold
std::pair<size_t, double> threshold_Otsu ( const std::vector< std::pair<double, unsigned long> >& x );

//Fast Fourier transform of vector
template <typename T>
std::vector< std::complex<double> > fft(std::vector< T >& src) {
  std::vector< std::complex<double> > res(src.size());
  for (size_t i = 0; i < res.size(); ++i)
    res[i] = src[i];
  for (size_t i = 1, j = 0; i < res.size(); ++i) {
    size_t bit = src.size() >> 1;
    for (; j >= bit; bit >>= 1)
      j -= bit;
    j += bit;
    if (i < j)
      std::swap(res[i], res[j]);
  }
  for (size_t len = 2; len <= src.size(); len <<= 1) {
    double arg = 2 * M_PI / len;
    std::complex<double> wlen(std::cos(arg), std::sin(arg));
    for (size_t i = 0; i < res.size(); i+=len) {
      std::complex<double> w(1);
      for (size_t j = 0; j < len/2; ++j) {
        std::complex<double> u = res[i+j], v = res[i+j+len/2]*w;
        res[i+j] = u+v;
        res[i+j + len/2] = u-v;
        w *= wlen;
      }
    }
  }
  return res;
};

//Inverse fast Fourier transform
std::vector< std::complex<double> > ifft(std::vector< std::complex<double> >& src);

//Window function
template <typename Win, typename T>
std::vector< T > tapering(std::vector< T >& src, Win&& win) {
  std::vector< T > res(src);
  for (size_t i = 0; i < res.size(); ++i)
    res[i] *= win(i, res.size());
  return res;
};
};
};

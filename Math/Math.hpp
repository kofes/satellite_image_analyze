#pragma once

#include <cmath>
#include "../Image/Image.hpp"

namespace satellite {
namespace math {

//First moment in matrix
double m0 ( short x0, short y0, short xh, short yh, satellite::Image& picture ) {
  double result = 0;

  if (x0 < 0 || x0 + xh > picture.width() || y0 < 0 || y0 + yh > picture.height() || (x0 + xh) < 0 || (y0 + yh) < 0)
    return -1;
  if (xh == 0 || yh == 0)
    return 0;

  for (int j = 0; j < yh; ++j)
    for (int i = 0; i < xh; ++i)
      result += picture[y0 + j][x0 + i];

  result /= xh * yh;

  return result;
};

//First moment two matrix
double mh ( short x0, short y0, short xh, short yh, satellite::Image& picture ) {
  double result = 0;

  if (x0 < 0 || x0 + 2*xh > picture.width() || y0 < 0 || y0 + 2*yh > picture.height() || x0 + 2*xh < 0 || y0 + 2*yh < 0)
    return -1;
  if (xh == 0 || yh == 0)
    return 0;

  for (int j = 0; j < yh; ++j)
    for (int i = 0; i < xh; ++i)
      result += picture[y0 + j + yh][x0 + i + xh];

  result /= xh * yh;

  return result;
};

//Corresponding standard deviation in matrix
double s0 ( short x0, short y0, short xh, short yh, satellite::Image& picture ) {
  double result = 0, m;

  if (x0 < 0 || x0 + xh > picture.width() || y0 < 0 || y0 + yh > picture.height() || (x0 + xh) < 0 || (y0 + yh) < 0)
    return -1;
  if (xh == 0 || yh == 0)
    return 0;

  m = m0(x0, y0, xh, yh, picture);

  for (int j = 0; j < yh; ++j)
    for (int i = 0; i < xh; ++i)
      result += (picture[y0 + j][x0 + i] - m)*(picture[y0 + j][x0 + i] - m);

  result /= xh * yh;

  return result;
};

//Corresponding standard deviation in two matrix
double sh ( short x0, short y0, short xh, short yh, satellite::Image& picture ) {
  double result = 0, m;

  if (x0 < 0 || x0 + 2*xh > picture.width() || y0 < 0 || y0 + 2*yh > picture.height() || x0 + 2*xh < 0 || y0 + 2*yh < 0)
    return -1;
  if (xh == 0 || yh == 0)
    return 0;

  m = mh(x0, y0, xh, yh, picture);

  for (int j = 0; j < yh; ++j)
    for (int i = 0; i < xh; ++i)
      result += (picture[y0 + j + yh][x0 + i + xh] - m)*(picture[y0 + j + yh][x0 + i + xh] - m);

  result /= xh * yh;

  return result;
};

//Dreif
double d ( short x0, short y0, short xh, short yh, satellite::Image& picture ) {
  double result = 0;

  if (x0 < 0 || x0 + xh > picture.width() || y0 < 0 || y0 + yh > picture.height() || (x0 + xh) < 0 || (y0 + yh) < 0)
    return -1;
  if (xh == 0 || yh == 0)
    return 0;

  for (int j = 0; j < yh; ++j)
    for (int i = 0; i < xh; ++i)
      result += (picture[y0 + j][x0 + i] - picture[y0 + j + yh][x0 + i + xh]);

  result /= 2 * xh * yh;

  return result;
};

//Covariation
double cov ( short x0, short y0, short xh, short yh, satellite::Image& picture ) {
  double result = 0;

  if (x0 < 0 || x0 + 2*xh > picture.width() || y0 < 0 || y0 + 2*yh > picture.height() || x0 + 2*xh < 0 || y0 + 2*yh < 0)
    return -1;
  if (xh == 0 || yh == 0)
    return 0;

  for (int j = 0; j < yh; ++j)
    for (int i = 0; i < xh; ++i)
      result += picture[y0 + j][x0 + i] * picture[y0 + j + yh][x0 + i + xh];

  result /= xh * yh;
  result -= m0(x0, y0, xh, yh, picture) * mh(x0, y0, xh, yh, picture);

  return result;
};

//Correlation
double r ( short x0, short y0, short xh, short yh, satellite::Image& picture ) {
  return cov(x0, y0, xh, yh, picture) / std::sqrt(s0(x0, y0, xh, yh, picture) * sh(x0, y0, xh, yh, picture));
};

//Semivariance
double var ( short x0, short y0, short xh, short yh, satellite::Image& picture ) {
  double result = 0;

  if (x0 < 0 || x0 + 2*xh > picture.width() || y0 < 0 || y0 + 2*yh > picture.height() || x0 + 2*xh < 0 || y0 + 2*yh < 0)
    return -1;
  if (xh == 0 || yh == 0)
    return 0;

    for (int j = 0; j < yh; ++j)
      for (int i = 0; i < xh; ++i)
      result += (picture[y0 + j][x0 + i] - picture[y0 + j + yh][x0 + i + xh])*(picture[y0 + j][x0 + i] - picture[y0 + j + yh][x0 + i + xh]);

  result /= 2 * xh * yh;

  return result;
};

};
};

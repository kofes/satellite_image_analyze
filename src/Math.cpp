#include "../inc/Satellite.hpp"

std::queue< std::pair<short, short> > satellite::getPixelsInLine ( short x0, short y0, short x1, short y1 ) {
  unsigned short dx = std::abs(x1 - x0);
  unsigned short dy = std::abs(y1 - y0);
  char signX = x0 < x1 ? 1 : -1;
  char signY = y0 < y1 ? 1 : -1;
  short err = dx - dy;
  std::queue< std::pair<short, short> > result;

  while ( x0 != x1 || y0 != y1 ) {
    result.push(std::make_pair(x0, y0));

    short err2 = err*2;
    if (err2 > -dy) {
      err -= dy;
      x0 += signX;
    }
    if (err2 < dx) {
      err += dx;
      y0 += signY;
    }
  }
  result.push(std::pair<short, short>(x1, y1));

  return result;
};

std::queue< std::pair<short, short> > satellite::getPixelsInCircle ( short x0, short y0, short r ) {
  int x = 0;
  int y = r;
  int delta = 1 - 2*r;
  int err = 0;
  std::queue< std::pair<short, short> > result;
  while (y >= 0) {
    result.push(std::make_pair(x0 + x, y0 + y));
    result.push(std::make_pair(x0 + x, y0 - y));
    result.push(std::make_pair(x0 - x, y0 + y));
    result.push(std::make_pair(x0 - x, y0 - y));
      err = 2 * (delta + y) - 1;
      if ((delta < 0) && (err <= 0)) {
          delta += 2 * ++x + 1;
          continue;
      }
      err = 2 * (delta - x) - 1;
      if ((delta > 0) && (err > 0)) {

          delta += 1 - 2 * --y;
          continue;
      }
      x++;
      delta += 2 * (x - y);
      y--;
  }
  return result;
};

satellite::math::Pack satellite::math::Pack::calc (  short x0, short y0, short x1, short y1, int h, satellite::Image& picture  ) {
  double m_h, m_0, s_0, s_h;
  m_h = m_0 = s_0 = s_h = 0;
  unsigned int count = 0;
  _err = false;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0) {
    _err = true;
    return *this;
  }
  if (h <= 0 || !(x1 - x0) || !(y1 - y0))
    return *this;

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (i+x < x1) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j+y][i+x] < 0) ? 0 : picture[j+y][i+x];
            _drift += point_h - point_0;
            _covariance += point_0 * point_h;
            _semivariance += (point_0 - point_h)*(point_0 - point_h);
            m_0 += point_0;
            m_h += point_h;
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j-y][i+x] < 0) ? 0 : picture[j-y][i+x];
            _drift += point_h - point_0;
            _covariance += point_0 * point_h;
            _semivariance += (point_0 - point_h)*(point_0 - point_h);
            m_0 += point_0;
            m_h += point_h;
          }
        }
        if (i-x >= x0) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j+y][i-x] < 0) ? 0 : picture[j+y][i-x];
            _drift += point_0 - point_h;
            _covariance += point_0 * point_h;
            _semivariance += (point_0 - point_h)*(point_0 - point_h);
            m_0 += point_0;
            m_h += point_h;
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j-y][i-x] < 0) ? 0 : picture[j-y][i-x];
            _drift += point_0 - point_h;
            _covariance += point_0 * point_h;
            _semivariance += (point_0 - point_h)*(point_0 - point_h);
            m_0 += point_0;
            m_h += point_h;
          }
        }
        err = 2 * (delta + y) - 1;
        if ((delta < 0) && (err <= 0)) {
          delta += 2* ++x + 1;
          continue;
        }
        err = 2 * (delta - x) - 1;
        if ((delta > 0) && (err > 0)) {
          delta += 1 - 2* --y;
          continue;
        }
        x++;
        delta +=2*(x-y);
        y--;
      }
    }

  if (count) {
    m_0 /= count;
    m_h /= count;
    _semivariance /= 2 * count;
    _drift /= count;
    _covariance /= count;
    _covariance -= m_0 * m_h;
  }

  return *this;
};

double satellite::math::m ( short x0, short y0, short x1, short y1, satellite::Image& picture) {
  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;

  if (!(x1 - x0) || !(y1 - y0))
    return 0;

  double result = 0;
  std::queue< std::pair<short, short> > que = getPixelsInLine(x0, y0, x1, y1);

  while (!que.empty()) {
    auto pr = que.front();
    que.pop();
    short point = (picture[pr.first][pr.second] < 0) ? 0 : picture[pr.first][pr.second];
    result += point;
  }
  result /= std::sqrt((y1 - y0)*(y1 - y0) + (x1 - x0)*(x1 - x0));

  return result;
};

double satellite::math::m0 ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 0 || !(x1 - x0) || !(y1 - y0))
    return 0;

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (i+x < x1) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            result += point_0;
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            result += point_0;
          }
        }
        if (i-x >= x0) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            result += point_0;
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            result += point_0;
          }
        }
        err = 2 * (delta + y) - 1;
        if ((delta < 0) && (err <= 0)) {
          delta += 2* ++x + 1;
          continue;
        }
        err = 2 * (delta - x) - 1;
        if ((delta > 0) && (err > 0)) {
          delta += 1 - 2* --y;
          continue;
        }
        x++;
        delta +=2*(x-y);
        y--;
      }
    }

  if (count) result /= count;

  return result;
};

double satellite::math::mh ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 0 || !(x1 - x0) || !(y1 - y0))
    return 0;

    for (int j = y0; j < y1; ++j)
      for (int i = x0; i < x1; ++i) {
        int x = 0;
        int y = h;
        int delta = 1 - 2*h;
        int err = 0;
        while (y >= 0) {
          if (i+x < x1) {
            if (j + y < y1) {
              ++count;
              short point_h = (picture[j+y][i+x] < 0) ? 0 : picture[j+y][i+x];
              result += point_h;
            }
            if (j - y >= y0) {
              ++count;
              short point_h = (picture[j-y][i+x] < 0) ? 0 : picture[j-y][i+x];
              result += point_h;
            }
          }
          if (i-x >= x0) {
            if (j + y < y1) {
              ++count;
              short point_h = (picture[j+y][i-x] < 0) ? 0 : picture[j+y][i-x];
              result += point_h;
            }
            if (j - y >= y0) {
              ++count;
              short point_h = (picture[j-y][i-x] < 0) ? 0 : picture[j-y][i-x];
              result += point_h;
            }
          }
          err = 2 * (delta + y) - 1;
          if ((delta < 0) && (err <= 0)) {
            delta += 2* ++x + 1;
            continue;
          }
          err = 2 * (delta - x) - 1;
          if ((delta > 0) && (err > 0)) {
            delta += 1 - 2* --y;
            continue;
          }
          x++;
          delta +=2*(x-y);
          y--;
        }
      }

  if (count) result /= count;

  return result;
};

double satellite::math::s0 ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 0 || !(x1 - x0) || !(y1 - y0))
    return 0;

  double dm = satellite::math::m0(x0, y0, x1, y1, h, picture);

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (i+x < x1) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            result += (point_0 - dm)*(point_0 - dm);
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            result += (point_0 - dm)*(point_0 - dm);
          }
        }
        if (i-x >= x0) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            result += (point_0 - dm)*(point_0 - dm);
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            result += (point_0 - dm)*(point_0 - dm);
          }
        }
        err = 2 * (delta + y) - 1;
        if ((delta < 0) && (err <= 0)) {
          delta += 2* ++x + 1;
          continue;
        }
        err = 2 * (delta - x) - 1;
        if ((delta > 0) && (err > 0)) {
          delta += 1 - 2* --y;
          continue;
        }
        x++;
        delta +=2*(x-y);
        y--;
      }
    }

  if (count) result /= count;

  return result;
};

double satellite::math::sh ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 0 || !(x1 - x0) || !(y1 - y0))
    return 0;

  double dm = satellite::math::mh(x0, y0, x1, y1, h, picture);

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (i+x < x1) {
          if (j + y < y1) {
            ++count;
            short point_h = (picture[j+y][i+x] < 0) ? 0 : picture[j+y][i+x];
            result += (point_h - dm)*(point_h - dm);
          }
          if (j - y >= y0) {
            ++count;
            short point_h = (picture[j-y][i+x] < 0) ? 0 : picture[j-y][i+x];
            result += (point_h - dm)*(point_h - dm);
          }
        }
        if (i-x >= x0) {
          if (j + y < y1) {
            ++count;
            short point_h = (picture[j+y][i-x] < 0) ? 0 : picture[j+y][i-x];
            result += (point_h - dm)*(point_h - dm);
          }
          if (j - y >= y0) {
            ++count;
            short point_h = (picture[j-y][i-x] < 0) ? 0 : picture[j-y][i-x];
            result += (point_h - dm)*(point_h - dm);
          }
        }
        err = 2 * (delta + y) - 1;
        if ((delta < 0) && (err <= 0)) {
          delta += 2* ++x + 1;
          continue;
        }
        err = 2 * (delta - x) - 1;
        if ((delta > 0) && (err > 0)) {
          delta += 1 - 2* --y;
          continue;
        }
        x++;
        delta +=2*(x-y);
        y--;
      }
    }

  if (count) result /= count;

  return result;
};

double satellite::math::d ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 0 || !(x1 - x0) || !(y1 - y0))
    return 0;

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (i+x < x1) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j+y][i+x] < 0) ? 0 : picture[j+y][i+x];
            result += point_h - point_0;
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j-y][i+x] < 0) ? 0 : picture[j-y][i+x];
            result += point_h - point_0;
          }
        }
        if (i-x >= x0) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j+y][i-x] < 0) ? 0 : picture[j+y][i-x];
            result += point_h - point_0;
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j-y][i-x] < 0) ? 0 : picture[j-y][i-x];
            result += point_h - point_0;
          }
        }
        err = 2 * (delta + y) - 1;
        if ((delta < 0) && (err <= 0)) {
          delta += 2* ++x + 1;
          continue;
        }
        err = 2 * (delta - x) - 1;
        if ((delta > 0) && (err > 0)) {
          delta += 1 - 2* --y;
          continue;
        }
        x++;
        delta +=2*(x-y);
        y--;
      }
    }

  if (count) result /= count;

  return result;
};

double satellite::math::cov ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 0 || !(x1 - x0) || !(y1 - y0))
    return 0;

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (i+x < x1) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j+y][i+x] < 0) ? 0 : picture[j+y][i+x];
            result += point_0 * point_h;
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j-y][i+x] < 0) ? 0 : picture[j-y][i+x];
            result += point_0 * point_h;
          }
        }
        if (i-x >= x0) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j+y][i-x] < 0) ? 0 : picture[j+y][i-x];
            result += point_0 * point_h;
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j-y][i-x] < 0) ? 0 : picture[j-y][i-x];
            result += point_0 * point_h;
          }
        }
        err = 2 * (delta + y) - 1;
        if ((delta < 0) && (err <= 0)) {
          delta += 2* ++x + 1;
          continue;
        }
        err = 2 * (delta - x) - 1;
        if ((delta > 0) && (err > 0)) {
          delta += 1 - 2* --y;
          continue;
        }
        x++;
        delta +=2*(x-y);
        y--;
      }
    }

  if (count) {
    result /= count;
    result -= satellite::math::m0(x0, y0, x1, y1, h, picture) * satellite::math::mh(x0, y0, x1, y1, h, picture);
  }

  return result;
};

double satellite::math::r ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture ) {
  double dv = std::sqrt(satellite::math::s0(x0, y0, x1, y1, h, picture) * satellite::math::sh(x0, y0, x1, y1, h, picture));
  if (std::abs(dv) < 1e-5)
    return 0;
  return satellite::math::cov(x0, y0, x1, y1, h, picture) / dv;
};

double satellite::math::g ( short x0, short y0, short x1, short y1, int h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 0 || !(x1 - x0) || !(y1 - y0))
    return 0;

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (i+x < x1) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j+y][i+x] < 0) ? 0 : picture[j+y][i+x];
            result += (point_0 - point_h)*(point_0 - point_h);
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j-y][i+x] < 0) ? 0 : picture[j-y][i+x];
            result += (point_0 - point_h)*(point_0 - point_h);
          }
        }
        if (i-x >= x0) {
          if (j + y < y1) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j+y][i-x] < 0) ? 0 : picture[j+y][i-x];
            result += (point_0 - point_h)*(point_0 - point_h);
          }
          if (j - y >= y0) {
            ++count;
            short point_0 = (picture[j][i] < 0) ? 0 : picture[j][i];
            short point_h = (picture[j-y][i-x] < 0) ? 0 : picture[j-y][i-x];
            result += (point_0 - point_h)*(point_0 - point_h);
          }
        }
        err = 2 * (delta + y) - 1;
        if ((delta < 0) && (err <= 0)) {
          delta += 2* ++x + 1;
          continue;
        }
        err = 2 * (delta - x) - 1;
        if ((delta > 0) && (err > 0)) {
          delta += 1 - 2* --y;
          continue;
        }
        x++;
        delta +=2*(x-y);
        y--;
      }
    }

  if (count) result /= 2 * count;

  return result;
};

std::vector<double> satellite::math::leastSquares ( unsigned long int degree, std::list<double> x, std::list<double> y, double maxDiff ) {
  if (!x.size() || !y.size() || x.size() != y.size() || maxDiff <= 0)
    return std::vector<double>();

  //mat*ans = res

  std::vector<double> ans, res, tmp;
  std::vector< std::vector<double> > mat;
  ans.resize(degree + 1);
  res.resize(degree + 1);
  tmp.resize(degree + 1);
  mat.resize(degree + 1);
  for (unsigned int i = 0; i <= degree; ++i)
    mat[i].resize(degree + 1);

  //fill left and bottom of matrics 'mat' and fill vector 'res'
  for (unsigned int i = 0; i <= degree; ++i) {
    std::list<double>::iterator it_x, it_y;
    for (it_x = x.begin(), it_y = y.begin(); it_x != x.end() && it_y != y.end(); ++it_x, ++it_y)
      if (std::abs(*it_x) > 1e-5 || !(degree-i)) {
        double tmp_x = std::pow(*it_x, degree-i) / x.size();
        //set res
        res[i] += tmp_x * (*it_y);
        //set left and bottom
        mat[degree][i] += tmp_x;
        mat[i][0] += std::pow(*it_x, 2*degree - i) / x.size();
      }
  }
  mat[degree][0] /= 2;
  mat[degree][degree] = 1;

  //fill all
  for (unsigned int j = 1; j <= degree; ++j)
    for (unsigned int i = 0; i < degree; ++i)
      mat[i][j] = mat[i+1][j-1];

  //find 'ans'
  double diff = 1;
  while (diff > maxDiff) {
    tmp = ans;
    diff = 0;
    for (unsigned int i = 0; i <= degree; ++i) {
      double a1, a2, err;
      a1 = a2 = err = 0;
      for (unsigned int j = 0; j < i; ++j) {
        double buff, dt;
        dt = mat[i][j]*ans[j] - err;
        buff = a1 + dt;
        err = (buff - a1) - dt;
        a1 = buff;
      }
      err = 0;
      for (unsigned int j = i+1; j <= degree; ++j) {
        double buff, dt;
        dt = mat[i][j]*tmp[j] - err;
        buff = a2 + dt;
        err = (buff - a2) - dt;
        a2 = buff;
      }
      ans[i] = (res[i] - a1 - a2) / mat[i][i];
      if (std::abs(ans[i] - tmp[i]) > diff)
        diff = std::abs(ans[i] - tmp[i]);
    }
  }

  return ans;
};

double satellite::math::moment ( const std::vector< std::pair<double, unsigned long> >& x, double center, unsigned short degree, double min, double max ) {
  if (!x.size()) return 0;
  if (!degree) return 1;

  if (min < 0) min = 0;
  if (max > x.size()) max = x.size();

  double res = 0, acc = 0, err = 0;

  for (size_t i = min; i < max; ++i) {
    double buff, dx;

    dx = x[i].second - err;
    buff = acc + dx;
    err = (buff - acc) - dx;
    acc = buff;
  }

  err = 0;
  for (size_t i = min; i < max; ++i) {
    double buff, dx;

    dx = std::pow(x[i].first - center, degree) * (x[i].second/acc) - err;
    buff = res + dx;
    err = (buff - res) - dx;
    res = buff;
  }

  return res;
}

double satellite::math::first_row_moment ( const std::vector< std::pair<double, unsigned long> >& x, unsigned short degree, double min, double max ) {
  return satellite::math::moment(x, 0, degree, min, max);
};

double satellite::math::central_moment ( const std::vector< std::pair<double, unsigned long> >& x, unsigned short degree, double min, double max ) {
  return satellite::math::moment(x, satellite::math::first_row_moment(x, 1, min, max), degree, min, max);
};

double satellite::math::cov ( const std::vector< std::pair<double, unsigned long> >& x, const std::vector< std::pair<double, unsigned long> >& y ) {
  if (!x.size() || !y.size()) return 0;
  if (x.size() != y.size()) return 0/0.0;

  double res, xy, err;
  xy = satellite::math::first_row_moment(x, 1) * satellite::math::first_row_moment(y, 1);
  res = err = 0;
  std::vector< std::pair<double, unsigned long> >::const_iterator it_x, it_y;
  for (it_x = x.begin(), it_y = y.begin(); it_x != x.end(); ++it_x, ++it_y) {
    double buff, dx;

    dx = (it_x->second)*(it_y->second) / x.size() - err;
    buff = res + dx;
    err = (buff - res) - dx;
    res = buff;
  }

  res -= xy;

  return res;

};

std::pair<size_t, double> satellite::math::threshold_Otsu ( const std::vector< std::pair<double, unsigned long> >& x ) {
  size_t threshold = 0;
  double sc_max, sc_curr, disp_full;
  sc_max = 0;
  disp_full = satellite::math::central_moment(x);

  for (size_t t = 1; t < x.size(); ++t) {
    sc_curr = 1 - (satellite::math::central_moment(x, 2, 0, t) + satellite::math::central_moment(x, 2, t, x.size()))/disp_full;
    if (sc_curr > sc_max) {
      sc_max = sc_curr;
      threshold = t;
    }
  }

  return std::make_pair(threshold, sc_max);
}

std::vector< std::complex<double> > satellite::math::ifft(std::vector< std::complex<double> >& src) {
  std::vector< std::complex<double> > res(src);
  for (size_t i = 1, j = 0; i < res.size(); ++i) {
    size_t bit = res.size() >> 1;
    for (; j >= bit; bit >>= 1)
      j -= bit;
    j += bit;
    if (i < j)
      std::swap(res[i], res[j]);
  }
  for (size_t len = 2; len <= res.size(); len <<= 1) {
    double arg = -2 * M_PI / len;
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
  for (size_t i = 0; i < res.size(); ++i)
    res[i] /= res.size();

  return res;
};

#include "Math.hpp"

std::queue< std::pair<short, short> > satellite::getPixels ( short x0, short y0, short x1, short y1 ) {
  unsigned short dx = std::abs(x1 - x0);
  unsigned short dy = std::abs(y1 - y0);
  char signX = x0 < x1 ? 1 : -1;
  char signY = y0 < y1 ? 1 : -1;
  short err = dx - dy;
  std::queue< std::pair<short, short> > result;

  while ( x0 != x1 || y0 != y1 ) {
    result.push(std::pair<short, short>(x0, y0));

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

double satellite::math::m ( short x0, short y0, short x1, short y1, satellite::Image& picture) {
  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;

  double result = 0;
  std::queue< std::pair<short, short> > que = getPixels(x0, y0, x1, y1);

  while (!que.empty()) {
    auto pr = que.front();
    que.pop();
    result += picture[pr.first][pr.second];
  }
  result /= std::sqrt((y1 - y0)*(y1 - y0) + (x1 - x0)*(x1 - x0));

  return result;
};

double satellite::math::m0 ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 1e-5 || !(x1 - x0) || !(y1 - y0))
    return 0;

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (j + y <= y1 && i + x <= x1) {
          ++count;
          result += picture[j][i];
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

  result /= count;

  return result;
};

double satellite::math::mh ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 1e-5 || !(x1 - x0) || !(y1 - y0))
    return 0;

    for (int j = y0; j < y1; ++j)
      for (int i = x0; i < x1; ++i) {
        int x = 0;
        int y = h;
        int delta = 1 - 2*h;
        int err = 0;
        while (y >= 0) {
          if (j + y <= y1 && i + x <= x1) {
            ++count;
            result += picture[j + y][i + x];
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

  result /= count;

  return result;
};

double satellite::math::s0 ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 1e-5 || !(x1 - x0) || !(y1 - y0))
    return 0;

  double dm = satellite::math::m0(x0, y0, x1, y1, h, picture);

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (j + y <= y1 && i + x <= x1) {
          ++count;
          result += (picture[j][i] - dm)*(picture[j][i] - dm);
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

  result /= count;

  return result;
};

double satellite::math::sh ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 1e-5 || !(x1 - x0) || !(y1 - y0))
    return 0;

  double dm = satellite::math::mh(x0, y0, x1, y1, h, picture);

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (j + y <= y1 && i + x <= x1) {
          ++count;
          result += (picture[j + y][i + x] - dm)*(picture[j + y][i + x] - dm);
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

  result /= count;

  return result;
};

double satellite::math::d ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 1e-5 || !(x1 - x0) || !(y1 - y0))
    return 0;

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (j + y < y1 && i + x < x1) {
          ++count;
          result += (picture[j][i] - picture[j + y][i + x]);
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

  result /= count;

  return result;
};

double satellite::math::cov ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 1e-5 || !(x1 - x0) || !(y1 - y0))
    return 0;

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (j + y <= y1 && i + x <= x1) {
          ++count;
          result += picture[j][i] * picture[j + y][i + x];
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
  result /= count;
  result -= satellite::math::m0(x0, y0, x1, y1, h, picture) * satellite::math::mh(x0, y0, x1, y1, h, picture);

  return result;
};

double satellite::math::r ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture ) {
  return satellite::math::cov(x0, y0, x1, y1, h, picture) / std::sqrt(satellite::math::s0(x0, y0, x1, y1, h, picture) * satellite::math::sh(x0, y0, x1, y1, h, picture));
};

double satellite::math::var ( short x0, short y0, short x1, short y1, double h, satellite::Image& picture ) {
  double result = 0;
  unsigned int count = 0;

  if (x0 < 0 || x1 > picture.width() || y0 < 0 || y1 > picture.height() || x1 < 0 || y1 < 0)
    return -1;
  if (h <= 1e-5 || !(x1 - x0) || !(y1 - y0))
    return 0;

  for (int j = y0; j < y1; ++j)
    for (int i = x0; i < x1; ++i) {
      int x = 0;
      int y = h;
      int delta = 1 - 2*h;
      int err = 0;
      while (y >= 0) {
        if (j + y <= y1 && i + x <= x1) {
          ++count;
          result += (picture[j][i] - picture[j + y][i + x])*(picture[j][i] - picture[j + y][i + x]);
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

  result /= count;

  return result;
};

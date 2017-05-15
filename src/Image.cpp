#include "../inc/Satellite.hpp"

satellite::Image::Image ( short width, short height, const std::string& fileName ) {
  std::ifstream file;

  if (width < 0 || height < 0) return;

  file.open(fileName);
  if (!file.is_open()) return;

  pImage = new short*[height];
  if (pImage == nullptr) {
    file.close();
    return;
  }
  for (size_t i = 0; i < height; ++i) {
    pImage[i] = new short[width];
    if (pImage[i] == nullptr) {
      while (i-- > 0)
        delete[] pImage[i];
      delete[] pImage;
      file.close();
      return;
    }
  }

  iWidth = width;
  iHeight = height;

  for (size_t i = 0; i < height; ++i)
    file.read(reinterpret_cast<char *>(pImage[i]), sizeof(short)*width);
  file.close();
  iMin = 0;
  iMax = 1;
  for (size_t i = 0; i < height; ++i)
    for (size_t j = 0; j < width; ++j) {
      iMax = (iMax < pImage[i][j]) ? pImage[i][j] : iMax;
      iMin = (iMin > pImage[i][j] && pImage[i][j] > 0) ? pImage[i][j] : iMin;
    }
};

satellite::Image::Image ( const satellite::Image& img ) {
  if ( img.pImage == nullptr || !img.iWidth || !img.iHeight ) {
    pImage = nullptr;
    iWidth = iHeight = 0;
    return;
  }

  iWidth = img.iWidth;
  iHeight = img.iHeight;

  pImage = new short*[iHeight];
  if (pImage == nullptr) {
    iWidth = iHeight = 0;
    return;
  }
  for (size_t i = 0; i < iHeight; ++i) {
    pImage[i] = new short[iWidth];
    if (pImage[i] == nullptr) {
      while (i-- > 0)
        delete[] pImage[i];
      delete[] pImage;
      iWidth = iHeight = 0;
      return;
    }
  }

  iMin = 0;
  iMax = 1;
  for (size_t i = 0; i < iHeight; ++i)
    for (size_t j = 0; j < iWidth; ++j) {
      pImage[i][j] = img.pImage[i][j];
      iMax = (iMax < pImage[i][j]) ? pImage[i][j] : iMax;
      iMin = (iMin > pImage[i][j] && pImage[i][j] > 0) ? pImage[i][j] : iMin;
    }
}

satellite::Image::~Image () {
  if (pImage != nullptr) {
    for (size_t i = 0; i < iHeight; ++i)
      delete[] pImage[i];
    delete[] pImage;
  }
  iWidth = iHeight = 0;
}

void satellite::Image::read ( short width, short height, std::ifstream& file ) {
  if (width < 0 || height < 0) return;

  if (pImage != nullptr) {
    for (int i = 0; i < iHeight; ++i)
      delete[] pImage[i];
    delete[] pImage;
  }
  iWidth = iHeight = 0;

  pImage = new short*[height];
  if (pImage == nullptr) {
    file.close();
    return;
  }
  for (size_t i = 0; i < height; ++i) {
    pImage[i] = new short[width];
    if (pImage[i] == nullptr) {
      while (i-- > 0)
        delete[] pImage[i];
      delete[] pImage;
      return;
    }
  }

  iWidth = width;
  iHeight = height;

  for (size_t i = 0; i < height; ++i)
    file.read(reinterpret_cast<char *>(pImage[i]), sizeof(short)*width);
  iMin = 0;
  iMax = 1;
  for (size_t i = 0; i < height; ++i)
    for (size_t j = 0; j < width; ++j) {
      iMax = (iMax < pImage[i][j]) ? pImage[i][j] : iMax;
      iMin = (iMin > pImage[i][j] && pImage[i][j] > 0) ? pImage[i][j] : iMin;
    }
}

void satellite::Image::changeMaxMin ( unsigned short minColor, unsigned short maxColor ) {
  if ( pImage == nullptr || !iWidth || !iHeight )
    return;
  iMin = minColor;
  iMax = maxColor;
  for (size_t i = 0; i < iHeight; ++i)
    for (size_t j = 0; j < iWidth; ++j) {
      if ( pImage[i][j] < 0 )
          continue;
      if ( pImage[i][j] > maxColor )
        pImage[i][j] = maxColor;
      if ( pImage[i][j] < minColor )
        pImage[i][j] = minColor;
    }
};

void satellite::Image::cropColor ( unsigned short minColor, unsigned short maxColor ) {
  if ( pImage == nullptr || !iWidth || !iHeight )
    return;
  iMin = minColor;
  iMax = maxColor;
  for (size_t i = 0; i < iHeight; ++i)
    for (size_t j = 0; j < iWidth; ++j) {
      if ( pImage[i][j] < 0 )
        continue;
      if ( pImage[i][j] > maxColor )
        pImage[i][j] = 0;
      if ( pImage[i][j] < minColor )
        pImage[i][j] = 0;
    }
};

void satellite::Image::binary ( unsigned short border ) {
  if ( pImage == nullptr || !iWidth || !iHeight )
    return;
  iMin = 0;
  iMax = 255;
  for (size_t i = 0; i < iHeight; ++i)
    for (size_t j = 0; j < iWidth; ++j) {
      if ( pImage[i][j] < 0 )
        continue;
      if ( pImage[i][j] >= border )
        pImage[i][j] = 255;
      else
        pImage[i][j] = 0;
    }
}

void satellite::Image::copy ( unsigned short width, unsigned short height, short** src )  {
  if ( !width || !height || src == nullptr || *src == nullptr )
    return;

  for (size_t i = 0; i < iHeight; ++i)
    delete[] pImage[i];
  delete[] pImage;

  iWidth = width;
  iHeight = height;

  pImage = new short*[iHeight];
  if (pImage == nullptr) {
    iWidth = iHeight = 0;
    return;
  }
  for (size_t i = 0; i < iHeight; ++i) {
    pImage[i] = new short[iWidth];
    if (pImage[i] == nullptr) {
      while (i-- > 0)
        delete[] pImage[i];
      delete[] pImage;
      iWidth = iHeight = 0;
      return;
    }
  }

  iMin = 0;
  iMax = 1;
  for (size_t i = 0; i < iHeight; ++i)
    for (size_t j = 0; j < iWidth; ++j) {
      pImage[i][j] = src[i][j];
      iMax = (iMax < pImage[i][j]) ? pImage[i][j] : iMax;
      iMin = (iMin > pImage[i][j] && pImage[i][j] > 0) ? pImage[i][j] : iMin;
    }
};

satellite::Image& satellite::Image::setShapes ( unsigned short x0, unsigned short y0, unsigned short dx, unsigned short dy, unsigned short radius, short distance, double err, satellite::Shape type, satellite::ShapeFill fill ) {
  unsigned short countX, countY;
  if (pImage == nullptr || !iWidth || !iHeight || !dx || !dy || x0 + dx > iWidth || y0 + dy > iHeight || (dx / (radius*2 + distance)) <= 0 || (dy / (radius*2 + distance)) <= 0 || std::fabs(err) > 1 || err < 0)
    return *this;

  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<short> distribution(distance - std::abs(distance*err), distance);

  std::list< std::pair<short, short> > lst;
  std::queue< std::pair<short, short> > tmp;

  switch (type) {
    case (satellite::Shape::CIRCLE) :
      tmp = satellite::getPixelsInCircle(0, 0, radius);
      while (!tmp.empty()) {
        lst.push_front(tmp.front());
        tmp.pop();
      }
      break;
    case (satellite::Shape::SQUARE) :
      for (long i = 0; i <= radius; ++i) {
        lst.push_front(std::make_pair(-radius, -i));
        lst.push_front(std::make_pair(+radius, -i));
        lst.push_front(std::make_pair(-radius, +i));
        lst.push_front(std::make_pair(+radius, +i));
        if (fill == satellite::ShapeFill::DEFAULT) {
          lst.push_front(std::make_pair(-i, -radius));
          lst.push_front(std::make_pair(-i, +radius));
          lst.push_front(std::make_pair(+i, -radius));
          lst.push_front(std::make_pair(+i, +radius));
        }
      }
    break;
  }

  countX = dx / (radius*2 + distance);
  countY = dy / (radius*2 + distance);

  switch (fill) {
    case (satellite::ShapeFill::DEFAULT) :
      for (size_t i = 0; i < countY; ++i)
        for (size_t j = 0; j < countX; ++j) {
          short dist_x = distribution(generator);
          short dist_y = distribution(generator);
          for (auto pr : lst) {
            if (radius + dist_y/2 + (dy - (radius*2 + dist_y)*countY)/2.0 + (radius*2 + dist_y)*i + pr.second >= 0 &&
                radius + dist_y/2 + (dy - (radius*2 + dist_y)*countY)/2.0 + (radius*2 + dist_y)*i + pr.second < dy &&
                radius + dist_x/2 + (dx - (radius*2 + dist_x)*countX)/2.0 + (radius*2 + dist_x)*j + pr.first >= 0 &&
                radius + dist_x/2 + (dx - (radius*2 + dist_x)*countX)/2.0 + (radius*2 + dist_x)*j + pr.first < dx)
                pImage[y0 + radius + dist_y/2 + (dy - (radius*2 + dist_y)*countY)/2 + (radius*2 + dist_y)*i + pr.second][x0 + radius + dist_x/2 + (dx - (radius*2 + dist_x)*countX)/2 + (radius*2 + dist_x)*j + pr.first] = sizeof(unsigned short) * (1 << CHAR_BIT) - 1;
          }
        }
    break;
    case (satellite::ShapeFill::SOLID) :
      for (size_t i = 0; i < countY; ++i)
        for (size_t j = 0; j < countX; ++j) {
          short dist_x = distribution(generator);
          short dist_y = distribution(generator);
          for (auto iter = lst.begin(); iter != lst.end();) {
            auto pr_1 = *iter; iter++;
            auto pr_2 = *iter; iter++;
            tmp = satellite::getPixelsInLine(pr_1.first, pr_1.second, pr_2.first, pr_2.second);
            while (!tmp.empty()) {
              auto pr = tmp.front();
              tmp.pop();
              if (radius + dist_y/2 + (dy - (radius*2 + dist_y)*countY)/2.0 + (radius*2 + dist_y)*i + pr.second >= 0 &&
                  radius + dist_y/2 + (dy - (radius*2 + dist_y)*countY)/2.0 + (radius*2 + dist_y)*i + pr.second < dy &&
                  radius + dist_x/2 + (dx - (radius*2 + dist_x)*countX)/2.0 + (radius*2 + dist_x)*j + pr.first >= 0 &&
                  radius + dist_x/2 + (dx - (radius*2 + dist_x)*countX)/2.0 + (radius*2 + dist_x)*j + pr.first < dx)
                  pImage[y0 + radius + dist_y/2 + (dy - (radius*2 + dist_y)*countY)/2 + (radius*2 + dist_y)*i + pr.second][x0 + radius + dist_x/2 + (dx - (radius*2 + dist_x)*countX)/2 + (radius*2 + dist_x)*j + pr.first] = sizeof(unsigned short) * (1 << CHAR_BIT) - 1;
            }
          }
        }
    break;
  }

  iMin = 0;
  iMax = 1;
  for (size_t i = 0; i < iHeight; ++i)
    for (size_t j = 0; j < iWidth; ++j) {
      iMax = (iMax < pImage[i][j]) ? pImage[i][j] : iMax;
      iMin = (iMin > pImage[i][j] && pImage[i][j] > 0) ? pImage[i][j] : iMin;
    }

  return *this;
};

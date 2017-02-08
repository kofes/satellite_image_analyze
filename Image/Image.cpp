#include "Image.hpp"

satellite::Image::Image ( short width, short height, const std::string& fileName ) {
  std::ifstream file;

  if (width < 0 || height < 0) return;

  file.open(fileName);
  if (!file.is_open()) return;

  pImage = new unsigned short*[height];
  if (pImage == nullptr) {
    file.close();
    return;
  }
  for (int i = 0; i < height; ++i) {
    pImage[i] = new unsigned short[width];
    if (pImage[i] == nullptr) {
      i--;
      while (i-- > 0)
        delete[] pImage[i];
      delete[] pImage;
      file.close();
      return;
    }
  }

  iWidth = width;
  iHeight = height;

  for (int i = 0; i < height; ++i)
    file.read(reinterpret_cast<char *>(pImage[i]), sizeof(short)*width);

  file.close();
};

satellite::Image::~Image () {
  if (pImage != nullptr) {
    for (int i = 0; i < iHeight; ++i)
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

  pImage = new unsigned short*[height];
  if (pImage == nullptr) {
    file.close();
    return;
  }
  for (int i = 0; i < height; ++i) {
    pImage[i] = new unsigned short[width];
    if (pImage[i] == nullptr) {
      i--;
      while (i-- > 0)
        delete[] pImage[i];
      delete[] pImage;
      return;
    }
  }

  iWidth = width;
  iHeight = height;

  for (int i = 0; i < height; ++i)
    file.read(reinterpret_cast<char *>(pImage[i]), sizeof(short)*width);
}

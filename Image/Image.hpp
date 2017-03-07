#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>

namespace satellite {

#pragma pack(push, 1)
class Image {
public:
  Image () : iWidth(0), iHeight(0), pImage(nullptr) {};
  Image ( short width, short height, const std::string& fileName );
  Image ( const satellite::Image& img );
  void read ( short width, short height, std::ifstream& file );
  ~Image ();
  const unsigned short width () const {
    return iWidth;
  };
  const unsigned short height () const {
    return iHeight;
  };
  inline const unsigned short* operator[] ( unsigned short index ) const {
    return pImage[index];
  };
  void display ( unsigned short width, unsigned short height, unsigned short x0, unsigned short y0, unsigned short dx, unsigned short dy, unsigned short minColor = 0, unsigned short maxColor = 300 );
  void changeMaxMin ( unsigned short minColor, unsigned short maxColor );
  void binary ( unsigned short border );
  void copy (unsigned short width, unsigned short height, unsigned short** src);
private:
  /*Вывод изображения в файл в бинарном виде*/
  friend std::ofstream& operator< ( std::ofstream& file, const satellite::Image& img ) {
    if ( img.pImage == nullptr || !img.iWidth || !img.iHeight )
      return file;

    for (unsigned short i = 0; i < img.iHeight; ++i)
        file.write((char*)(img.pImage[i]), sizeof(short)*img.iWidth);

    return file;
  }
  unsigned short iWidth, iHeight;
  unsigned short** pImage;
};
#pragma pack(pop)
};

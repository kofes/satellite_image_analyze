#pragma once

#include <fstream>
#include <string>

namespace satellite {

#pragma pack(push, 1)
class Image {
public:
  Image () : iWidth(0), iHeight(0), pImage(nullptr) {};
  Image ( short width, short height, const std::string& fileName );
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
private:
  unsigned short iWidth, iHeight;
  unsigned short** pImage;
};
#pragma pack(pop)
};

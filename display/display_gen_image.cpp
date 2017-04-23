#include "../inc/Display.hpp"
#include <fstream>
#include <iostream>
using namespace satellite;

int main ( int argc, char const *argv[] ) {
  std::ifstream fp;
  unsigned short width, height;
  Image img;
  if (argc > 1)
    fp.open(argv[1]);
  if (!fp.is_open())
    fp.open("test.img");
  fp.read(reinterpret_cast<char *>(&height), sizeof(height));
  fp.read(reinterpret_cast<char *>(&width), sizeof(width));
  img.read(width, height, fp);
  display(img, width, height, 0, 0, width, height);
  return 0;
}

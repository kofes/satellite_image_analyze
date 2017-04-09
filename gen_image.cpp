#include "inc/Image.hpp"
#include "inc/Math.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>

using namespace satellite;

int main (int argc, char* argv[]) {
  std::ofstream out("test.img");
  unsigned short radius = 10, space = 10;
  unsigned short width = 250, height = 250, countX, countY;
  short** buff;

  switch (argc) {
    case 1 : break;
    case 2 :
      radius = std::strtoul(argv[1], nullptr, 10);
    break;
    case 3 :
      radius = std::strtoul(argv[1], nullptr, 10);
      space = std::strtoul(argv[2], nullptr, 10);
    break;
    case 4 :
      radius = std::strtoul(argv[1], nullptr, 10);
      space = std::strtoul(argv[2], nullptr, 10);
      height = std::strtoul(argv[3], nullptr, 10);
    break;
    default :
      radius = std::strtoul(argv[1], nullptr, 10);
      space = std::strtoul(argv[2], nullptr, 10);
      height = std::strtoul(argv[3], nullptr, 10);
      width = std::strtoul(argv[4], nullptr, 10);
  }

  if (!(radius*space)) radius = space = 10;
  if (!width) width = 250;
  if (!height) height = 250;

  buff = new short* [height];
  for (int i = 0; i < height; ++i)
  buff[i] = new short [width];

  countX = width / (radius*2 + space);
  countY = height / (radius*2 + space);

  for (long int i = 0; i < height; ++i)
    for (long int j = 0; j < width; ++j)
      buff[i][j] = 0x00;

  Image img;
  img.copy(width, height, buff);
  img.setShapes(0, 0, width, height, radius, space, 0, Shape::CIRCLE, ShapeFill::SOLID);
  img.display(width, height, 0, 0, width, height);
  out.write((char*)(&(height)), sizeof(height));
  out.write((char*)(&(width)), sizeof(width));
  out < img;

  for (int i = 0; i < height; ++i)
    delete[] buff[i];
  delete[] buff;

  return 0;
}

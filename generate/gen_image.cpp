#include "../inc/Satellite.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>

using namespace satellite;

int main (int argc, char* argv[]) {
  std::ofstream out;
  unsigned short radius = 10, space = 10;
  unsigned short width = 250, height = 250;
  Shape type = Shape::CIRCLE;
  ShapeFill fill = ShapeFill::SOLID;
  double err = 0;
  short** buff;

//Switch argc
  if (argc > 1)
    out.open(argv[1]);
  if (!out.is_open())
    out.open("test.img");
  if (argc > 2)
    radius = std::strtoul(argv[2], nullptr, 10);
  if (argc > 3)
    space = std::strtoul(argv[3], nullptr, 10);
  if (argc > 4)
    height = std::strtoul(argv[4], nullptr, 10);
  if (argc > 5)
    width = std::strtoul(argv[5], nullptr, 10);
  if (argc > 6) {
    if (std::string(argv[6]) == "circle")
      type = Shape::CIRCLE;
    else if (std::string(argv[6]) == "square")
      type = Shape::SQUARE;
  }
  if (argc > 7) {
    if (std::string(argv[7]) == "solid")
      fill = ShapeFill::SOLID;
    else if (std::string(argv[7]) == "default")
      fill = ShapeFill::DEFAULT;
  }
  if (argc > 8)
      err = std::strtod(argv[8], nullptr);
//

  if (!(radius*space)) radius = space = 10;
  if (!width) width = 250;
  if (!height) height = 250;

  buff = new short* [height];
  for (int i = 0; i < height; ++i)
  buff[i] = new short [width];

  for (long int i = 0; i < height; ++i)
    for (long int j = 0; j < width; ++j)
      buff[i][j] = 0x00;

  Image img;
  img.copy(width, height, buff);
  img.setShapes(0, 0, width, height, radius, space, err, type, fill);
  out.write((char*)(&(height)), sizeof(height));
  out.write((char*)(&(width)), sizeof(width));
  out < img;

  for (int i = 0; i < height; ++i)
    delete[] buff[i];
  delete[] buff;

  return 0;
}

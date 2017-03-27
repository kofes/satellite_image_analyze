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

  std::queue< std::pair<short, short> > tmp;
  std::list< std::pair<short, short> > lst;

  switch (argc) {
    case 1 :
      radius = space = 10;
    break;
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

  tmp = getPixelsInCircle(0, 0, radius);

  while (!tmp.empty()) {
    lst.push_front(tmp.front());
    tmp.pop();
  }

  for (long int i = 0; i < height; ++i)
    for (long int j = 0; j < width; ++j)
      buff[i][j] = 0x00;

  for (short i = 0; i < countY; ++i)
    for (short j = 0; j < countX; ++j) {
      auto iter = lst.begin();
      while (iter != lst.end()) {
        auto pr1 = *iter;
        iter++;
        auto pr2 = *iter;
        iter++;
        std::queue< std::pair<short, short> > tmp2 = getPixelsInLine(pr1.first, pr1.second, pr2.first, pr2.second);
        while (!tmp2.empty()) {
          auto pr_2 = tmp2.front();
          tmp2.pop();
          if (radius + space/2 + (height - (radius*2 + space)*countY)/2.0 + (radius*2 + space)*i + pr_2.second >= 0 &&
              radius + space/2 + (height - (radius*2 + space)*countY)/2.0 + (radius*2 + space)*i + pr_2.second < height &&
              radius + space/2 + (width - (radius*2 + space)*countX)/2.0 + (radius*2 + space)*j + pr_2.first >= 0 &&
              radius + space/2 + (width - (radius*2 + space)*countX)/2.0 + (radius*2 + space)*j + pr_2.first < width)
            buff[(height - (radius*2 + space)*countY)/2 + space/2 + radius + (radius*2 + space)*i + pr_2.second][(width - (radius*2 + space)*countX)/2 + space/2 + radius + (radius*2 + space)*j + pr_2.first] = 0xFF;
        }
      }
    }

  Image img;
  img.copy(width, height, buff);
  out.write((char*)(&(height)), sizeof(height));
  out.write((char*)(&(width)), sizeof(width));
  out < img;

  for (int i = 0; i < height; ++i)
    delete[] buff[i];
  delete[] buff;

  return 0;
}

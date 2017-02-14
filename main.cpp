#include "Passport/Passport.hpp"
#include "Image/Image.hpp"
#include "Math/Math.hpp"
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <iomanip>

using namespace satellite;

int main () {
  std::ifstream file("../2016-01-01.NOAA_19.pro", std::ios::binary );
  std::ofstream out( "out.txt" );
  passport::Proection pass;
  Image img;
  file >> pass;

  std::cout << pass << std::endl;

  std::cout << "Количество пикселей в строке: " << pass.getCountPixelsInLine() << '\n'
            << "Количество строк: " << pass.getCountLines() << '\n'
            << "Номер канала: " << pass.getChannelNum() << std::endl;

  img.read(pass.getCountPixelsInLine(), pass.getCountLines(), file);

  double d, h = 0.8;
  short start = 350, end = 500;
  std::streamsize ss = std::cout.precision();

  while (h <= (end - start)) {
    d = math::d(start, start, end, end, h, img);
    if (std::abs(d) >= 1) break;
    out << std::fixed << std::setprecision(1)
        << "h: " << h
        << std::defaultfloat << std::setprecision(ss)
        << "\t|var: " <<  math::var(start, start, end, end, h, img)
        << "\t|cov: " << math::cov(start, start, end, end, h, img)
        << std::endl;
    h+= 0.2;
  }

  out.close();
  file.close();

  return 0;
}

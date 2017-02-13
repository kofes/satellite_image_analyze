#include "Passport/Passport.hpp"
#include "Image/Image.hpp"
#include "Math/Math.hpp"
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <queue>

using namespace satellite;

int main () {
  std::ifstream file("../2016-01-01.NOAA_19.pro", std::ios::binary );
  std::ofstream out( "out.pro", std::ios::binary );
  passport::Proection pass;
  Image img;
  file >> pass;

  std::cout << pass << std::endl;

  std::cout << "Количество пикселей в строке: " << pass.getCountPixelsInLine() << '\n'
            << "Количество строк: " << pass.getCountLines() << '\n'
            << "Номер канала: " << pass.getChannelNum() << std::endl;

  img.read(pass.getCountPixelsInLine(), pass.getCountLines(), file);
  img.ChangeMaxMin(0, 300);


  out < pass < img;

  // img.display(420, 620, 0, 0, pass.getCountPixelsInLine(), pass.getCountLines(), 0, 1000 );

  out.close();
  file.close();

  return 0;
}

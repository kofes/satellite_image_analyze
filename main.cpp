#include "Passport/Passport.hpp"
#include "Image/Image.hpp"
#include "Math/Math.hpp"
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <queue>
using namespace satellite;

int main () {
  std::ifstream file("../2016-01-01.NOAA_19.pro", std::ios::in | std::ios::binary );
  passport::Proection pass;
  Image img;
  file >> pass;

  std::cout << pass << std::endl;

  std::cout << "Количество пикселей в строке: " << pass.getCountPixelsInLine() << '\n' << "Количество строк: " << pass.getCountLines() << '\n' << "Номер канала: " << pass.getChannelNum() << std::endl;

  img.read(pass.getCountPixelsInLine(), pass.getCountLines(), file);

  img.display(150, 150, 600, 850, 100, 100);

  // short x = 0, y = 0;
  // short dx, dy;
  // dx = dy = 10;
  // std::ofstream out("out.txt");
  // for (dx = 1; dx < 100; ++dx, out << '\n')
  // for (dy = 1; dy < 100; ++dy)
  //   out << math::r(x, y, dx, dy, img) << ' ';
  // out.close();
  // std::cout << "Dreif: " << math::d(x, y, dx, dy, img) << '\n'
  //           << "Semivariogram: " << math::var(x, y, dx, dy, img) << '\n'
  //           << "Covariation: " << math::cov(x, y, dx, dy, img) << '\n'
  //           << "Correlation: " << math::r(x, y, dx, dy, img)
  //           << std::endl;
  // std::ofstream py("tmp.py", std::ios::out);
  // py <<
  // "from mpl_toolkits.mplot3d import Axes3D\nimport matplotlib.pyplot as plt\nfig = plt.figure(1)\nax = Axes3D(fig)\nx, y, z = [";
  // std::queue<short> vecX, vecY;
  // std::queue<double> vecZ;
  // x = y = 0;
  // dx = dy = 1;
  //TODO: Нужен цикл по всем точкам, где выполняется (хотя бы немного) внутренняя гипотеза.
  // while (true) {
  //   double z = math::var(x, y, dx, dy, img);
  //   if (std::fabs(math::d(x, y, dx, dy, img)) > 1 ) {
  //     x++;
  //     dy += x / img.width();
  //     x %= img.width();
  //     dx = dy = 1;
  //     continue;
  //   }
  //   vecX.push(x + dx); vecY.push(y + dy);
  //   vecZ.push(z);
  //   if (dx)
  // }
  // for (short x = 0; x < pass.getCountPixelsInLine(); ++x)
  //   for ()
  // for (short dx = 1; dx < pass.getCountPixelsInLine(); ++dx)
  //   for (short dy = 1; dy < pass.getCountLines(); ++dy) {
  //     double z = math::var(x, y, dx, dy, img);
        // if (std::fabs(math::d(x, y, dx, dy, img)) > 1 ) {
        //   x++; y++;
        //   break;
        // }
        // vecX.push(x + dx); vecY.push(y + dy);
        // vecZ.push(z);
  //   }
  // std::cout << "DONE!";
  // if (!vecX.empty()) {
  //   py << vecX.front();
  //   vecX.pop();
  // }
  // while (!vecX.empty()) {
  //   py  << ',' << vecX.front();
  //   vecX.pop();
  // }
  // std::cout << "DONE X!";
  // py << "],[";
  // if (!vecY.empty()) {
  //   py << vecY.front();
  //   vecY.pop();
  // }
  // while (!vecY.empty()) {
  //   py << ',' << vecY.front();
  //   vecY.pop();
  // }
  // std::cout << "DONE Y!";
  // py << "],[";
  // if (!vecZ.empty()) {
  //   py << vecZ.front();
  //   vecZ.pop();
  // }
  // while (!vecZ.empty()) {
  //   py << ',' << vecZ.front();
  //   vecZ.pop();
  // }
  // std::cout << "DONE Z!";
  // py <<"]\nax.scatter(x, y, z)\nplt.show()" << std::endl;
  // py.close();

  file.close();

  return 0;
}

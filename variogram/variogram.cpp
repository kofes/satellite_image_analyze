#include "../inc/Satellite.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <string>
#include <cstring>
#include <vector>

using namespace satellite;


void create_variogram ( std::ifstream& input, std::ofstream& output ) {
  Image img;

  if (!input.is_open()) {
    std::cerr << "Error: file isn't exist" << std::endl;
    return;
  }

  img.read(200, 200, input);

  for (int h = 0; h < std::sqrt(200*200 + 200*200); ++h) {
    output << math::g(0, 0, 200, 200, h, img) << std::endl;
    if (h*100/std::sqrt(200*200 + 200*200) > (h-1)*100/std::sqrt(200*200 + 200*200)) {
      std::cout << '\r';
      std::cout.flush();
      std::cout << "Calculation variogram: " << h*100/(int)(std::sqrt(200*200 + 200*200)) << "%";
    }
  }
  std::cout << '\r';
  std::cout.flush();
  std::cout << "Calculation variogram: 100 %";
  std::cout << std::endl;
}

int main (int argc, char* argv[]) {

  std::ifstream input;
  std::ofstream output;

  if (argc > 1) input.open(std::string(argv[1]) + ".img");
  else          input.open("test.img");

  if (argc > 2) output.open(std::string(argv[2]) + ".var");
  else          output.open("test.var");

  create_variogram(input, output);

  return 0;
}

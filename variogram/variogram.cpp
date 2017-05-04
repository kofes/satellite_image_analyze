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

  double h = 0.4;
  while (h < std::sqrt(200*200 + 200*200)) {
    output << math::g(0, 0, 200, 200, h, img) << std::endl;
    h+=0.4;
  }
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

#include "../inc/Satellite.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <string>
#include <cstring>
#include <vector>

using namespace satellite;

void calculate_approx ( std::ifstream& input, std::ofstream& output ) {
  if (!input.is_open()) {
    std::cerr << "Error: file isn't exist" << std::endl;
    return;
  }
  std::list<double> range, val;
  for (size_t i = 0; ; ++i) {
    double buff;
    input >> buff;
    if (input.eof())
      break;
    range.push_back(i);
    val.push_back(buff);
  }
  std::vector<double> res = math::leastSquares(5, range, val);
  for (double elem : res)
    output << elem << std::endl;
}

int main (int argc, char* argv[]) {

  std::ifstream input;
  std::ofstream output;

  if (argc > 1) input.open(std::string(argv[1]) + ".var");
  else          input.open("test.var");

  if (argc > 2) output.open(std::string(argv[2]) + ".lsq");
  else          output.open("test.lsq");

  calculate_approx(input, output);

  return 0;
}

#include "../inc/Satellite.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <string>
#include <cstring>
#include <vector>

using namespace satellite;

const float t_y = 3.291f;

const std::string help =
  "Usage\n\n"
  "   <exec_file> [option] <input> <output>\n\n"
  "Options\n\n"
  "   -g, --gist             - Generate gist of <input>.pro to <output>.var\n"
  "   -o, --one-var          - Calculate sample mean of one distribution of <input>.var to <output>.var\n"
  "   -h, --help             - Print 'help'\n\n"
  "<input> <- 'test', if not setted.\n"
  "<output> <- 'test', if not setted.\n";

void generate_gist ( std::ifstream& input, std::ofstream& output ) {
  passport::Proection pass;
  Image img;
  short max = SHRT_MIN, min = SHRT_MAX;
  unsigned long *buff;

  if (!input.is_open()) {
    std::cerr << "Error: file isn't exist" << std::endl;
    return;
  }

  input >> pass;

  if (pass.DATA_TYPE[0] != (uint8_t)(passport::DATA_TYPE::PROECTION)) {
    std::cerr << "Error: file isn't proection" << std::endl;
    return;
  }

  img.read(pass.getCountPixelsInLine(), pass.getCountLines(), input);

  for (unsigned short i = 0; i < img.height(); ++i)
    for (unsigned short j = 0; j < img.width(); ++j) {
      max = (max > img[i][j]) ? max : img[i][j];
      min = (img[i][j] <= 0 || min < img[i][j]) ? min : img[i][j];
    }

  if (max <= 0) {
    output << 1 << 1 << '\n' << 0 << std::endl;
    return;
  }

  buff = new unsigned long[max-min+1];

  for (unsigned short i = 0; i < img.height(); ++i)
    for (unsigned short j = 0; j < img.width(); ++j)
      if (img[i][j] > 0) buff[img[i][j]-min]++;

  output << min << ' ' << max << ' ' << 1 << std::endl;

  for (unsigned short i = 0; i < max-min+1; ++i)
    output << buff[i] << std::endl;
}

void clac_one_dist( std::ifstream& input, std::ofstream& output ) {
  std::vector<double> buff;
  short min, max;
  double dh;
  unsigned long sum = 0;

  if (!input.is_open()) {
    std::cerr << "Error: file isn't exist" << std::endl;
    return;
  }

  input >> min >> max >> dh;

  for (unsigned short i = 0; i < max-min+1; ++i) {
    unsigned long tmp;
    input >> tmp;
    buff.push_back(tmp);
    sum += tmp;
  }

  output << math::a(buff) << ' ' << math::m(buff)*t_y/std::sqrt(sum) << std::endl;

}

int main (int argc, char* argv[]) {
  if (argc < 2 || !std::strcmp(argv[1], "--help") || !std::strcmp(argv[1], "-h")) {
    std::cout << help << std::endl;
    return 0;
  }

  std::ifstream input;
  std::ofstream output;
  //GIST
  if (!std::strcmp(argv[1], "--gist") || !std::strcmp(argv[1], "-g")) {

    if (argc > 2) input.open(std::string(argv[2]) + ".pro");
    else          input.open("test.pro");

    if (argc > 3) output.open(std::string(argv[3]) + ".var");
    else          output.open("test.var");

    generate_gist(input, output);

    return 0;
  }
  //ONE-VAR
  if (!std::strcmp(argv[1], "--one-var") || !std::strcmp(argv[1], "-o")) {

    if (argc > 2) input.open(std::string(argv[2]) + ".var");
    else          input.open("test.var");

    if (argc > 3) output.open(std::string(argv[3]) + ".var");
    else          output.open("test.var");

    clac_one_dist(input, output);

    return 0;
  }

  std::cout << "unknown token: " << argv[1] << std::endl;
  std::cout << help << std::endl;

  return 0;
}

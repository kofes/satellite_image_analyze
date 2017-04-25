#include "../inc/Satellite.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <string>
#include <cstring>
#include <vector>

using namespace satellite;

const std::string help =
  "Usage\n\n"
  "   <exec_file> [option] <input> <output>\n\n"
  "Options\n\n"
  "   -g, --gist             - Generate gist of <input>.pro to <output>.var\n"
  "   -o, --one-var          - Calculate sample mean of one distribution of <input>.var to <output>.var\n"
  "   -d, --double-var       - Calculate sample mean of two distribution of <input>.var to <output>.var\n"
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
  std::vector< std::pair<double, unsigned long> > buff;
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
    buff.push_back(std::make_pair((i+min+1)*dh, tmp));
    sum += tmp;
  }

  output << math::first_row_moment(buff) << ' ' << math::central_moment(buff) << std::endl;
}

void clac_two_dist( std::ifstream& input, std::ofstream& output ) {
  std::vector< std::pair<double, unsigned long> > buff;
  short min, max;
  double dh, E, D,
             minus_m, plus_m,
             minus_d, plus_d;

  if (!input.is_open()) {
    std::cerr << "Error: file isn't exist" << std::endl;
    return;
  }

  input >> min >> max >> dh;

  for (unsigned short i = 0; i < max-min+1; ++i) {
    unsigned long tmp;
    input >> tmp;
    buff.push_back(std::make_pair((i+min+1)*dh, tmp));
  }

  E = math::first_row_moment(buff);
  D = math::central_moment(buff);

  minus_m = 0;
  plus_m = max-min;

  for (unsigned short i = 0; i < std::trunc(E)-min+1; ++i)
    minus_m = (buff[minus_m].second > buff[i].second) ? minus_m : i;

  for (unsigned short i = std::trunc(E)-min+1; i < buff.size(); ++i)
    plus_m = (buff[plus_m].second > buff[i].second) ? plus_m : i;

  minus_m = buff[minus_m].first;
  minus_d = math::moment(buff, buff[minus_m].first, 2);

  plus_m = buff[plus_m].first;
  plus_d = math::moment(buff, buff[plus_m].first, 2);

  if (D < minus_d + plus_d) {
    minus_d = minus_d * ( D / (minus_d + plus_d));
    plus_d = D - minus_d;
  }

  output << minus_m << ' ' << minus_d << std::endl;
  output << plus_m << ' ' << plus_d << std::endl;
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
  //TWO-VAR
  if (!std::strcmp(argv[1], "--double-var") || !std::strcmp(argv[1], "-d")) {

    if (argc > 2) input.open(std::string(argv[2]) + ".var");
    else          input.open("test.var");

    if (argc > 3) output.open(std::string(argv[3]) + ".var");
    else          output.open("test.var");

    clac_two_dist(input, output);

    return 0;
  }
  std::cout << "unknown token: " << argv[1] << std::endl;
  std::cout << help << std::endl;

  return 0;
}

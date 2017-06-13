#include "../inc/Satellite.hpp"
#include <fstream>
#include <iomanip>

using namespace satellite;

double f(double x) {
  return std::exp(x/200.f)*std::sin(x/200.f);
}

int main() {
  std::ofstream output("text.out"),
                buff("text.curr"),
                input("text.in");
  std::vector<double> vec(256);
  std::vector<std::complex<double> > res;
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] = f(i);
    input << i << ' ' << vec[i] << '\n';
  }
  res = math::fft(vec);
  res = math::tapering(res, window::Hamming());
  for (size_t i = 0; i < res.size()/2; ++i)
    buff << i << ' '
         << 20*std::log(2*std::sqrt(res[i].real()*res[i].real()+res[i].imag()*res[i].imag())/res.size()) << '\n';
  res = math::ifft(res);
  for (size_t i = 0; i < res.size(); ++i) {
    output << i << ' ' << res[i].real() << '\n';
  }
  return 0;
}

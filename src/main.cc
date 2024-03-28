#include <cmath>
#include <iostream>

#include "h/fixed_point.h"

int main() {
  fixed<16> g(7L);
  fixed<8> f(10.25f);
  g /= f;
  std::cout << g.ToFloat() << std::endl;
  fixed<4, int8_t, int16_t> h(std::sqrt(2));
  fixed<4, int8_t, int16_t> q(std::sqrt(3));
  h -= q;
  std::cout << h.ToFloat() << std::endl;
  return 0;
}

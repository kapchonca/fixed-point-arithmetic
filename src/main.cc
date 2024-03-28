#include "h/fixed_point.h"

int main(int argc, char const* argv[]) {
  fixed<int16_t, int32_t, 4> g(7.125f);
  fixed<int16_t, int32_t, 2> f(10.25f);
  g /= f;
  std::cout << g.ToFloat();
  return 0;
}

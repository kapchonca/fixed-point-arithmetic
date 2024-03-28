#ifndef FIXEDPOINT
#define FIXEDPOINT

#include <iostream>
#include <limits>

template <typename basic_type, typename doubled_type, size_t scale>
class fixed {
 public:
  constexpr fixed(const float& num) {
    doubled_type res =
        num * static_cast<double>(1 << scale) + (num >= 0 ? 0.5 : -0.5);
    CheckOverflow(res);
    value_ = static_cast<basic_type>(res);
  }

  constexpr basic_type GetScaledInt() const { return value_; }

  constexpr float ToFloat() {
    return static_cast<float>(value_) / (1 << scale);
  }

  template <typename other_type, typename doubled_other, size_t other_scale>
  constexpr fixed operator+(
      const fixed<other_type, doubled_other, other_scale>& other) {
    doubled_type res = Rescale(other) + value_;
    CheckOverflow(res);
    float sm = static_cast<float>(res) / (1 << scale);
    return fixed<basic_type, doubled_type, scale>(sm);
  }

  template <typename other_type, typename doubled_other, size_t other_scale>
  constexpr fixed& operator+=(
      const fixed<other_type, doubled_other, other_scale>& other) {
    doubled_type res = value_ + Rescale(other);
    CheckOverflow(res);
    value_ = static_cast<basic_type>(res);
    return *this;
  }

  template <typename other_type, typename doubled_other, size_t other_scale>
  constexpr fixed operator-(
      const fixed<other_type, doubled_other, other_scale>& other) {
    doubled_type res = value_ - Rescale(other);
    CheckOverflow(res);
    float sm = static_cast<float>(res) / (1 << scale);
    return fixed<basic_type, doubled_type, scale>(sm);
  }

  template <typename other_type, typename doubled_other, size_t other_scale>
  constexpr fixed& operator-=(
      const fixed<other_type, doubled_other, other_scale>& other) {
    doubled_type res = value_ - Rescale(other);
    CheckOverflow(res);
    value_ = static_cast<basic_type>(res);
    return *this;
  }

  constexpr fixed operator-() {
    value_ = -value_;
    return *this;
  }

  template <typename other_type, typename doubled_other, size_t other_scale>
  constexpr fixed operator*(
      const fixed<other_type, doubled_other, other_scale>& other) {
    doubled_type res = (static_cast<doubled_type>(value_) *
                        static_cast<doubled_type>(Rescale(other))) >>
                       scale;
    CheckOverflow(res);
    float sm = static_cast<float>(res) / (1 << scale);
    return fixed<basic_type, doubled_type, scale>(sm);
  }

  template <typename other_type, typename doubled_other, size_t other_scale>
  constexpr fixed& operator*=(
      const fixed<other_type, doubled_other, other_scale>& other) {
    doubled_type res = (static_cast<doubled_type>(value_) *
                        static_cast<doubled_type>(Rescale(other))) >>
                       scale;
    CheckOverflow(res);
    value_ = static_cast<basic_type>(res);
    return *this;
  }

  template <typename other_type, typename doubled_other, size_t other_scale>
  constexpr fixed operator/(
      const fixed<other_type, doubled_other, other_scale>& other) {
    if (Rescale(other) == 0) {
      throw std::overflow_error("Division by zero!");
    }
    doubled_type res = ((static_cast<doubled_type>(value_) << scale) /
                        static_cast<doubled_type>(Rescale(other)));
    CheckOverflow(res);
    float sm = static_cast<float>(res) / (1 << scale);
    return fixed<basic_type, doubled_type, scale>(sm);
  }

  template <typename other_type, typename doubled_other, size_t other_scale>
  constexpr fixed& operator/=(
      const fixed<other_type, doubled_other, other_scale>& other) {
    if (Rescale(other) == 0) {
      throw std::overflow_error("Division by zero!");
    }
    doubled_type res = ((static_cast<doubled_type>(value_) << scale) /
                        static_cast<doubled_type>(Rescale(other)));
    CheckOverflow(res);
    value_ = static_cast<basic_type>(res);
    return *this;
  }

  constexpr fixed operator<<(int shift) {
    doubled_type res = value_ * (1 << shift);
    CheckOverflow(res);
    value_ = res;
    return *this;
  }

  constexpr fixed operator>>(int shift) {
    doubled_type res = value_ / (1 << shift);
    CheckOverflow(res);
    value_ = res;
    return *this;
  }

 private:
  basic_type value_ = 0;

  template <typename other_type, typename doubled_other, size_t other_scale>
  constexpr basic_type Rescale(
      const fixed<other_type, doubled_other, other_scale>& other) {
    doubled_type return_value = other.GetScaledInt();
    if (scale > other_scale) {
      return_value *= (1 << (scale - other_scale));
    }
    if (other_scale > scale) {
      return_value /= (1 << (other_scale - scale));
    }
    return return_value;
  }

  constexpr void CheckOverflow(doubled_type val) {
    if (val > std::numeric_limits<basic_type>::max()) {
      throw std::overflow_error(
          "Overflow in arithmetic with fixed point types");
    }
    if (val < std::numeric_limits<basic_type>::min()) {
      throw std::overflow_error(
          "Underflow in arithmetic with fixed point types");
    }
  }
};

#endif  // FIXEDPOINT
#ifndef FIXEDPOINT
#define FIXEDPOINT

#include <limits>

template <size_t scale, typename basic_type = int32_t,
          typename larger_type = int64_t>
class fixed {
 public:
  template <typename T>
  constexpr fixed(const T num) {
    if (sizeof(basic_type) >= sizeof(larger_type)) {
      throw std::logic_error("Your type for overflow handling is too small!");
    }
    larger_type res =
        num * static_cast<double>(1 << scale) + (num >= 0 ? 0.5 : -0.5);
    CheckOverflow(res);
    value_ = static_cast<basic_type>(res);
  }

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr fixed(const fixed<other_scale, other_type, larger_other>& other) {
    CheckTypes(other);
    value_ = (other.GetScaledInt() << scale) >> other_scale;
  }

  constexpr basic_type GetScaledInt() const { return value_; }

  constexpr float ToFloat() const {
    return static_cast<float>(value_) / (1 << scale);
  }

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr fixed operator+(
      const fixed<other_scale, other_type, larger_other>& other) {
    larger_type res = Rescale(other) + value_;
    CheckOverflow(res);
    float sm = static_cast<float>(res) / (1 << scale);
    return fixed<scale, basic_type, larger_type>(sm);
  }

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr fixed& operator+=(
      const fixed<other_scale, other_type, larger_other>& other) {
    larger_type res = value_ + Rescale(other);
    CheckOverflow(res);
    value_ = static_cast<basic_type>(res);
    return *this;
  }

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr fixed operator-(
      const fixed<other_scale, other_type, larger_other>& other) {
    larger_type res = value_ - Rescale(other);
    CheckOverflow(res);
    float sm = static_cast<float>(res) / (1 << scale);
    return fixed<scale, basic_type, larger_type>(sm);
  }

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr fixed& operator-=(
      const fixed<other_scale, other_type, larger_other>& other) {
    larger_type res = value_ - Rescale(other);
    CheckOverflow(res);
    value_ = static_cast<basic_type>(res);
    return *this;
  }

  constexpr fixed operator-() {
    value_ = -value_;
    return *this;
  }

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr fixed operator*(
      const fixed<other_scale, other_type, larger_other>& other) {
    larger_type res = (static_cast<larger_type>(value_) *
                       static_cast<larger_type>(Rescale(other))) >>
                      scale;
    CheckOverflow(res);
    float sm = static_cast<float>(res) / (1 << scale);
    return fixed<scale, basic_type, larger_type>(sm);
  }

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr fixed& operator*=(
      const fixed<other_scale, other_type, larger_other>& other) {
    larger_type res = (static_cast<larger_type>(value_) *
                       static_cast<larger_type>(Rescale(other))) >>
                      scale;
    CheckOverflow(res);
    value_ = static_cast<basic_type>(res);
    return *this;
  }

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr fixed operator/(
      const fixed<other_scale, other_type, larger_other>& other) {
    if (Rescale(other) == 0) {
      throw std::overflow_error("Division by zero!");
    }
    larger_type res = ((static_cast<larger_type>(value_) << scale) /
                       static_cast<larger_type>(Rescale(other)));
    CheckOverflow(res);
    float sm = static_cast<float>(res) / (1 << scale);
    return fixed<scale, basic_type, larger_type>(sm);
  }

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr fixed& operator/=(
      const fixed<other_scale, other_type, larger_other>& other) {
    if (Rescale(other) == 0) {
      throw std::overflow_error("Division by zero!");
    }
    larger_type res = ((static_cast<larger_type>(value_) << scale) /
                       static_cast<larger_type>(Rescale(other)));
    CheckOverflow(res);
    value_ = static_cast<basic_type>(res);
    return *this;
  }

 private:
  basic_type value_ = 0;

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr void CheckTypes(
      const fixed<other_scale, other_type, larger_other>&) const {
    if (!std::is_same<basic_type, other_type>::value ||
        !std::is_same<larger_type, larger_other>::value) {
      throw std::invalid_argument("Basic or doubled types do not match!");
    }
  }

  template <size_t other_scale, typename other_type, typename larger_other>
  constexpr basic_type Rescale(
      const fixed<other_scale, other_type, larger_other>& other) const {
    CheckTypes(other);
    larger_type return_value = other.GetScaledInt();
    if (scale > other_scale) {
      return_value *= (1 << (scale - other_scale));
    }
    if (other_scale > scale) {
      return_value /= (1 << (other_scale - scale));
    }
    return return_value;
  }

  constexpr void CheckOverflow(larger_type val) const {
    if (val > std::numeric_limits<basic_type>::max()) {
      throw std::overflow_error(
          "Overflow in arithmetic with fixed point types");
    }
    if (val < std::numeric_limits<basic_type>::min()) {
      throw std::underflow_error(
          "Underflow in arithmetic with fixed point types");
    }
  }
};

#endif  // FIXEDPOINT
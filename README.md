# Fixed Point Arithmetic 

## Overview

This library provides a template-based implementation of fixed-point arithmetic in C++. Fixed-point arithmetic is a way of representing fractional numbers by using integers, which is useful in systems where floating-point support is limited or when higher performance is required.

## Features

- **Fixed-point representation:** Supports different scales and underlying types.
- **Arithmetic operations:** Addition, subtraction, multiplication, and division.
- **Overflow and underflow checking:** Ensures arithmetic operations remain within the bounds of the specified types.
- **Type safety:** Template parameters ensure type consistency across operations.

### Usage

Create a fixed-point number by specifying the scale and the underlying types. For example, to create a fixed-point number with a scale of 8 using `int32_t` as the basic type:

```cpp
fixed<8, int32_t> a(3.14);
```

Perform arithmetic operations:

```cpp
fixed<8, int32_t> a(3.14);
fixed<8, int32_t> b(2.71);

fixed<8, int32_t> sum = a + b;
fixed<8, int32_t> diff = a - b;
fixed<8, int32_t> prod = a * b;
fixed<8, int32_t> quot = a / b;
```

## Template Parameters

- `scale`: The scaling factor (number of fractional bits).
- `basic_type`: The underlying integer type (default is `int32_t`).
- `larger_type`: A larger integer type for intermediate calculations to handle overflow (default is `int64_t`).

### Building the Project with CMake

To build the project using CMake, follow these steps:

1. Go to the project folder:
    ```sh
    cd <project_folder>
    ```

2. Create a build directory:
    ```sh
    mkdir build
    ```

3. Navigate to the build directory:
    ```sh
    cd build
    ```

4. Run CMake to configure the project:
    ```sh
    cmake ../src
    ```

5. Build the project using `make`:
    ```sh
    make
    ```

6. Run the resulting executable:
    ```sh
    ./fixed_point
    ```

## Example

```cpp
#include <iostream>
#include <cmath>

#include "h/fixed_point.h"

int main() {
    fixed<16> g(7L);
    fixed<8> f(10.25f);
    g /= f;
    std::cout << "Quotient: " << g.ToFloat() << std::endl;

    fixed<4, int8_t, int16_t> h(std::sqrt(2));
    fixed<4, int8_t, int16_t> q(std::sqrt(3));
    h -= q;
    std::cout << "Difference: " << h.ToFloat() << std::endl;
    
    return 0;
}
```

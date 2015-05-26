[![Build Status](https://travis-ci.org/lubkoll/RFFGen.svg?branch=master)](https://travis-ci.org/lubkoll/RFFGen/builds) [![Coverage Status](https://coveralls.io/repos/lubkoll/RFFGen/badge.svg)](https://coveralls.io/r/lubkoll/RFFGen)

# RFFGen
A fast and easy to use function generation library that gives provides access to the first three derivatives of a function.

Function arguments can be matrices and/or scalars. Vectors are also possible but most functionality targets on matrices.

# Installation
This library is header-only and no installation is required.

# Examples

Usage is illustrated using three different examples. Further examples can be found in RFFGen/Tests and RFFGen/Examples.

### A simple example
We begin with the function

![equation](http://latex.codecogs.com/gif.latex?f%28x%29%3D%5Csqrt%7Bx%5E3%7D+%5Csin%28%5Csqrt%7Bx%7D%29%3D%28h%5Ccirc%20g%29%28x%29)

with

![equation](http://latex.codecogs.com/gif.latex?h%28x%29%3Dx%5E3+%5Csin%28x%29)

and

![equation](http://latex.codecogs.com/gif.latex?g%28x%29%3D%5Csqrt%7Bx%7D)
```
#include "RFFGen.hh"

auto generateFunction()
{
  using namespace RFFGen::CMath;
  
  auto h = Pow<3>() + Sin();
  auto g = Sqrt();
  
  return RFFGen::finalize_scalar( h(g) );
}
```
Usage is as follows:
```
auto f = generateFunction();

f.update(1.) //  change function argument

auto value = f(); // or f.d0()
auto firstDerivative  = f.d1();
auto secondDerivative = f.d2();
auto thirdDerivative  = f.d3();
```
The call to `finalize_scalar` is necessary to simplify usage of the generated function. Without usage is slightly less convenient as illustrated in the other examples below. 


Contact: lars.lubkoll@posteo.de

[![Build Status](https://travis-ci.org/lubkoll/RFFGen.svg?branch=master)](https://travis-ci.org/lubkoll/RFFGen/builds) [![Coverage Status](https://coveralls.io/repos/lubkoll/RFFGen/badge.svg)](https://coveralls.io/r/lubkoll/RFFGen)

# RFFGen
A fast and easy to use function generation library that gives provides access to the first three derivatives of a function.

Function arguments can be matrices and/or scalars. Vectors are also possible but most functionality targets on matrices.

# Installation
This library is header-only and no installation is required.

# Examples

Usage is illustrated using three different examples. Further examples can be found in RFFGen/Tests and RFFGen/Examples.

### A simple example
![equation](http://latex.codecogs.com/gif.latex?1%2Bsin%28mc%5E2%29%0D%0A)
We begin with the function $f(x)=\sqrt(x^3)+\sin(\sqrt(x))=(h\circ g)(x)$ with \f$ h(x) = x^3+\sin(x) \f$ and \f$ g(x) = \sqrt(x) \f$:
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

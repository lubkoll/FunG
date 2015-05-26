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

### An example from biomechanics
Now let us generate the function

![equation](http://latex.codecogs.com/gif.latex?W%28F%29%3D%20c_%7BCells%7D%28%5Ciota_1-3%29+%20%5Cfrac%7Bk_1%7D%7Bk_2%7D%5Cexp%28k_2%5B%5Ckappa%5Ciota_1+%281-3%5Ckappa%29%5Ciota_4-1%5D%5E2-1%29)

which constitutes a model for adipose tissue. In this context the invariants depend on the left Cauchy-Green strain tensor. Its implementation is
```
#include "RFFGen.hh"

template <class Mat>
auto generateAdiposeTissue(double cCells, double k1, double k2,
             double kappa, const Matrix& M, const Matrix& F)
{
  using namespace RFFGen::LinearAlgebra;
  using RFFGen::CMath::exp;

  auto i1 = FirstPrincipalInvariant<Matrix>();
  auto si1 = ShiftedFirstPrincipalInvariant<Matrix>();
  auto i4 = FirstMixedInvariant<Matrix>(F,M);
 	
  auto aniso = kappa*i1 + (1-3*kappa) * i4 - 1;
  auto materialLaw = cCells*si1 + (k1/k2) * ( exp(aniso^2) - 1 );

 return finalize( materialLaw( LeftCauchyGreenStrainTensor<Matrix>(F) ) );
}
```

Usage is as follows (assuming that the material parameters, the structural tensor M, the deformation gradient F and perturbations dF0, dF1, dF2 of the latter are given):
```
auto f = generateAdiposeTissue(cCells,k1,k2,kappa,M,F);

f.update(F) //  change function argument

double value = f(); // or f.d0()
double firstDerivative  = f.d1(dF0);
double secondDerivative = f.d2(dF0,dF1);
double thirdDerivative  = f.d3(dF0,dF1,dF2);
```

Observe that the derivatives are DIRECTIONAL derivatives. This approach admits to work with any (reasonable) input types. In the first example, where the input variable was a scalar, the call to finalize_scalar did generate 
suitable default values for the directions. Here this is not admissible. 


Contact: lars.lubkoll@posteo.de

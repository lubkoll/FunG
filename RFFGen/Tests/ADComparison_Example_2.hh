/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library RFFGen.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   RFFGen is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   RFFGen is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with RFFGen.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ADCOMPARISON_EXAMPLE_2_HH
#define ADCOMPARISON_EXAMPLE_2_HH

#include <chrono>
#include <iostream>
#include <vector>

#include <fadiff.h>
#include <badiff.h>

#include <Sacado.hpp>

#include <adept.h>

#include <casadi/casadi.hpp>
#include <casadi/core/function/custom_function.hpp>
#include <casadi/core/functor.hpp>

#include "RFFGen.hh"

using namespace fadbad;
using namespace casadi;

using CustomEvaluateCPtr = void (*)(CustomFunction& f, void* user_data);

namespace Example_2
{
  void casadiFunction(CustomFunction& f, void*)
  {
    DMatrix x = f.input(0);
    auto z = exp(sqrt(x))+1;
    f.setOutput(sin(z)+x*z);
  }

  template <class T=double>
  struct Func2
  {
    T operator()(const T& x) const
    {
      z0 = sqrt(x);
      z = exp(z0);
      dz = 0.5*z/z0;
      return x*(z+1) + sin(z+1);
    }

    T d1(const T& x) const
    {
      return z+1 + (x*dz) + cos(z+1)*dz;
    }
    mutable T z, z0, dz;
  };

  struct Func
  {
    template <typename T>
    T operator()(const T& x) const
    {
      auto z = exp(sqrt(x))+1;
      return x*z+sin(z);
    }

    template <class T>
    T d1(const T& x) const
    {
      auto z0 = sqrt(x);
      auto z = exp(z0);
      auto dz = 0.5*z/z0;
      return z+1 + x*dz + cos(z+1)*dz;
    }

  };

  template <class adouble>
  adouble adeptFunc(const adouble x)
  {
    adouble z = exp(sqrt(x))+1;
    return x*z+sin(z);
  }

  auto generateTestFunction()
  {
    using namespace RFFGen;
    using namespace RFFGen::CMath;
    auto g = exp(root) + 1;//( Exp() << Sqrt() ) + 1;
    auto f = identity(1.) * g + sin(g);
    return Finalize<decltype(f),true>(f);
  }

  template <class Scalar>
  Scalar func(const Scalar& x)
  {
    return Func()(x);
  }

  template <typename C>
  struct FDiff
  {
    template <typename T>
    T operator()( T& o_dfdx, const T& i_x)
    {
      F<T> x(i_x); // Initialize arguments
      x.diff(0,1);        // Differentiate wrt. x
      C func;             // Instantiate functor
      F<T> f(func(x));  // Evaluate function and record DAG
      o_dfdx=f.d(0);      // Value of df/dx
      return f.x();       // Return function value
    }
  };

  template <typename C>
  struct BDiff
  {
    template <class T>
    T operator()( T& o_dfdx, const T& i_x )
    {
      B<T> x(i_x); // Initialize arguments
      C func;             // Instantiate functor
      B<T> f(func(x));  // Evaluate function and record DAG
      f.diff(0,1);        // Differentiate
      o_dfdx=x.d(0);      // Value of df/dx
      return f.x();       // Return function value
    }
  };
}

void ADComparison_Example_2()
{
  using std::cout;
  using std::endl;
  using namespace std::chrono;
  auto iter = 10u * 1000u * 1000u;

  cout << "Comparing different automatic differentation implementations\n" << endl;
  cout << "Function: x * ( exp( sqrt(x) ) + 1 ) + sin( exp( sqrt(x) ) + 1 )" << endl;
  cout << "iter: " << iter << endl << endl;

  double x,f,dfdx;    // Declare variables
  x=1;                       // Initialize variable x

  cout << "FADBAD++ (forward)" << endl;
  Example_2::FDiff<Example_2::Func> FFunc;         // Functor for function and derivatives
  auto startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f=FFunc(dfdx,x);    // Evaluate function and derivatives
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;
  x=1;
  cout << "FADBAD++ (backward)" << endl;
  Example_2::BDiff<Example_2::Func> BFunc;         // Functor for function and derivatives
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i){
    x*=1.00000001;
    f=BFunc(dfdx,x);    // Evaluate function and derivatives
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;

  cout << "SACADO (FAD): FAILED\n" << endl;
  cout << "SACADO (ELRFAD): FAILED\n" << endl;
//  int num_deriv = 1;
//  // SACADO (FAD) and SACADO (ELRFAD) fails on this example
//  x=1;
//  cout << "SACADO (FAD)" << endl;
//  Sacado::Fad::SFad<double,1> rfad;
//  startTime = high_resolution_clock::now();
//  for(auto i=0u; i<iter; ++i){
//    x*=1.00000001;
//    Sacado::Fad::SFad<double,1> xfad(num_deriv,0,x);
//    rfad = Example_2::func(xfad);
//  }
//  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
//  cout << "function value: " << rfad.val() << endl;
//  cout << "first derivative: " << rfad.dx(0) << endl << endl;

//  cout << "SACADO (ELRFAD)" << endl;
//  Sacado::ELRFad::DFad<double> relrfad, xelrfad;
//  timer.stop(); timer.start();
//  x=1;
//  for(auto i=0u; i<iter; ++i)
//  {
//    x*=1.00000001;
//    xelrfad = Sacado::ELRFad::DFad<double> (num_deriv,0,x);
//    relrfad = Example_2::func(xelrfad);
//  }
//  cout << "computation time: " << boost::timer::format(timer.elapsed());
//  cout << "function value: " << relrfad.val() << endl;
//  cout << "first derivative: " << relrfad.dx(0) << endl << endl;

  cout << "CASADI" << endl;
  std::vector<Sparsity> ins = { Sparsity::dense(1,1) },
      outs = { Sparsity::dense(1,1) };

  CustomFunction casadiF(Example_2::casadiFunction,ins,outs);
  startTime = high_resolution_clock::now();
  std::remove_reference_t<decltype(casadiF.output())> cf;
  for(auto i=0u; i<iter; ++i)
  {
    casadiF.init();
    casadiF.setInput(x);
    casadiF.evaluate();
    cf = casadiF.output();
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << cf << endl;
  x=1;

  cout << "ADEPT" << endl;
  startTime = high_resolution_clock::now();
  adept::Stack stack;
  using adept::adouble;
  adouble ax = x;
  adouble y;
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    ax = x;
    stack.new_recording();
    y = Example_2::adeptFunc(ax);
    y.set_gradient(1.);
    stack.compute_adjoint();
    f = y.value();
    dfdx = ax.get_gradient();
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now()-startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;
  x=1;
  cout << "SFGEN" << endl;
  auto testF = Example_2::generateTestFunction();
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    testF.update(x);
    f = testF();
    dfdx = testF.d1();
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;

  x=1;
  startTime = high_resolution_clock::now();
  Example_2::Func function;
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f = function(x);
    dfdx = function.d1(x);
  }
  cout << "manual computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;
  x=1;
  Example_2::Func2<double> optfunction;
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f = optfunction(x);
    dfdx = optfunction.d1(x);
  }
  cout << "optimized manual computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;
}

#endif // ADCOMPARISON_EXAMPLE_2_HH

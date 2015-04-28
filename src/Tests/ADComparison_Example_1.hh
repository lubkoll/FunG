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

#ifndef ADCOMPARISON_EXAMPLE_1_HH
#define ADCOMPARISON_EXAMPLE_1_HH

#include <iostream>
#include <vector>

#include <boost/timer/timer.hpp>


#include <fadiff.h>
#include <badiff.h>

#include <Sacado.hpp>

#include <casadi/casadi.hpp>
#include <casadi/core/function/custom_function.hpp>
#include <casadi/core/functor.hpp>

#include "RFFGen.hh"

using namespace fadbad;
using namespace casadi;

using CustomEvaluateCPtr = void (*)(CustomFunction& f, void* user_data);

namespace Example_1
{
  void casadiFunction(CustomFunction& f, void*)
  {
    DMatrix x = f.input(0);
    auto z = sqrt(x);
    f.setOutput(sin(z)+x*z);
  }

  template <class T=double>
  struct Func2
  {
    T operator()(const T& x) const
    {
      z = sqrt(x);
      return x*z + sin(z);
    }

    T d1(const T& x) const
    {
      return 1.5*z + 0.5*cos(z)/z;
    }
    mutable T z;
  };

  struct Func
  {
    template <typename T>
    T operator()(const T& x) const
    {
      T z=sqrt(x);
      return x*z+sin(z);
    }

    template <class T>
    T d1(const T& x) const
    {
      T z = sqrt(x);
      return 1.5*z+0.5*cos(z)/z;
    }

  };

  auto generateTestFunction()
  {
    auto f = ( RFFGen::CMath::Pow<3>() + RFFGen::CMath::Sin() ) << RFFGen::CMath::Sqrt();
    return RFFGen::Finalize<decltype(f),true>(f);
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

void ADComparison_Example_1()
{
  using std::cout;
  using std::endl;
  using namespace FunctionGeneration;
  auto iter = 10u * 1000u * 1000u;

  cout << "Comparing different automatic differentation implementations\n" << endl;
  cout << "Function: x^1.5 + sin(x^0.5)" << endl;
  cout << "iter: " << iter << endl << endl;

  double x,f,dfdx;    // Declare variables
  x=5;                       // Initialize variable x

  cout << "FADBAD++ (forward)" << endl;
  Example_1::FDiff<Example_1::Func> FFunc;         // Functor for function and derivatives
  boost::timer::cpu_timer timer;
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f=FFunc(dfdx,x);    // Evaluate function and derivatives
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed());
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;
  x=5;
  cout << "FADBAD++ (backward)" << endl;
  Example_1::BDiff<Example_1::Func> BFunc;         // Functor for function and derivatives
  timer.stop(); timer.start();
  for(auto i=0u; i<iter; ++i){
    x*=1.00000001;
    f=BFunc(dfdx,x);    // Evaluate function and derivatives
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed());
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;
  x=5;
  cout << "SACADO (FAD)" << endl;
  int num_deriv = 1;
  Sacado::Fad::DFad<double> rfad;
  timer.stop(); timer.start();
  for(auto i=0u; i<iter; ++i){
    x*=1.00000001;
    Sacado::Fad::DFad<double> xfad(num_deriv,0,x);
    rfad = Example_1::func(xfad);
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed());
  cout << "function value: " << rfad.val() << endl;
  cout << "first derivative: " << rfad.dx(0) << endl << endl;

  cout << "SACADO (ELRFAD)" << endl;
  Sacado::ELRFad::DFad<double> relrfad, xelrfad;
  timer.stop(); timer.start();
  x=5;
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    xelrfad = Sacado::ELRFad::DFad<double> (num_deriv,0,x);
    relrfad = Example_1::func(xelrfad);
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed());
  cout << "function value: " << relrfad.val() << endl;
  cout << "first derivative: " << relrfad.dx(0) << endl << endl;

  cout << "CASADI" << endl;
  std::vector<Sparsity> ins = { Sparsity::dense(1,1) },
      outs = { Sparsity::dense(1,1) };

  CustomFunction casadiF(Example_1::casadiFunction,ins,outs);
  timer.stop(); timer.start();
  std::remove_reference_t<decltype(casadiF.output())> cf;
  for(auto i=0u; i<iter; ++i)
  {
    casadiF.init();
    casadiF.setInput(x);
    casadiF.evaluate();
    cf = casadiF.output();
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed());
  cout << "function value: " << cf << endl;

  x=5;
  cout << "SFGEN" << endl;
  auto testF = Example_1::generateTestFunction();
  timer.stop(); timer.start();

  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f = testF(x);
    dfdx = testF.d1();
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed(),9);
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;

  x=5;
  timer.stop(); timer.start();

  Example_1::Func function;
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f = function(x);
    dfdx = function.d1(x);
  }
  cout << "manual computation time: " << boost::timer::format(timer.elapsed(),9);
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;
  x=5;
  timer.stop(); timer.start();
  Example_1::Func2<double> optfunction;
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f = optfunction(x);
    dfdx = optfunction.d1(x);
  }
  cout << "optimized manual computation time: " << boost::timer::format(timer.elapsed(),9);
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;
}

#endif // ADCOMPARISON_EXAMPLE_1_HH

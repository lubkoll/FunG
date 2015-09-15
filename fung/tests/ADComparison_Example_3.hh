/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library FunG.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   FunG is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   FunG is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with FunG.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ADCOMPARISON_EXAMPLE_3_HH
#define ADCOMPARISON_EXAMPLE_3_HH

#include <chrono>
#include <iostream>
#include <vector>

#include <fadiff.h>
#include <badiff.h>

#include <Sacado.hpp>

#include <casadi/casadi.hpp>
#include <casadi/core/function/custom_function.hpp>
#include <casadi/core/functor.hpp>

#include <adept.h>

#include "fung/fung.hh"

using namespace casadi;

using CustomEvaluateCPtr = void (*)(CustomFunction& f, void* user_data);

namespace Example_3
{
  void casadiFunction(CustomFunction& f, void*)
  {
    DMatrix x = f.input(0);
    DMatrix y = f.input(1);
    auto z = sqrt(x);
    f.setOutput(sin(z)+y*z);
  }

  template <class T=double>
  struct Func2
  {
    T operator()(const T& x, const T& y) const
    {
      z = sqrt(x);
      return y*z + sin(z);
    }

    T d1(const T& x, const T& y, int id) const
    {
      if(id==0) return 0.5*(y+cos(z))/z;
      return z;
    }
    mutable T z;
  };

  struct Func
  {
    template <typename T>
    T operator()(const T& x, const T& y) const
    {
      T z=sqrt(x);
      return y*z+sin(z);
    }

    template <class T>
    T d1(const T& x, const T& y, int id) const
    {
      T z = sqrt(x);
      if(id==1) return z;
      return 0.5*(y+cos(z))/z;
    }

  };

  template <class adouble>
  adouble adeptFunc(const adouble x[2])
  {
    adouble z = sqrt(x[0]);
    return x[1]*z+sin(z);
  }

  auto generateTestFunction()
  {
    using namespace FunG::CMath;
    using namespace FunG;

    auto g = Sqrt() << variable<0>(1.);
    auto f = variable<1>(1.) * g + ( Sin() << g );

    return Finalize<decltype(f),true>(f);
  }

  template <class Scalar>
  Scalar func(const Scalar& x, const Scalar& y)
  {
    return Func()(x,y);
  }

  template <typename C>
  struct FDiff
  {
    template <typename T>
    T operator()( T& o_dfdx, T& o_dfdy, const T& i_x, const T& i_y)
    {
      using namespace fadbad;
      F<T,2> x(i_x), y(i_y); // Initialize arguments
      x.diff(0);        // Differentiate wrt. x
      y.diff(1);
      C func;             // Instantiate functor
      F<T,2> f(func(x,y));  // Evaluate function and record DAG
      o_dfdx=f.d(0);      // Value of df/dx
      o_dfdy=f.d(1);
      return f.x();       // Return function value
    }
  };

  template <typename C>
  struct BDiff
  {
    template <class T>
    T operator()( T& o_dfdx, T& o_dfdy, const T& i_x , const T& i_y)
    {
      using namespace fadbad;
      B<T> x(i_x), y(i_y); // Initialize arguments
      C func;             // Instantiate functor
      B<T> f(func(x,y));  // Evaluate function and record DAG
      f.diff(0,2);        // Differentiate
      f.diff(1,2);
      o_dfdx=x.d(0);      // Value of df/dx
      o_dfdy=x.d(1);
      return f.x();       // Return function value
    }
  };
}

void ADComparison_Example_3()
{
  using std::cout;
  using std::endl;
  using namespace std::chrono;
  auto iter = 10u * 1000u * 1000u;

  cout << "Comparing different automatic differentation implementations\n" << endl;
  cout << "Function: y*x^0.5 + sin(x^0.5)" << endl;
  cout << "iter: " << iter << endl << endl;

  double x,y=1,f,dfdx,dfdy;    // Declare variables
  x=5;                       // Initialize variable x

  cout << "FADBAD++ (forward)" << endl;
  Example_3::FDiff<Example_3::Func> FFunc;         // Functor for function and derivatives
  auto startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f=FFunc(dfdx,dfdy,x,y);    // Evaluate function and derivatives
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative(0): " << dfdx << endl;
  cout << "first derivative(1): " << dfdy << endl << endl;
  x=5;
  cout << "FADBAD++ (backward)" << endl;
  Example_3::BDiff<Example_3::Func> BFunc;         // Functor for function and derivatives
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i){
    x*=1.00000001;
    f=BFunc(dfdx,dfdy,x,y);    // Evaluate function and derivatives
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative(0): " << dfdx << endl;
  cout << "first derivative(1): " << dfdy << endl << endl;
  x=5;
  cout << "SACADO (FAD)" << endl;
  int num_deriv = 2;
  Sacado::Fad::SFad<double,2> rfad;
  Sacado::Fad::SFad<double,2> yfad(num_deriv,1,y);
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i){
    x*=1.00000001;
    Sacado::Fad::SFad<double,2> xfad(num_deriv,0,x);
    rfad = Example_3::func(xfad,yfad);
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << rfad.val() << endl;
  cout << "first derivative: " << rfad.dx(0) << endl;
  cout << "second derivative: " << rfad.dx(1) << endl;

  cout << "SACADO (ELRFAD)" << endl;
  Sacado::ELRFad::DFad<double> relrfad, xelrfad, yelrfad;
  startTime = high_resolution_clock::now();
  x=5;
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    xelrfad = Sacado::ELRFad::DFad<double> (num_deriv,0,x);
    yelrfad = Sacado::ELRFad::DFad<double> (num_deriv,1,y);
    relrfad = Example_3::func(xelrfad,yelrfad);
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << relrfad.val() << endl;
  cout << "first derivative: " << relrfad.dx(0) << endl;
  cout << "second derivative: " << relrfad.dx(1) << endl;

  cout << "CASADI" << endl;
  std::vector<Sparsity> ins = { Sparsity::dense(1,1) , Sparsity::dense(1,1) },
      outs = { Sparsity::dense(1,1) , Sparsity::dense(1,1) };

  x=5;
  CustomFunction casadiF(Example_3::casadiFunction,ins,outs);
  startTime = high_resolution_clock::now();
  std::remove_reference_t<decltype(casadiF.output())> cf;
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    casadiF.init();
    casadiF.setInput(x);
    casadiF.evaluate();
    cf = casadiF.output();
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << cf << endl;
  x=5;

  cout << "ADEPT" << endl;
  startTime = high_resolution_clock::now();
  adept::Stack stack;
  using adept::adouble;
  adouble ax[2] = {x,y};
  adouble x2;
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    ax[0] = x;
//    ax[1] = y;
    stack.new_recording();
    x2 = Example_3::adeptFunc(ax);
    x2.set_gradient(1.);
    stack.compute_adjoint();
    f = x2.value();
    dfdx = ax[0].get_gradient();
    dfdy = ax[1].get_gradient();
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now()-startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative(0): " << dfdx << endl;
  cout << "first derivative(1): " << dfdy << endl << endl;

  x=5;
  cout << "SFGEN" << endl;
  auto testF = Example_3::generateTestFunction();
  testF.update<1>(y);
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    testF.template update<0>(x);
    f = testF();
    dfdx = testF.template d1<0>();
    dfdy = testF.template d1<1>();
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative(0): " << dfdx << endl;
  cout << "first derivative(1): " << dfdy << endl << endl;

  x=5;
  Example_3::Func function;
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f = function(x,y);
    dfdx = function.d1(x,y,0);
    dfdy = function.d1(x,y,1);
  }
  cout << "manual computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative(0): " << dfdx << endl;
  cout << "first derivative(1): " << dfdy << endl << endl;
  x=5;
  Example_3::Func2<double> optfunction;
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f = optfunction(x,y);
    dfdx = optfunction.d1(x,y,0);
    dfdy = optfunction.d1(x,y,1);
  }
  cout << "optimized manual computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative(0): " << dfdx << endl;
  cout << "first derivative(1): " << dfdy << endl << endl;
}

#endif // ADCOMPARISON_EXAMPLE_3_HH

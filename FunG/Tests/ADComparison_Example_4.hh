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

#ifndef ADCOMPARISON_EXAMPLE_4_HH
#define ADCOMPARISON_EXAMPLE_4_HH

#include <chrono>
#include <iostream>
#include <vector>

#include <dune/common/fmatrix.hh>

#include <fadiff.h>
#include <badiff.h>

#include <Sacado.hpp>

#include <casadi/casadi.hpp>
#include <casadi/core/function/custom_function.hpp>
#include <casadi/core/functor.hpp>

#include "FunG.hh"

using namespace casadi;

using CustomEvaluateCPtr = void (*)(CustomFunction& f, void* user_data);

namespace Example_4
{
//  template <class Mat>
//  void casadiFunction(CustomFunction& f, void*)
//  {
//    DMatrix x = f.input(0);
//    Matrix<Mat> y = f.input(1);
//    auto z = sqrt(x);
//    auto tr = y[0][0] + y[1][1] + y[2][2];
//    f.setOutput(sin(z)+tr*z);
//  }

  template <class T0, class T1>
  struct Func2
  {
    auto operator()(const T0& x, const T1& y) const
    {
      z = sqrt(x);
      tr = y[0][0] + y[1][1] + y[2][2];
      return tr*z + sin(z);
    }

    auto d1(const T0& x, const T1& y, int id) const
    {
      if(id==0) return 0.5*(tr+cos(z))/z;
      return z*tr;
    }
    mutable T0 z, tr;
  };

  struct Func
  {
    template <typename T0, typename T1>
    auto operator()(const T0& x, const T1& y_) const
    {
      auto z=sqrt(x);
      auto y = y_.val();
      auto tr = y[0][0] + y[1][1] + y[2][2];
      return tr*z+sin(z);
    }

    template <typename T0, typename T1>
    auto d1(const T0& x, const T1& y_, int id) const
    {
      auto z = sqrt(x);
      auto y = y_.val();
      auto tr = y[0][0] + y[1][1] + y[2][2];
      if(id==1) return z*tr;
      return 0.5*(tr+cos(z))/z;
    }

  };

  template <class Mat>
  auto generateTestFunction()
  {
    using namespace FunG::CMath;
    using namespace FunG::LinearAlgebra;

    auto x = FunG::Variable<double,0>();
    auto y = FunG::Variable<Mat,1>();

    auto f = trace(y) * root(x) + sine( root(x) );

    return FunG::Finalize<decltype(f)>(f);
  }

  template <class T0, class T1>
  auto func(const T0& x, const T1& y)
  {
    return Func()(x,y);
  }

  template <typename C>
  struct FDiff
  {
    template <typename T0, typename T1>
    auto operator()( T0& o_dfdx, T0& o_dfdy, const T0& i_x, const T1& i_y) const
    {
      using namespace fadbad;
      F<T0> x(i_x);
      F<T1> y(i_y); // Initialize arguments
      x.diff(0,2);        // Differentiate wrt. x
      y.diff(1,2);
      C func;             // Instantiate functor
      F<T0> f(func(x,y));  // Evaluate function and record DAG
      o_dfdx=f.d(0);      // Value of df/dx
      o_dfdy=f.d(1);
      return f.x();       // Return function value
    }
  };

//  template <typename C>
//  struct BDiff
//  {
//    template <typename T0, typename T1>
//    auto operator()( T0& o_dfdx, T1& o_dfdy, const T0& i_x, const T1& i_y) const
//    {
//      using namespace fadbad;
//      B<T> x(i_x), y(i_y); // Initialize arguments
//      C func;             // Instantiate functor
//      B<T> f(func(x,y));  // Evaluate function and record DAG
//      f.diff(0,2);        // Differentiate
//      f.diff(1,2);
//      o_dfdx=x.d(0);      // Value of df/dx
//      o_dfdy=x.d(1);
//      return f.x();       // Return function value
//    }
//  };
}

void ADComparison_Example_4()
{
  using std::cout;
  using std::endl;
  using namespace std::chrono;
  auto iter = 1u * 1000u * 1000u;

  constexpr int dim = 3;
  using Mat = Dune::FieldMatrix<double,dim,dim>;

  cout << "Comparing different automatic differentation implementations\n" << endl;
  cout << "Function: tr(y)*x^0.5 + sin(x^0.5)" << endl;
  cout << "iter: " << iter << endl << endl;

  double x, f, dfdx, dfdy;
  Mat y = FunG::LinearAlgebra::unitMatrix<Mat>();    // Declare variables
  x=5;                       // Initialize variable x

  cout << "FADBAD++ (forward)" << endl;
  Example_4::FDiff<Example_4::Func> FFunc;         // Functor for function and derivatives
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
//  x=5;
//  cout << "FADBAD++ (backward)" << endl;
//  Example_4::BDiff<Example_4::Func> BFunc;         // Functor for function and derivatives
//  startTime = high_resolution_clock::now();
//  for(auto i=0u; i<iter; ++i){
//    x*=1.00000001;
//    f=BFunc(dfdx,dfdy,x,y);    // Evaluate function and derivatives
//  }
//  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
//  cout << "function value: " << f << endl;
//  cout << "first derivative(0): " << dfdx << endl;
//  cout << "first derivative(1): " << dfdy << endl << endl;
//  x=5;
//  cout << "SACADO (FAD)" << endl;
//  int num_deriv = 2;
//  Sacado::Fad::DFad<double> rfad;
//  startTime = high_resolution_clock::now();
//  for(auto i=0u; i<iter; ++i){
//    x*=1.00000001;
//    Sacado::Fad::DFad<double> xfad(num_deriv,0,x);
//    Sacado::Fad::DFad<Mat> yfad(num_deriv,1,y);
//    rfad = Example_4::func(xfad,yfad);
//  }
//  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
//  cout << "function value: " << rfad.val() << endl;
//  cout << "first derivative: " << rfad.dx(0) << endl;
//  cout << "second derivative: " << rfad.dx(1) << endl;

//  cout << "SACADO (ELRFAD)" << endl;
//  Sacado::ELRFad::DFad<double> relrfad, xelrfad, yelrfad;
//  startTime = high_resolution_clock::now();
//  x=5;
//  for(auto i=0u; i<iter; ++i)
//  {
//    x*=1.00000001;
//    xelrfad = Sacado::ELRFad::DFad<double> (num_deriv,0,x);
//    yelrfad = Sacado::ELRFad::DFad<Mat> (num_deriv,1,y);
//    relrfad = Example_4::func(xelrfad,yelrfad);
//  }
//  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
//  cout << "function value: " << relrfad.val() << endl;
//  cout << "first derivative: " << relrfad.dx(0) << endl;
//  cout << "second derivative: " << relrfad.dx(1) << endl;

//  cout << "CASADI" << endl;
//  std::vector<Sparsity> ins = { Sparsity::dense(1,1) , Sparsity::dense(dim,dim) },
//      outs = { Sparsity::dense(1,1) , Sparsity::dense(1,1) };

//  CustomFunction casadiF(Example_4::casadiFunction<Mat>,ins,outs);
//  startTime = high_resolution_clock::now();
//  std::remove_reference_t<decltype(casadiF.output())> cf;
//  for(auto i=0u; i<iter; ++i)
//  {
//    casadiF.init();
//    casadiF.setInput(x);
//    casadiF.evaluate();
//    cf = casadiF.output();
//  }
//  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
//  cout << "function value: " << cf << endl;

  x=5;
  cout << "SFGEN" << endl;
  auto testF = Example_4::generateTestFunction<Mat>();
  testF.template update<1>(y);
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    testF.template update<0>(x);
    f = testF();
    dfdx = testF.template d1<0>(1.);
    dfdy = testF.template d1<1>(y);
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative(0): " << dfdx << endl;
  cout << "first derivative(1): " << dfdy << endl << endl;

  x=5;
//  Example_4::Func function;
//  startTime = high_resolution_clock::now();
//  for(auto i=0u; i<iter; ++i)
//  {
//    x*=1.00000001;
//    f = function(x,y);
//    dfdx = function.d1(x,y,0);
//    dfdy = function.d1(x,y,1);
//  }
//  cout << "manual computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
//  cout << "function value: " << f << endl;
//  cout << "first derivative(0): " << dfdx << endl;
//  cout << "first derivative(1): " << dfdy << endl << endl;
  x=5;
  Example_4::Func2<double,Mat> optfunction;
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

#endif // ADCOMPARISON_EXAMPLE_4_HH

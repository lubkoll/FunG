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

#ifndef ADCOMPARISON_EXAMPLE_5_HH
#define ADCOMPARISON_EXAMPLE_5_HH

#include <chrono>
#include <iostream>
#include <vector>

#include <dune/common/fmatrix.hh>

#include <fadiff.h>
#include <badiff.h>

#include <Sacado.hpp>

#include <adept.h>

#include <casadi/casadi.hpp>
#include <casadi/core/function/custom_function.hpp>
#include <casadi/core/functor.hpp>

#include "fung/fung.hh"

using namespace casadi;

using CustomEvaluateCPtr = void (*)(CustomFunction& f, void* user_data);

namespace Example_5
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

  template <class T0>
  struct Func2
  {
    auto operator()(const T0& x) const
    {
      tr = x[0][0] + x[1][1] + x[2][2];
      return tr;
    }

    auto d1(const T0& x) const
    {
      tr = x[0][0] + x[1][1] + x[2][2];
      return tr;
    }
    mutable std::decay_t<decltype(std::declval<T0>()[0][0])> tr;
  };

  struct Func
  {
    template <typename T0>
    auto operator()(const T0& x) const
    {
      auto y = x.val();
      auto tr = y[0][0] + y[1][1] + y[2][2];
      return tr;
    }

    template <typename T0>
    auto d1(const T0& x) const
    {
      auto y = x.val();
      auto tr = y[0][0] + y[1][1] + y[2][2];
      return tr;
    }
  };

  template <class adouble>
  adouble adeptFunc(const adouble x[9])
  {
    return x[0] + x[4] + x[8];
  }


  template <class Mat>
  auto generateTestFunction()
  {
    using FunG::LinearAlgebra::trace;
    using FunG::sqrt;
    using FunG::Variable;

    auto x = Variable<double,0>();
    auto F = Variable<Mat,1>();

    return FunG::finalize( sqrt(x)*trace(F) );
//    auto f = Trace<Mat>();
//    return FunG::Finalize<decltype(f)>(f);
  }

  template <class T0>
  auto func(const T0& x)
  {
    return Func()(x);
  }

  template <typename C>
  struct FDiff
  {
    template <typename T0, typename T1>
    auto operator()( T0& o_dfdx, const T1& i_x) const
    {
      using namespace fadbad;
      F<T1> x(i_x);
      x.diff(0,1);        // Differentiate wrt. x
      C func;             // Instantiate functor
      F<T0> f(func(x));  // Evaluate function and record DAG
      o_dfdx=f.d(0);      // Value of df/dx
      return f.x();       // Return function value
    }
  };

//  template <typename C>
//  struct BDiff
//  {
//    template <typename T0, typename T1>
//    auto operator()( T0& o_dfdx, const T1& i_y) const
//    {
//      using namespace fadbad;
//      B<T1> y(i_y); // Initialize arguments
//      C func;             // Instantiate functor
//      B<T0> f(func(y));  // Evaluate function and record DAG
//      f.diff(0,1);        // Differentiate
//      o_dfdx=y.d(0);      // Value of df/dx
//      return f.x();       // Return function value
//    }
//  };
}

void ADComparison_Example_5()
{
  using std::cout;
  using std::endl;
  using namespace std::chrono;
  auto iter = 10u * 1000u * 1000u;

  constexpr int dim = 3;
  using Mat = Dune::FieldMatrix<double,dim,dim>;

  cout << "Comparing different automatic differentation implementations\n" << endl;
  cout << "Function: tr(y)*x^0.5 + sin(x^0.5)" << endl;
  cout << "iter: " << iter << endl << endl;

  double x, f, dfdx, dfdy;
  Mat y = FunG::LinearAlgebra::unitMatrix<Mat>();    // Declare variables
  x=5;                       // Initialize variable x

  cout << "FADBAD++ (forward)" << endl;
  Example_5::FDiff<Example_5::Func> FFunc;         // Functor for function and derivatives
  auto startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    y*=1.00000001;
    f=FFunc(dfdx,y);    // Evaluate function and derivatives
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative(0): " << dfdx << endl;
//  x=5;
//  y = FunG::LinearAlgebra::unitMatrix<Mat>();
//  cout << "FADBAD++ (backward)" << endl;
//  Example_5::BDiff<Example_5::Func> BFunc;         // Functor for function and derivatives
//  startTime = high_resolution_clock::now();
//  for(auto i=0u; i<iter; ++i){
//    y*=1.00000001;
//    f=BFunc(dfdx,y);    // Evaluate function and derivatives
//  }
//  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
//  cout << "function value: " << f << endl;
//  cout << "first derivative(0): " << dfdx << endl << endl;
//  x=5;
  cout << "SACADO (FAD)" << endl;
  int num_deriv = 1;
  Sacado::Fad::SFad<double,1> rfad;
  y = FunG::LinearAlgebra::unitMatrix<Mat>();
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i){
    y*=1.00000001;
    Sacado::Fad::SFad<Mat,1> yfad(num_deriv,0,y);
    rfad = Example_5::func(yfad);
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << rfad.val() << endl;
  cout << "first derivative: " << rfad.dx(0) << endl;

//  cout << "SACADO (ELRFAD)" << endl;
//  Sacado::ELRFad::DFad<double> relrfad, xelrfad, yelrfad;
//  startTime = high_resolution_clock::now();
//  x=5;
//  for(auto i=0u; i<iter; ++i)
//  {
//    x*=1.00000001;
//    xelrfad = Sacado::ELRFad::DFad<double> (num_deriv,0,x);
//    yelrfad = Sacado::ELRFad::DFad<Mat> (num_deriv,1,y);
//    relrfad = Example_5::func(xelrfad,yelrfad);
//  }
//  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
//  cout << "function value: " << relrfad.val() << endl;
//  cout << "first derivative: " << relrfad.dx(0) << endl;
//  cout << "second derivative: " << relrfad.dx(1) << endl;

//  cout << "CASADI" << endl;
//  std::vector<Sparsity> ins = { Sparsity::dense(1,1) , Sparsity::dense(dim,dim) },
//      outs = { Sparsity::dense(1,1) , Sparsity::dense(1,1) };

//  CustomFunction casadiF(Example_5::casadiFunction<Mat>,ins,outs);
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

  cout << "ADEPT" << endl;
  y = FunG::LinearAlgebra::unitMatrix<Mat>();
  startTime = high_resolution_clock::now();
  adept::Stack stack;
  using adept::adouble;
  adouble ax[9] = { y[0][0], y[0][1], y[0][2], y[1][0], y[1][1], y[1][2], y[2][0], y[2][1], y[2][2] };
  adouble ar;
  for(auto i=0u; i<iter; ++i)
  {
    for(int i=0; i<9; ++i)
      ax[i]*=1.00000001;
    stack.new_recording();
    ar = Example_5::adeptFunc(ax);
    ar.set_gradient(1.);
    stack.compute_adjoint();
    f = ar.value();
    dfdx = ax[0].get_gradient();
    dfdy = ax[4].get_gradient();
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now()-startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl;
  cout << "second derivative: " << dfdy << endl;


  x=5;
  y = FunG::LinearAlgebra::unitMatrix<Mat>();
  cout << "SFGEN" << endl;
  auto testF = Example_5::generateTestFunction<Mat>();
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    y*=1.00000001;
    testF.update<0>(x);
    testF.update<1>(y);
    f = testF();
    dfdx = testF.d1<1>(y);
  }
  cout << "computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative(0): " << dfdx << endl;

  x=5;
//  Example_5::Func function;
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
  y = FunG::LinearAlgebra::unitMatrix<Mat>();
  Example_5::Func2<Mat> optfunction;
  startTime = high_resolution_clock::now();
  for(auto i=0u; i<iter; ++i)
  {
    y*=1.00000001;
    f = optfunction(y);
    dfdx = optfunction.d1(y);
  }
  cout << "optimized manual computation time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count()/1000. << "s\n";
  cout << "function value: " << f << endl;
  cout << "first derivative(0): " << dfdx << endl;
}

#endif // ADCOMPARISON_EXAMPLE_5_HH

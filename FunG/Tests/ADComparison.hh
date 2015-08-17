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

#ifndef ADCOMPARISON_HH
#define ADCOMPARISON_HH

#include <iostream>
#include <vector>

#include <boost/timer/timer.hpp>

#include <armadillo>

#include <fadiff.h>
#include <badiff.h>

#include <Sacado.hpp>

#include <casadi/casadi.hpp>
#include <casadi/core/function/custom_function.hpp>
#include <casadi/core/functor.hpp>

#include "FunG.hh"

using namespace fadbad;
using namespace casadi;

using CustomEvaluateCPtr = void (*)(CustomFunction& f, void* user_data);

void casadiFunction(CustomFunction& f, void*)
{
  DMatrix x = f.input(0);
  auto z = exp(sqrt(x))+1;
  f.setOutput(sin(z)+x*z);
}

void casadiFunction2(CustomFunction& f, void*)
{
  DMatrix x = f.input(0);
  f.setOutput(x(0,0)+x(1,1)+x(2,2));
}

namespace ADTest
{
  template <class T=double>
  struct Func2
  {
    T operator()(const T& x) const
    {
      z0 = sqrt(x);
      z = exp(z0);
      return 1+z+sin(z+1);
    }

    T d1(const T& x) const
    {
       dz = 0.5*z/z0;
      return (1 + cos(z+1))*dz;//(1.5*z) + (0.5*cos(z)/z);
    }
    mutable T z, z0, dz;
  };

  struct Func
  {
    template <typename T>
    T operator()(const T& x) const
    {
      T z=exp(sqrt(x))+1;
      return z+sin(z);
    }

    template <class T>
    T d1(const T& x) const
    {
      T z0 = sqrt(x);
      T z1 = exp(z0);
      T dz = 0.5*z1/z0;
      return (1 + cos(z1+1))*dz;//(1.5*z) + (0.5*cos(z)/z);
    }

  };

  auto generateTestFunction()
  {
    using namespace FunG;
    using M = arma::mat::fixed<3,3>;
    auto v1 = variable<1>(unitMatrix<M>());
    std::cout << HasD1MemberFunction<decltype(v1),IndexedType<double,0> >::value << std::endl;
    std::cout << hasD1MemberFunction<decltype(v1),IndexedType<M,1> >() << std::endl;
    auto f = variable<0>(1.) * ( Trace<M>() << v1 );
    //    auto f = (variable<1>(1.)^2) * ( Pow<3>() << variable<0>(1.) );
    //auto f = ( /*variable<2>(1.)**/Identity<double>() + Sin() ) << ((Exp() << Sqrt()) + 1) << variable<0>(1.) ;
//    using type = decltype(f);
//    using tvids = typename type::VariableIds;
//    auto a = tvids::openSesame;
  //  using type2 = typename ProcessType<type,0>::type;
//    auto a = type2::openSesame;
//    return Finalize<decltype(f),true>(f);
//    auto g = ProcessType<type,0>()(f);
    using V0 = FunG::Variable<double,0>;
    std::cout << "min id: " << MinVariableId<decltype(f)>::value << std::endl;
    std::cout << "max id: " << MaxVariableId<decltype(f)>::value << std::endl;
    //std::cout << "has variable (f): " << HasVariable<decltype(f)>::value << std::endl;
    std::cout << "v1: d1<1>: " << hasD1MemberFunction<decltype(v1),IndexedType<M,1>>()<< std::endl;
    std::cout << "f d1<0>: " << HasD1MemberFunction<decltype(f),IndexedType<double,0>>::value << std::endl;
    std::cout << "f d1<1>: " << HasD1MemberFunction<decltype(f),IndexedType<M,1>>::value << std::endl;
    std::cout << "f d1<2>: " << HasD1MemberFunction<decltype(f),IndexedType<double,2>>::value << std::endl;
    std::cout << "f Pow<3>: " << HasD1MemberFunction<Pow<3>,IndexedType<double,0>>::value << std::endl;
    std::cout << D1<Pow<3>,IndexedType<double,0> >::present << std::endl;
    std::cout << "f v0: " << HasD1MemberFunction<V0,IndexedType<double,0>>::value << std::endl;
    std::cout << D1<V0,IndexedType<double,0> >::present << std::endl;
//    std::cout << "has variable (g): " << HasVariable<decltype(g)>::value << std::endl;
    std::cout << "hasConsistenVariableIds (f): " << hasConsistentVariableIds<decltype(f)>() << std::endl;
//    std::cout << "hasConsistenVariableIds (g): " << hasConsistentVariableIds<decltype(g)>() << std::endl;
    return Finalize<decltype(f)>(f);
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

void ADComparison()
{
  using std::cout;
  using std::endl;
  using namespace ADTest;
  using namespace FunctionGeneration;
  auto iter = 1u;// 1000u*1000u;

  cout << "Comparing different automatic differentation implementations\n" << endl;
  cout << "Function: x^1.5 + sin(x^0.5)" << endl;
  cout << "iter: " << iter << endl << endl;

  double x,f,dfdx;    // Declare variables
  x=1;                       // Initialize variable x

  cout << "FADBAD++ (forward)" << endl;
  FDiff<Func> FFunc;         // Functor for function and derivatives
  boost::timer::cpu_timer timer;
  for(auto i=0u; i<iter; ++i)
   {
    x*=1.00000001;
    f=FFunc(dfdx,x);    // Evaluate function and derivatives
  }
    cout << "computation time: " << boost::timer::format(timer.elapsed());
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;
  x=1;
  cout << "FADBAD++ (backward)" << endl;
  BDiff<Func> BFunc;         // Functor for function and derivatives
  timer.stop(); timer.start();
  for(auto i=0u; i<iter; ++i){
    x*=1.00000001;
    f=BFunc(dfdx,x);    // Evaluate function and derivatives
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed());
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl << endl;
x=1;
  cout << "SACADO (FAD)" << endl;
  int num_deriv = 1;
  Sacado::Fad::DFad<double> rfad;
  timer.stop(); timer.start();
  for(auto i=0u; i<iter; ++i){
    x*=1.00000001;
    Sacado::Fad::DFad<double> xfad(num_deriv,0,x);
    rfad = func(xfad);
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed());
  cout << "function value: " << rfad.val() << endl;
  cout << "first derivative: " << rfad.dx(0) << endl << endl;

  cout << "SACADO (ELRFAD)" << endl;
  Sacado::ELRFad::DFad<double> relrfad, xelrfad;
  timer.stop(); timer.start();
  x=1;
  for(auto i=0u; i<iter; ++i)
  {
        x*=1.00000001;
    xelrfad = Sacado::ELRFad::DFad<double> (num_deriv,0,x);
    relrfad = func(xelrfad);
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed());
  cout << "function value: " << relrfad.val() << endl;
  cout << "first derivative: " << relrfad.dx(0) << endl << endl;

  cout << "CASADI" << endl;
  std::vector<Sparsity> ins = { Sparsity::dense(1,1) },
                        outs = { Sparsity::dense(1,1) };

  CustomFunction casadiF(casadiFunction,ins,outs);
  timer.stop(); timer.start();
  std::remove_reference_t<decltype(casadiF.output())> cf;
//  std::remove_reference_t<decltype(casadiF.gradient())> dcf;
  for(auto i=0u; i<iter; ++i)
  {
    casadiF.init();
    casadiF.setInput(x);
    casadiF.evaluate();
    cf = casadiF.output();
//    dcf = casadiF.gradient();
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed());
  cout << "function value: " << cf << endl;
//  cout << "first derivative: " << dcf << endl << endl;

//  cout << "CASADI neoHooke" << endl;
//  std::vector<Sparsity> ins2 = { Sparsity::dense(3,3) },
//                        outs2 = { Sparsity::dense(3,3) };

//  CustomFunction casadiF2(casadiFunction2,ins2,outs2);
//  timer.stop(); timer.start();
//  std::remove_reference_t<decltype(casadiF2.output())> cf2;
//  auto x0 = Sparsity::dense((3,3));
//  for(auto i=0u; i<iter; ++i)
//  {
//    casadiF2.init();
////    casadiF2.setInput(x0);
//    casadiF2.evaluate();
//    cf2 = casadiF2.output();
//  }
//  cout << "computation time: " << boost::timer::format(timer.elapsed());
//  cout << "function value: " << cf2 << endl;
x=1;
  cout << "SFGEN" << endl;
  using M = arma::mat::fixed<3,3>;
  auto unitMat = unitMatrix<M>();
  auto testF = generateTestFunction();

  x=1;
  std::cout << "iteration" << std::endl;
  f = testF();
  dfdx = testF.template d1<1>(unitMat);
  auto ddf00 = testF.template d2<0,0>(1.,1.);
  auto ddf10 = testF.template d2<1,0>(unitMat,1.);
  auto ddf01 = testF.template d2<0,1>(1.,unitMat);
  auto ddf11 = testF.template d2<1,1>(unitMat,unitMat);
//  auto dddf = testF.template d3<0,0,0>();
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl;
  cout << "second derivative: " << ddf00 << endl;
  cout << "second derivative: " << ddf01 << endl;
  cout << "second derivative: " << ddf10 << endl;
  cout << "second derivative: " << ddf11 << endl;
//  cout << "third derivative:" << dddf << endl;
  timer.stop(); timer.start();

  for(auto i=0u; i<iter; ++i)
  {
    x*=2;//1.00000001;
    testF.update<0>(x);
    testF.update<1>(unitMat);
    f = testF();
    dfdx = testF.template d1<0>(1.);
    ddf00 = testF.template d2<0,0>(1.,1.);
    ddf10 = testF.template d2<1,0>(unitMat,1.);
    ddf01 = testF.template d2<0,1>(1.,unitMat);
    ddf11 = testF.template d2<1,1>(unitMat,unitMat);
//    dddf = testF.template d3<0,0,0>();
  }
  cout << "computation time: " << boost::timer::format(timer.elapsed(),9);
  cout << "function value: " << f << endl;
  cout << "first derivative: " << dfdx << endl;
  cout << "second derivative: " << ddf00 << endl;
  cout << "second derivative: " << ddf01 << endl;
  cout << "second derivative: " << ddf10 << endl;
  cout << "second derivative: " << ddf11 << endl;
//  cout << "third derivative: " << dddf << endl;

  x=1;
  timer.stop(); timer.start();

  //  auto z = 0.;
//  auto power = 0., sine = 0.;
  Func2<> function;
  for(auto i=0u; i<iter; ++i)
  {
    x*=1.00000001;
    f = function(x);
    dfdx = function.d1(x);
    /*
    z = sqrt(x);
    power = pow(z,3);
    sine = sin(z);
    f = power + sine;
//    f = pow(z,3) + sin(z);*/
  }
  cout << "manual computation time: " << boost::timer::format(timer.elapsed(),9);
  cout << "function value: " << f << endl << endl;
  cout << "first derivative: " << dfdx << endl << endl;
}

#endif // ADCOMPARISON_HH

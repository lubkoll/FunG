// Copyright (C) 2016 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <tuple>

#include <Eigen/Dense>
#include <gtest/gtest.h>

#define FUNG_ENABLE_EXCEPTIONS
#include "fung/linear_algebra.hh"

namespace
{
  constexpr int dim = 3;

  using M = Eigen::Matrix<double,dim,dim>;
  using FunG::LinearAlgebra::computeCofactor;
  using FunG::LinearAlgebra::computeCofactorDirectionalDerivative;

  auto generateA()
  {
    M m;
    m.fill(0);
    m(0,0) = 0; m(0,1) = 1; m(0,2) = 2;
    m(1,0) = 3; m(1,1) = 4; m(1,2) = 5;
    m(2,0) = 6; m(2,1) = 7; m(2,2) = 8;
    return m;
  }

  auto generateDA()
  {
    M m;
    m.fill(0);
    m(0,0) = 1;
    m(1,1) = 1;
    m(2,2) = 1;
    return m;
  }

  template < int i, int j >
  auto symmetricDerivative(const M& A, const M& B)
  {
    return computeCofactorDirectionalDerivative< i, j >( A, B ) + computeCofactorDirectionalDerivative< i, j >( B, A );
  }

}

TEST(CofactorTest,D0)
{
  auto A = generateA();

  auto value = computeCofactor<0,0>( A );
  EXPECT_DOUBLE_EQ( value , A(1,1)*A(2,2) - A(1,2)*A(2,1) );

  value = computeCofactor<0,1>( A );
  EXPECT_DOUBLE_EQ( value , - A(1,0)*A(2,2) + A(2,0)*A(1,2) );

  value = computeCofactor<0,2>( A );
  EXPECT_DOUBLE_EQ( value , A(1,0)*A(2,1) - A(2,0)*A(1,1) );

  value = computeCofactor<1,0>( A );
  EXPECT_DOUBLE_EQ( value , - A(0,1)*A(2,2) + A(2,1)*A(0,2) );

  value = computeCofactor<1,1>( A );
  EXPECT_DOUBLE_EQ( value , A(0,0)*A(2,2) - A(2,0)*A(0,2) );

  value = computeCofactor<1,2>( A );
  EXPECT_DOUBLE_EQ( value , - A(0,0)*A(2,1) + A(2,0)*A(0,1) );

  value = computeCofactor<2,0>( A );
  EXPECT_DOUBLE_EQ( value , A(0,1)*A(1,2) - A(1,1)*A(0,2) );

  value = computeCofactor<2,1>( A );
  EXPECT_DOUBLE_EQ( value , - A(0,0)*A(1,2) + A(1,0)*A(0,2) );

  value = computeCofactor<2,2>( A );
  EXPECT_DOUBLE_EQ( value , A(0,0)*A(1,1) - A(1,0)*A(0,1) );
}

TEST(CofactorTest,D1)
{
  auto A = generateA();
  auto dA = generateDA();
  auto value = symmetricDerivative<0,0>( A, dA );
  EXPECT_DOUBLE_EQ( value , A(1,1) + A(2,2) );

  value = symmetricDerivative<0,1>( A, dA );
  EXPECT_DOUBLE_EQ( value , - A(1,0) );

  value = symmetricDerivative<0,2>( A, dA );
  EXPECT_DOUBLE_EQ( value , - A(2,0) );

  value = symmetricDerivative<1,0>( A, dA );
  EXPECT_DOUBLE_EQ( value , - A(0,1) );

  value = symmetricDerivative<1,1>( A, dA );
  EXPECT_DOUBLE_EQ( value , A(0,0) + A(2,2) );

  value = symmetricDerivative<1,2>( A, dA );
  EXPECT_DOUBLE_EQ( value , - A(2,1) );

  value = symmetricDerivative<2,0>( A, dA );
  EXPECT_DOUBLE_EQ( value , - A(0,2) );

  value = symmetricDerivative<2,1>( A, dA );
  EXPECT_DOUBLE_EQ( value , - A(1,2) );

  value = symmetricDerivative<2,2>( A, dA );
  EXPECT_DOUBLE_EQ( value , A(0,0) + A(1,1) );
}


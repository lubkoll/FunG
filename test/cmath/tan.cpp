// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>
#include "fung/cmath/tan.hh"

namespace
{
  auto x0()
  {
    return 1.0;
  }

  auto condition()
  {
    return 1e1;
  }

  auto generateTestTan()
  {
    return FunG::Tan(x0());
  }
}
TEST(TanTest,D0)
{
  auto fun = generateTestTan();
  double t = ::tan(x0());
  EXPECT_DOUBLE_EQ( fun.d0() , t );
}

TEST(TanTest,D1)
{
  auto fun = generateTestTan();
  double t = ::tan(x0());
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   ,   1 + t*t      );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , ( 1 + t*t )*dx );
}

TEST(TanTest, D1DifferentialQuotient)
{
  auto fun = generateTestTan();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update(x0() + dx);
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx*condition() );
}

TEST(TanTest,D2)
{
  auto fun = generateTestTan();
  double t = ::tan(x0());
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      ,   2 * t * ( 1 + t*t )         );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , ( 2 * t * ( 1 + t*t ) )*dx*dy );
}

TEST(TanTest,D3)
{
  auto fun = generateTestTan();
  double t = ::tan(x0());
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         , 2 * ( 1 + t*t )*( 1 + 3*t*t )          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 2 * ( 1 + t*t )*( 1 + 3*t*t )*dx*dy*dz );
}


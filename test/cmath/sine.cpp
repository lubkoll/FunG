// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>
#include "fung/cmath/sine.hh"

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

  auto generateTestSin()
  {
    return FunG::Sin( x0() );
  }
}

TEST(SineTest,D0)
{
  auto fun = generateTestSin();
  EXPECT_DOUBLE_EQ( fun.d0() , sin(x0()) );
}

TEST(SineTest,D1)
{
  auto fun = generateTestSin();
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , cos(x0())    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , cos(x0())*dx );
}

TEST(SineTest, D1DifferentialQuotient)
{
  auto fun = generateTestSin();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update(x0() + dx);
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx*condition() );
}

TEST(SineTest,D2)
{
  auto fun = generateTestSin();
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , -sin(x0())          );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , -sin(x0())*dx*dy    );
}

TEST(SineTest,D3)
{
  auto fun = generateTestSin();
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         ,  -cos(x0())          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) ,  -cos(x0())*dx*dy*dz );
}


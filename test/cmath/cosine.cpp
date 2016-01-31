// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>
#include "fung/cmath/cosine.hh"

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

  auto generateTestCos()
  {
    return FunG::Cos( x0() );
  }
}

TEST(CosineTest,D0)
{
  auto fun = generateTestCos();
  EXPECT_DOUBLE_EQ( fun.d0() ,  cos(x0()) );
}

TEST(CosineTest,D1)
{
  auto fun = generateTestCos();
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , -sin(x0())    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , -sin(x0())*dx );
}

TEST(CosineTest, D1DifferentialQuotient)
{
  auto fun = generateTestCos();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update(x0() + dx);
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx*condition() );
}

TEST(CosineTest,D2)
{
  auto fun = generateTestCos();
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , -cos(x0())          );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , -cos(x0())*dx*dy    );
}

TEST(CosineTest,D3)
{
  auto fun = generateTestCos();
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         ,  sin(x0())          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) ,  sin(x0())*dx*dy*dz );
}

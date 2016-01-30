// Copyright (C) 2016 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#define FUNG_ENABLE_EXCEPTIONS
#include <gtest/gtest.h>
#include "fung/cmath/arcsine.hh"

namespace
{
  auto x0()
  {
    return 0.5;
  }

  auto condition()
  {
    return 1e1;
  }

  auto generateTestASin()
  {
    return FunG::ASin( x0() );
  }
}

TEST(ArcsineTest,Update)
{
  auto fun = generateTestASin();
  ASSERT_THROW( fun.update(-1.1) , FunG::OutOfDomainException );
  ASSERT_THROW( fun.update(1.1) , FunG::OutOfDomainException );
}

TEST(ArcsineTest,D0)
{
  auto fun = generateTestASin();
  EXPECT_DOUBLE_EQ( fun.d0() , asin(.5) );
}

TEST(ArcsineTest,D1)
{
  auto fun = generateTestASin();
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , 1/sqrt(0.75)    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , 1/sqrt(0.75)*dx );

}

TEST(ArcsineTest, D1DifferentialQuotient)
{
  auto fun = generateTestASin();
  auto dx = 1e-8;
  auto f0 = fun();
  fun.update(x0() + dx);
  EXPECT_NEAR( fun.d1() , ( fun() - f0 )/dx , dx*condition() );
}

TEST(ArcsineTest,D2)
{
  auto fun = generateTestASin();
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , .5/pow(sqrt(0.75),3)       );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , .5/pow(sqrt(0.75),3)*dx*dy );
}

TEST(ArcsineTest,D3)
{
  auto fun = generateTestASin();
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         , 1/sqrt(pow(0.75,3)) * ( 1 + 0.75*0.75 )          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 1/sqrt(pow(0.75,3)) * ( 1 + 0.75*0.75 )*dx*dy*dz );
}



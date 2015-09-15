#include <gtest/gtest.h>
#include "fung/cmath/tan.hh"

auto generateTestTan()
{
  return FunG::CMath::Tan(1.);
}

TEST(TanTest,D0)
{
  auto fun = generateTestTan();
  double t = ::tan(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , t );
}

TEST(TanTest,D1)
{
  auto fun = generateTestTan();
  double t = ::tan(1.);
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   ,   1 + t*t      );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , ( 1 + t*t )*dx );
}

TEST(TanTest,D2)
{
  auto fun = generateTestTan();
  double t = ::tan(1.);
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      ,   2 * t * ( 1 + t*t )         );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , ( 2 * t * ( 1 + t*t ) )*dx*dy );
}

TEST(TanTest,D3)
{
  auto fun = generateTestTan();
  double t = ::tan(1.);
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         , 2 * ( 1 + t*t )*( 1 + 3*t*t )          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 2 * ( 1 + t*t )*( 1 + 3*t*t )*dx*dy*dz );
}


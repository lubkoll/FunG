#include <gtest/gtest.h>
#include "fung/cmath/sine.hh"

auto generateTestSin()
{
  return FunG::CMath::Sin(1.);
}

TEST(SineTest,D0)
{
  auto fun = generateTestSin();
  EXPECT_DOUBLE_EQ( fun.d0() , sin(1.) );
}

TEST(SineTest,D1)
{
  auto fun = generateTestSin();
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , cos(1.)    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , cos(1.)*dx );
}

TEST(SineTest,D2)
{
  auto fun = generateTestSin();
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , -sin(1.)          );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , -sin(1.)*dx*dy    );
}

TEST(SineTest,D3)
{
  auto fun = generateTestSin();
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         ,  -cos(1.)          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) ,  -cos(1.)*dx*dy*dz );
}


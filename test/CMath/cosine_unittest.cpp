#include "../../FunG/FunG/CMath/cosine.hh"

#include <gtest/gtest.h>

auto generateTestCos()
{
  return FunG::CMath::Cos(1.);
}

TEST(CosineTest,D0)
{
  auto fun = generateTestCos();
  EXPECT_DOUBLE_EQ( fun.d0() ,  cos(1.) );
}

TEST(CosineTest,D1)
{
  auto fun = generateTestCos();
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , -sin(1.)    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , -sin(1.)*dx );
}

TEST(CosineTest,D2)
{
  auto fun = generateTestCos();
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , -cos(1.)          );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , -cos(1.)*dx*dy    );
}

TEST(CosineTest,D3)
{
  auto fun = generateTestCos();
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         ,  sin(1.)          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) ,  sin(1.)*dx*dy*dz );
}

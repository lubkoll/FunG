#include "../../src/CMath/cosine.hh"

#include <gtest/gtest.h>

TEST(CosineTest,D0)
{
  const RFFGen::CMath::Cos fun(1.);
  EXPECT_DOUBLE_EQ( fun.d0() ,  cos(1.) );
}

TEST(CosineTest,D1)
{
  const RFFGen::CMath::Cos fun(1.);
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , -sin(1.)    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , -sin(1.)*dx );
}

TEST(CosineTest,D2)
{
  const RFFGen::CMath::Cos fun(1.);
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , -cos(1.)          );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , -cos(1.)*dx*dy    );
}

TEST(CosineTest,D3)
{
  const RFFGen::CMath::Cos fun(1.);
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         ,  sin(1.)          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) ,  sin(1.)*dx*dy*dz );
}

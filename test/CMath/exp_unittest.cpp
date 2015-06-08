#include "../../RFFGen/CMath/exp.hh"

#include <gtest/gtest.h>

auto generateTestExp()
{
  return RFFGen::CMath::Exp(1.);
}

TEST(ExpTest,D0)
{
  auto fun = generateTestExp();
  EXPECT_DOUBLE_EQ( fun.d0() ,  exp(1.) );
}

TEST(ExpTest,D1)
{
  auto fun = generateTestExp();
  const double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , exp(1.)    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , exp(1.)*dx );
}

TEST(ExpTest,D2)
{
  auto fun = generateTestExp();
  const double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , exp(1.)          );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , exp(1.)*dx*dy    );
}

TEST(ExpTest,D3)
{
  auto fun = generateTestExp();
  const double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         ,  exp(1.)          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) ,  exp(1.)*dx*dy*dz );
}


TEST(Exp2Test,D0)
{
  const RFFGen::CMath::Exp2 fun(1.);
  EXPECT_DOUBLE_EQ( fun.d0() ,  exp2(1.) );
}

TEST(Exp2Test,D1)
{
  const RFFGen::CMath::Exp2 fun(1.);
  const double dx = 2.;
  const double ln2 = log(2);
  EXPECT_DOUBLE_EQ( fun.d1()   , exp2(1.)*ln2    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , exp2(1.)*ln2*dx );
}

TEST(Exp2Test,D2)
{
  const RFFGen::CMath::Exp2 fun(1.);
  const double dx = 2., dy = 3.;
  const double ln2 = log(2);
  EXPECT_DOUBLE_EQ( fun.d2()      , exp2(1.)*ln2*ln2       );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , exp2(1.)*ln2*ln2*dx*dy );
}

TEST(Exp2Test,D3)
{
  const RFFGen::CMath::Exp2 fun(1.);
  const double dx = 2., dy = 3., dz = 4.;
  const double ln2 = log(2);
  EXPECT_DOUBLE_EQ( fun.d3()         ,  exp2(1.)*ln2*ln2*ln2          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) ,  exp2(1.)*ln2*ln2*ln2*dx*dy*dz );
}

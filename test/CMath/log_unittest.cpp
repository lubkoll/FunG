#include "../../src/CMath/log.hh"

#include <gtest/gtest.h>

TEST(LNTest,Update)
{
  RFFGen::CMath::LN fun(1.);
  EXPECT_THROW(fun.update(0), RFFGen::OutOfDomainException);
}

TEST(LNTest,D0)
{
  const RFFGen::CMath::LN fun(1.);
  EXPECT_DOUBLE_EQ( fun.d0() ,  log(1.) );
}

TEST(LNTest,D1)
{
  const RFFGen::CMath::LN fun(1.);
  const double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , 1. );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , dx );
}

TEST(LNTest,D2)
{
  const RFFGen::CMath::LN fun(1.);
  const double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , -1.    );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , -dx*dy );
}

TEST(LNTest,D3)
{
  const RFFGen::CMath::LN fun(1.);
  const double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         , 2.         );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 2*dx*dy*dz );
}


///
TEST(Log2Test,Update)
{
  RFFGen::CMath::Log2 fun(1.);
  EXPECT_THROW(fun.update(0), RFFGen::OutOfDomainException);
}

TEST(Log2Test,D0)
{
  const RFFGen::CMath::Log2 fun(1.);
  EXPECT_DOUBLE_EQ( fun.d0() ,  log2(1.) );
}

TEST(Log2Test,D1)
{
  const RFFGen::CMath::Log2 fun(1.);
  const double dx = 2.;
  const double ln2 = log(2);
  EXPECT_DOUBLE_EQ( fun.d1()   , 1/ln2  );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , dx/ln2 );
}

TEST(Log2Test,D2)
{
  const RFFGen::CMath::Log2 fun(1.);
  const double dx = 2., dy = 3.;
  const double ln2 = log(2);
  EXPECT_DOUBLE_EQ( fun.d2()      , -1/ln2     );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , -dx*dy/ln2 );
}

TEST(Log2Test,D3)
{
  const RFFGen::CMath::Log2 fun(1.);
  const double dx = 2., dy = 3., dz = 4.;
  const double ln2 = log(2);
  EXPECT_DOUBLE_EQ( fun.d3()         , 2/ln2          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 2/ln2*dx*dy*dz );
}


///
TEST(Log10Test,Update)
{
  RFFGen::CMath::Log10 fun(1.);
  EXPECT_THROW(fun.update(0), RFFGen::OutOfDomainException);
}

TEST(Log10Test,D0)
{
  const RFFGen::CMath::Log10 fun(1.);
  EXPECT_DOUBLE_EQ( fun.d0() ,  log10(1.) );
}

TEST(Log10Test,D1)
{
  const RFFGen::CMath::Log10 fun(1.);
  const double dx = 2.;
  const double ln10 = log(10);
  EXPECT_DOUBLE_EQ( fun.d1()   , 1/ln10  );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , dx/ln10 );
}

TEST(Log10Test,D2)
{
  const RFFGen::CMath::Log10 fun(1.);
  const double dx = 2., dy = 3.;
  const double ln10 = log(10);
  EXPECT_DOUBLE_EQ( fun.d2()      , -1/ln10     );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , -dx*dy/ln10 );
}

TEST(Log10Test,D3)
{
  const RFFGen::CMath::Log10 fun(1.);
  const double dx = 2., dy = 3., dz = 4.;
  const double ln10 = log(10);
  EXPECT_DOUBLE_EQ( fun.d3()         , 2/ln10          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 2/ln10*dx*dy*dz );
}

#include "../../src/CMath/pow.hh"
#include "../../src/generate.hh"
#include "../../src/finalize.hh"

#include <gtest/gtest.h>

TEST(SumTest,D0)
{
  using RFFGen::CMath::Pow;
  auto fun = Pow<3,1>(2.) + Pow<3,2>(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , 9. );
}

TEST(SumTest,D1)
{
  using RFFGen::CMath::Pow;
  auto fun = RFFGen::finalize_scalar( Pow<3,1>(2.) + Pow<3,2>(1.) );
  EXPECT_DOUBLE_EQ( fun.d1() , 13.5 );
}

TEST(SumTest,D2)
{
  using RFFGen::CMath::Pow;
  auto fun = RFFGen::finalize_scalar( Pow<3,1>(2.) + Pow<3,2>(1.) );
  EXPECT_DOUBLE_EQ( fun.d2() , 12.75 );
}

TEST(SumTest,D3)
{
  using RFFGen::CMath::Pow;
  auto fun = RFFGen::finalize_scalar( Pow<3,1>(2.) + Pow<3,2>(1.) );
  EXPECT_DOUBLE_EQ( fun.d3() , 5.625 );
}


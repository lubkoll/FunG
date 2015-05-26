#include "../../RFFGen/CMath/pow.hh"
#include "../../RFFGen/generate.hh"
#include "../../RFFGen/finalize.hh"

#include <gtest/gtest.h>

TEST(ScaleTest,D0)
{
  using RFFGen::CMath::Pow;
  auto fun = 2*Pow<3,1>(2.);
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
}

TEST(ScaleTest,D1)
{
  using RFFGen::CMath::Pow;
  auto fun = RFFGen::finalize_scalar ( 2*Pow<3,1>(2.) );
  EXPECT_DOUBLE_EQ( fun.d1() , 24. );
}

TEST(ScaleTest,D2)
{
  using RFFGen::CMath::Pow;
  auto fun = RFFGen::finalize_scalar( 2*Pow<3,1>(2.) );
  EXPECT_DOUBLE_EQ( fun.d2() , 24. );
}

TEST(ScaleTest,D3)
{
  using RFFGen::CMath::Pow;
  auto fun = RFFGen::finalize_scalar( 2*Pow<3,1>(2.) );
  EXPECT_DOUBLE_EQ( fun.d3() , 12. );
}



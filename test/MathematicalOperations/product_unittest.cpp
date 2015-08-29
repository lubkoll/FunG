#include "FunG/CMath/pow.hh"
#include "FunG/generate.hh"
#include "FunG/finalize.hh"

#include <gtest/gtest.h>

TEST(ProductTest,UpdateVariable)
{
  auto x = FunG::variable<0>(2.);
  using FunG::CMath::Pow;
  auto fun = ( Pow<1,2>(2.) * Pow<3,2>(2.) ) << x;
  EXPECT_DOUBLE_EQ( fun.d0() , 4. );
  EXPECT_NO_THROW( fun.updateVariable<0>(4.) );
  EXPECT_NO_THROW( fun.update(0); );
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
}

TEST(ProductTest,D0)
{
  using FunG::CMath::Pow;
  auto fun = Pow<1,2>(4.) * Pow<3,2>(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , 2. );
  fun.update(3.);
  EXPECT_DOUBLE_EQ( fun.d0() , 9. );
}

TEST(ProductTest,D1)
{
  using FunG::CMath::Pow;
  auto fun = FunG::finalize_scalar( Pow<1,2>(4.) * Pow<3,2>(4.) );
  EXPECT_DOUBLE_EQ( fun.d1() , 8. );
}

TEST(ProductTest,D2)
{
  using FunG::CMath::Pow;
  auto fun = FunG::finalize_scalar( Pow<1,2>(5.) * Pow<3,2>(5.) );
  EXPECT_DOUBLE_EQ( fun.d2() , 2. );
}

TEST(ProductTest,D3)
{
  using FunG::CMath::Pow;
  auto fun = FunG::finalize_scalar( Pow<1,2>(3.) * Pow<3,2>(3.) );
  EXPECT_DOUBLE_EQ( fun.d3() , 0. );
}



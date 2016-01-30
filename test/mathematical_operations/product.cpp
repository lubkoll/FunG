#include <gtest/gtest.h>

#include "fung/cmath/pow.hh"
#include "fung/generate.hh"
#include "fung/finalize.hh"

TEST(ProductTest,UpdateVariable)
{
  auto x = FunG::variable<0>(2.);
  using FunG::Pow;
  auto fun = ( Pow<1,2>(2.) * Pow<3,2>(2.) ) << x;
  EXPECT_DOUBLE_EQ( fun.d0() , 4. );
  EXPECT_NO_THROW( fun.update<0>(4.) );
  EXPECT_NO_THROW( fun.update(0); );
  EXPECT_DOUBLE_EQ( fun.d0() , 16. );
}

TEST(ProductTest,D0)
{
  using FunG::Pow;
  auto fun = Pow<1,2>(4.) * Pow<3,2>(1.);
  EXPECT_DOUBLE_EQ( fun.d0() , 2. );
  fun.update(3.);
  EXPECT_DOUBLE_EQ( fun.d0() , 9. );
}

TEST(ProductTest,D1)
{
  using FunG::Pow;
  auto fun = FunG::finalize( Pow<1,2>(4.) * Pow<3,2>(4.) );
  EXPECT_DOUBLE_EQ( fun.d1(1) , 8. );
}

TEST(ProductTest,D2)
{
  using FunG::Pow;
  auto fun = FunG::finalize( Pow<1,2>(5.) * Pow<3,2>(5.) );
  EXPECT_DOUBLE_EQ( fun.d2(1,1) , 2. );
}

TEST(ProductTest,D3)
{
  using FunG::Pow;
  auto fun = FunG::finalize( Pow<1,2>(3.) * Pow<3,2>(3.) );
  EXPECT_DOUBLE_EQ( fun.d3(1,1,1) , 0. );
}



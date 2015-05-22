#include "../../src/Util/chainer.hh"
#include "../../src/CMath/arcsine.hh"

#include <gtest/gtest.h>

TEST(ArcsineTest,D0)
{
  const RFFGen::CMath::ASin fun(.5);
  EXPECT_DOUBLE_EQ( fun.d0() , asin(.5) );
}

TEST(ArcsineTest,D1)
{
  const RFFGen::CMath::ASin fun(.5);
  double dx = 2.;
  EXPECT_DOUBLE_EQ( fun.d1()   , 1/sqrt(0.75)    );
  EXPECT_DOUBLE_EQ( fun.d1(dx) , 1/sqrt(0.75)*dx );
}

TEST(ArcsineTest,D2)
{
  const RFFGen::CMath::ASin fun(.5);
  double dx = 2., dy = 3.;
  EXPECT_DOUBLE_EQ( fun.d2()      , .5/pow(sqrt(0.75),3)       );
  EXPECT_DOUBLE_EQ( fun.d2(dx,dy) , .5/pow(sqrt(0.75),3)*dx*dy );
}

TEST(ArcsineTest,D3)
{
  const RFFGen::CMath::ASin fun(.5);
  double dx = 2., dy = 3., dz = 4.;
  EXPECT_DOUBLE_EQ( fun.d3()         , 1/sqrt(pow(0.75,3)) * ( 1 + 0.75*0.75 )          );
  EXPECT_DOUBLE_EQ( fun.d3(dx,dy,dz) , 1/sqrt(pow(0.75,3)) * ( 1 + 0.75*0.75 )*dx*dy*dz );
}



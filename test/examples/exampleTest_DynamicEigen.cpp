// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <tuple>

#include <Eigen/Dense>
#include <gtest/gtest.h>

#define FUNG_ENABLE_EXCEPTIONS
#include "fung/examples/rubber/neo_hooke.hh"
#include "fung/examples/rubber/mooney_rivlin.hh"
#include "fung/examples/biomechanics/skin_tissue_hendriks.hh"
#include "fung/examples/biomechanics/adipose_tissue_sommer_holzapfel.hh"
#include "fung/examples/biomechanics/muscle_tissue_martins.hh"

namespace
{
  using FunG::LN;
  using FunG::Pow;
  constexpr int dim = 3;
  using M = Eigen::MatrixXd;

  template <class Function>
  auto runTest(Function& f)
  {
    M a = FunG::LinearAlgebra::unitMatrix<M>(dim);
    M da0 = 2*a, da1 = 3*a, da2 = 4*a;

    f.update(a);

    return std::make_tuple( f() , f.d1(da0) , f.d2(da0,da1) , f.d3(da0,da1,da2) );
  }

  auto c0 = 1., c1 = 1., d0 = 1., d1 = 1.;

  auto initFiberTensor()
  {
    M m(dim,dim);
    m.fill(0);
    m(0,0) = 1;
    return m;
  }

  M fiberTensor = initFiberTensor(), I = FunG::LinearAlgebra::unitMatrix<M>(dim);
}

TEST(NeoHooke,Incompressible_Dynamic_Eigen)
{
  auto incompressibleNeoHooke = FunG::incompressibleNeoHooke<M,dim>(c0,I);
  auto f = runTest(incompressibleNeoHooke);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 12 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 36 );
  EXPECT_DOUBLE_EQ( std::get<3>(f) , 0 );
}

TEST(NeoHooke,Compressible_Dynamic_Eigen)
{
  auto compressibleNeoHooke = FunG::compressibleNeoHooke<Pow<2>,LN,M,dim>(c0,d0,d1,I);
  auto f = runTest(compressibleNeoHooke);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 30 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 198 );
  EXPECT_DOUBLE_EQ( std::get<3>(f) , 3024 );
}

TEST(NeoHooke,ModifiedIncompressible_Dynamic_Eigen)
{
  auto modifiedIncompressibleNeoHooke = FunG::modifiedIncompressibleNeoHooke<M,dim>(c0,I);
  auto f = runTest(modifiedIncompressibleNeoHooke);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 0 );
  EXPECT_NEAR( std::get<3>(f) , 0 , 1e-11);
}

TEST(NeoHooke,ModifiedCompressible_Dynamic_Eigen)
{
  auto modifiedCompressibleNeoHooke = FunG::modifiedCompressibleNeoHooke<Pow<2>,LN,M,dim>(c0,d0,d1,I);
  auto f = runTest(modifiedCompressibleNeoHooke);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 18 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 162 );
  EXPECT_NEAR( std::get<3>(f) , 3024 , 1e-11 );
}

TEST(MooneyRivlin,Incompressible_Dynamic_Eigen)
{
  auto mooneyRivlin = FunG::incompressibleMooneyRivlin<M,dim>(c0,c1,I);
  auto f = runTest(mooneyRivlin);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 36 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 252 );
  EXPECT_NEAR( std::get<3>(f) , 1728 , 1e-13 );
}

TEST(MooneyRivlin,Compressible_Dynamic_Eigen)
{
  auto mooneyRivlin = FunG::compressibleMooneyRivlin<Pow<2>,LN,M,dim>(c0,c1,d0,d1,I);
  auto f = runTest(mooneyRivlin);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 54 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 414 );
  EXPECT_NEAR( std::get<3>(f) , 4752 , 1e-13 );
}

TEST(Skin_Hendriks,Incompressible_Dynamic_Eigen)
{
  auto skin = FunG::incompressibleSkin_Hendriks<M,dim>(c0,c1,I);
  auto f = runTest(skin);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 12 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 900 );
  EXPECT_NEAR( std::get<3>(f) , 20736 , 1e-13 );
}

TEST(Skin_Hendriks,Compressible_Dynamic_Eigen)
{
  auto skin = FunG::compressibleSkin_Hendriks<Pow<2>,LN,M,dim>(c0,c1,d0,d1,I);
  auto f = runTest(skin);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 30 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 1062 );
  EXPECT_NEAR( std::get<3>(f) , 23760 , 1e-13 );
}

TEST(Adipose_SommerHolzapfel,Incompressible_Dynamic_Eigen)
{
  auto adipose = FunG::incompressibleAdiposeTissue_SommerHolzapfel<M,dim>(fiberTensor,I);
  auto f = runTest(adipose);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_NEAR( std::get<1>(f) , 1.8 , 1e-11 );
  EXPECT_NEAR( std::get<2>(f) , 43.8 , 1e-11 );
  EXPECT_NEAR( std::get<3>(f) , 460.8 , 1e-11 );
}

TEST(Adipose_SommerHolzapfel,Compressible_Dynamic_Eigen)
{
  auto adipose = FunG::compressibleAdiposeTissue_SommerHolzapfel<Pow<2>,LN,M,dim>(d0,d1,fiberTensor,I);
  auto f = runTest(adipose);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_NEAR( std::get<1>(f) , 19.8 , 1e-11 );
  EXPECT_NEAR( std::get<2>(f) , 205.8 , 1e-11 );
  EXPECT_NEAR( std::get<3>(f) , 3484.8 , 1e-11 );
}

TEST(Muscle_Martins,Incompressible_Dynamic_Eigen)
{
  auto muscle = FunG::incompressibleMuscleTissue_Martins<M,dim>(fiberTensor,I);
  auto f = runTest(muscle);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_NEAR( std::get<1>(f) , 0 , 1e-11 );
  EXPECT_NEAR( std::get<2>(f) , 0 , 1e-11 );
  EXPECT_NEAR( std::get<3>(f) , 0 , 1e-10 );
}

TEST(Muscle_Martins,Compressible_Dynamic_Eigen)
{
  auto muscle = FunG::compressibleMuscleTissue_Martins<Pow<2>,LN,M,dim>(d0,d1,fiberTensor,I);
  auto f = runTest(muscle);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_NEAR( std::get<1>(f) , 18 , 1e-11 );
  EXPECT_NEAR( std::get<2>(f) , 162 , 1e-11 );
  EXPECT_NEAR( std::get<3>(f) , 3024 , 1e-10 );
}

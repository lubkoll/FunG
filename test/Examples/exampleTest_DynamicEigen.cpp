/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library FunG.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   FunG is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   FunG is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with FunG.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <tuple>

#include <Eigen/Dense>
#include <gtest/gtest.h>

#include "Examples/Rubber/neoHooke.hh"
#include "Examples/Rubber/mooneyRivlin.hh"
#include "Examples/Biomechanics/skinTissue_Hendriks.hh"
#include "Examples/Biomechanics/adiposeTissue_SommerHolzapfel.hh"
#include "Examples/Biomechanics/muscleTissue_Martins.hh"

namespace
{
  using FunG::CMath::LN;
  using FunG::CMath::Pow;
  using FunG::LinearAlgebra::Invariant;
  constexpr int dim = 3;
  using M = Eigen::MatrixXd;

  template <class Function>
  auto runTest(Function& f)
  {
    M a = FunG::LinearAlgebra::unitMatrix<M>(dim);
    M da0 = 2*a, da1 = 3*a, da2 = 4*a;

    f.update(a);

    return std::make_tuple( f() , f.template d1<0>(da0) , f.template d2<0,0>(da0,da1) , f.template d3<0,0,0>(da0,da1,da2) );
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
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 12 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 180 );
  EXPECT_NEAR( std::get<3>(f) , 1728 , 1e-11 );
}

TEST(MooneyRivlin,Compressible_Dynamic_Eigen)
{
  auto mooneyRivlin = FunG::compressibleMooneyRivlin<Pow<2>,LN,M,dim>(c0,c1,d0,d1,I);
  auto f = runTest(mooneyRivlin);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 30 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 342 );
  EXPECT_NEAR( std::get<3>(f) , 4752 , 1e-11 );
}

TEST(Skin_Hendriks,Incompressible_Dynamic_Eigen)
{
  auto skin = FunG::incompressibleSkin_Hendriks<M,dim>(c0,c1,I);
  auto f = runTest(skin);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 12 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 36 );
  EXPECT_NEAR( std::get<3>(f) , 10368 , 1e-11 );
}

TEST(Skin_Hendriks,Compressible_Dynamic_Eigen)
{
  auto skin = FunG::compressibleSkin_Hendriks<Pow<2>,LN,M,dim>(c0,c1,d0,d1,I);
  auto f = runTest(skin);
  EXPECT_DOUBLE_EQ( std::get<0>(f) , 0 );
  EXPECT_DOUBLE_EQ( std::get<1>(f) , 30 );
  EXPECT_DOUBLE_EQ( std::get<2>(f) , 198 );
  EXPECT_NEAR( std::get<3>(f) , 13392 , 1e-11 );
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

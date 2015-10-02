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

#define FUNG_ENABLE_EXCEPTIONS
#include "fung/linear_algebra.hh"

constexpr int dim = 3;
using M = Eigen::Matrix<double,dim,dim>;

auto generateDeterminant()
{
  using FunG::LinearAlgebra::det;

  M m;
  m.fill(0);
  m(0,0) = 0; m(0,1) = 1; m(0,2) = 2;
  m(1,0) = 3; m(1,1) = 4; m(1,2) = 5;
  m(2,0) = 6; m(2,1) = 7; m(2,2) = 1;
  return det(m);
}

auto generateDA()
{
  M m;
  m.fill(0);
  m(0,0) = 1;
  m(1,1) = 1;
  m(2,2) = 1;
  return m;
}

TEST(DeterminantTest,D0)
{
  auto det = generateDeterminant();
  EXPECT_DOUBLE_EQ( det.d0() , 21. );
}

TEST(DeterminantTest,D1)
{
  auto det = generateDeterminant();
  EXPECT_DOUBLE_EQ( det.d1(generateDA()) , -46. );
}

TEST(DeterminantTest,D2)
{
  auto det = generateDeterminant();
  EXPECT_DOUBLE_EQ( det.d2(generateDA(),generateDA()) , 10. );
}


TEST(DeterminantTest,D3)
{
  auto det = generateDeterminant();
  EXPECT_DOUBLE_EQ( det.d3(generateDA(),generateDA(),generateDA()) , 6. );
}

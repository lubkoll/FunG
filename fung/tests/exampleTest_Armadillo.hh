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

#ifndef FUNG_PERFORMANCE_TEST_EXAMPLE_TEST_HH
#define FUNG_PERFORMANCE_TEST_EXAMPLE_TEST_HH

#include <chrono>
#include <iostream>

#include <armadillo>

#include "fung/examples/rubber/neo_hooke.hh"
#include "fung/examples/rubber/mooney_rivlin.hh"
#include "fung/examples/biomechanics/skin_tissue_hendriks.hh"
#include "fung/examples/biomechanics/adipose_tissue_sommer_holzapfel.hh"
#include "fung/examples/biomechanics/muscle_tissue_martins.hh"
#include "fung/math.hh"

namespace ArmadilloDetail
{
  template <class M, class Function>
  void runTest(Function& f)
  {
    using std::cout;
    using std::endl;
    using namespace std::chrono;
    auto nUpdates = 10'000'000u;
    auto nTimes = 1;
    auto nEval = nUpdates * nTimes;

    M a;
    a.eye();
    M da0 = 2*a, da1 = 3*a, da2 = 4*a;
    auto val = 0.;

    cout << "number of updates: " << nUpdates << endl;
    auto startTime = high_resolution_clock::now();
    for(auto i=0u; i<nUpdates; ++i){ if(i%2==0) a*=1.01; else a*=1./1.01; f.update(a); }
    cout << "updates: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
    cout << "number of evaluations: " << nEval << endl;
    startTime = high_resolution_clock::now();
    for(auto i=0u; i<nEval; ++i) val = f();
    cout << "d0: " << duration_cast<nanoseconds>(high_resolution_clock::now() - startTime).count() << "ns\n";
    cout << "value: " << val << endl;
    startTime = high_resolution_clock::now();
    for(auto i=0u; i<nEval; ++i) val = f.template d1<0>(da0);
    cout << "d1: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
    cout << "value: " << val << endl;
    startTime = high_resolution_clock::now();
    for(auto i=0u; i<nEval; ++i) val = f.template d2<0,0>(da0,da1);
    cout << "d2: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
    cout << "value: " << val << endl;
    startTime = high_resolution_clock::now();
    for(auto i=0u; i<nEval; ++i) val = f.template d3<0,0,0>(da0,da1,da2);
    cout << "d3: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << "ms\n";
    cout << "value: " << val << endl;
    cout << endl;
  }
}

void testExamples_Armadillo()
{
  using std::cout;
  using std::endl;
  using FunG::CMath::LN;
  using FunG::CMath::Pow;
  using namespace ArmadilloDetail;
  constexpr int dim = 3;
  using M = arma::mat::fixed<dim,dim>;

  cout << "Starting performance test with static armadillo matrices.\n" << endl;

  auto c0 = 1.;
  auto c1 = 1.;
  auto d0 = 1.;
  auto d1 = 1.;
  M fiberTensor, I;
  I.eye();
  fiberTensor.zeros();
  fiberTensor(0,0) = 1.;

  auto incompressibleNeoHooke = FunG::incompressibleNeoHooke<M,3>(c0,I);
  auto incompressibleModifiedNeoHooke = FunG::modifiedIncompressibleNeoHooke<M,3>(c0,I);

  auto compressibleNeoHooke = FunG::compressibleNeoHooke<Pow<2>,LN>(c0,d0,d1,I);
  auto compressibleModifiedNeoHooke = FunG::modifiedCompressibleNeoHooke<Pow<2>,LN>(c0,d0,d1,I);

  auto incompressibleMooneyRivlin = FunG::incompressibleMooneyRivlin(c0,c1,I);
  auto compressibleMooneyRivlin = FunG::compressibleMooneyRivlin<Pow<2>,LN>(c0,c1,d0,d1,I);

  auto incompressibleSkin = FunG::incompressibleSkin_Hendriks(I);
  auto compressibleSkin = FunG::compressibleSkin_Hendriks<Pow<2>,LN>(d0,d1,I);

  auto incompressibleAdipose = FunG::incompressibleAdiposeTissue_SommerHolzapfel(fiberTensor,I);
  auto compressibleAdipose = FunG::compressibleAdiposeTissue_SommerHolzapfel<Pow<2>,LN>(d0,d1,fiberTensor,I);

  auto incompressibleMuscle = FunG::incompressibleMuscleTissue_Martins(fiberTensor,I);
  auto compressibleMuscle = FunG::compressibleMuscleTissue_Martins<Pow<2>,LN>(d0,d1,fiberTensor,I);

  using namespace std::chrono;
  auto startTime = high_resolution_clock::now();
  cout << "incompressible neo-Hooke" << endl;
  runTest<M>(incompressibleNeoHooke);

  cout << "modified incompressible neo-Hooke" << endl;
  runTest<M>(incompressibleModifiedNeoHooke);

  cout << "compressible neo-Hooke" << endl;
  runTest<M>(compressibleNeoHooke);

  cout << "modified compressible neo-Hooke" << endl;
  runTest<M>(compressibleModifiedNeoHooke);

  cout << "incompressible Mooney-Rivlin" << endl;
  runTest<M>(incompressibleMooneyRivlin);

  cout << "compressible Mooney-Rivlin" << endl;
  runTest<M>(compressibleMooneyRivlin);

  cout << "incompressible skin (Hendriks)" << endl;
  runTest<M>(incompressibleSkin);

  cout << "compressible skin (Hendriks)" << endl;
  runTest<M>(compressibleSkin);

  cout << "incompressible adipose (Sommer,Holzapfel et al)" << endl;
  runTest<M>(incompressibleAdipose);

  cout << "compressible adipose (Sommer,Holzapfel et al)" << endl;
  runTest<M>(compressibleAdipose);

  cout << "incompressible muscle (Martins et al)" << endl;
  runTest<M>(incompressibleMuscle);

  cout << "compressible muscle (Martins et al)" << endl;
  runTest<M>(compressibleMuscle);

  cout << "Terminating performance test with armadillo matrices.\n" << endl;
  cout << "Overall computation time: " << duration_cast<seconds>(high_resolution_clock::now() - startTime).count() << "s\n";
}

#endif /* FUNG_PERFORMANCE_TEST_EXAMPLE_TEST_HH */
